#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/select.h>

#include "sqlparser_unit.h"
#include "utils.h"
#include "easylogging++.h"

using eventType = sarab::defs::defs::defs::eventType;
using ui        = sarab::sqlp::ui;

sarab::sqlp::MyData mydata[5];

/***************************************************************
 *   Here is the sample application column definitions.
 **************************************************************/
RTA_COLDEF   mycolumns[] = {
{
  "mytable",                /* the table name */
  "myint",                  /* the column name */
  RTA_INT,                  /* it is an integer */
  sizeof(int),              /* number of bytes */
  offsetof(struct sarab::sqlp::MyData, myint), /* location in struct */
  RTA_DISKSAVE,             /* save to disk */
  (int (*)()) 0,            /* called before read */
  (int (*)()) 0,            /* called after write */
"A sample integer in a table"},
};

RTA_TBLDEF MyTable {
  "mytable",                /* table name */
  mydata,                   /* address of table */
  sizeof(struct sarab::sqlp::MyData),    /* length of each row */
  ROW_COUNT,                /* number of rows */
  (void *) NULL,            /* iterator function */
  (void *) NULL,            /* iterator callback data */
  (void *) NULL,            /* INSERT callback */
  (void *) NULL,            /* DELETE callback */
  mycolumns,                /* array of column defs */
  sizeof(mycolumns) / sizeof(RTA_COLDEF),
  /* the number of columns */
  "/tmp/mysavefile",        /* save file name */
"A sample application table"
};

sarab::sqlp::sqlpUnit::sqlpUnit(std::string parentId) : sarab::unit::unit(parentId) {

  //addFd(tcpSck.getFd(), eventType::EREAD);

}

void sarab::sqlp::sqlpUnit::handle_ui_output(ui* pui) {
  int      ret;        /* write() return value */

  if (pui->rspfree < ui::maxRsp)
  {
    ret = write(pui->fd, pui->rsp, (ui::maxRsp - pui->rspfree));
    if (ret < 0)
    {
      /* log a failure to talk to a DB/UI connection */
      fprintf(stderr,
        "error #%d on ui write to port #%d on IP=%d\n",
        errno, pui->o_port, pui->o_ip);
      close(pui->fd);
      auto pred = [=](const struct ui& item) {
        return item.fd == pui->fd;
      };
      auto i = std::find_if(std::begin(connVect), std::end(connVect), pred);
      connVect.erase(i);
      return;
    }
    else if (ret == (ui::maxRsp - pui->rspfree))
    {
      pui->rspfree = ui::maxRsp;
      pui->nbytout += ret;
    }
    else
    {
      /* we had a partial write.  Adjust the buffer */
      (void) memmove(pui->rsp, &(pui->rsp[ret]),
        (ui::maxRsp - pui->rspfree - ret));
      pui->rspfree += ret;
      pui->nbytout += ret;  /* # bytes sent on conn */
    }
  }
}

void sarab::sqlp::sqlpUnit::handle_ui_request(ui* pui) {
  int      ret;        /* a return value */
  int      dbstat;     /* a return value */
  int      t;          /* a temp int */

  /* We read data from the connection into the buffer in the ui struct.
     Once we've read all of the data we can, we call the DB routine to
     parse out the SQL command and to execute it. */
  ret = read(pui->fd, &(pui->cmd[pui->cmdindx]), (ui::maxCmd - pui->cmdindx));

  pui->cmdindx += ret;
  pui->nbytin += ret;

  /* The commands are in the buffer. Call the DB to parse and execute
     them */
  do
  {
    t = pui->cmdindx;                        /* packet in length */
    dbstat = rta_dbcommand(pui->cmd,         /* packet in */
      &(pui->cmdindx),                       /* packet in length */
      &(pui->rsp[ui::maxRsp - pui->rspfree]),     /* ptr to out buf */
      &(pui->rspfree));                      /* N bytes at out */
    t -= pui->cmdindx;                       /* t = # bytes consumed */
    /* move any trailing SQL cmd text up in the buffer */
    (void) memmove(pui->cmd, &(pui->cmd[t]), t);
  } while (dbstat == RTA_SUCCESS);
  /* the command is done (including side effects).  Send any reply back
     to the UI.  You may want to check for RTA_CLOSE here. */
  handle_ui_output(pui);
}

void sarab::sqlp::sqlpUnit::accept_ui_session() {

  int      newuifd;    /* New UI FD */
  struct sockaddr_in cliskt; /* socket to the UI/DB client */
  int      flags;      /* helps set non-blocking IO */
  struct ui newUi;       /* pointer to the new UI struct */


  /* Accept the connection */
  // FIXME: we should return a new TCPSocket instead of fd!!
  newuifd = tcpSck.acceptCon();

  /* We've accepted the connection.  Now get a UI structure.
     Are we at our limit?  If so, drop the oldest conn. */
  if (connVect.size() >= ui::maxUi)
  {
    LOG(WARNING) << "no manager connections";
    close(connVect[0].fd);
    connVect.erase(connVect.begin());
  }

  /* Fill UI struct in.  */
  newUi.fd = newuifd;
  flags = fcntl(newUi.fd, F_GETFL, 0);
  flags |= O_NONBLOCK;
  if (fcntl(newUi.fd, F_SETFL, flags) == -1) {
    LOG(ERROR) << "Can't set flags using fcntl" << strerror(errno);
    throw std::system_error(errno, std::generic_category());
  }
  newUi.o_ip = (int) cliskt.sin_addr.s_addr;
  newUi.o_port = (int) ntohs(cliskt.sin_port);
  newUi.cmdindx = 0;
  newUi.rspfree = ui::maxRsp;
  newUi.ctm = (int) time((time_t *) 0);
  newUi.nbytin = 0;
  newUi.nbytout = 0;
  connVect.push_back(newUi);

}

int sarab::sqlp::sqlpUnit::run()
{
  fd_set rfds;       /* read bit masks for select statement */
  fd_set wfds;       /* write bit masks for select statement */
  int    mxfd;       /* Maximum FD for the select statement */
  int    newui_fd = tcpSck.getFd(); /* FD to TCP socket accept UI conns */
  ui*    pui;        /* pointer to a UI struct */
  ui*    nextpui;    /* points to next UI in list */

  rta_add_table(&MyTable);

  while (1)
  {
    /* Build the fd_set for the select call.  This includes the listen
       port for new UI connections and any existing UI connections.  We
       also look for the ability to write to the clients if data is
       queued.  */
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    mxfd = 0;

    FD_SET(newui_fd, &rfds);
    mxfd = (newui_fd > mxfd) ? newui_fd : mxfd;

    /* for each UI conn .... */
    for(auto i : connVect)
    {
      if (i.rspfree < ui::maxRsp) /* Data to send? */
      {
        FD_SET(i.fd, &wfds);
        mxfd = (i.fd > mxfd) ? i.fd : mxfd;
      }
      else
      {
        FD_SET(i.fd, &rfds);
        mxfd = (i.fd > mxfd) ? i.fd : mxfd;
      }
    }

    /* Wait for some something to do */
    // FIXME: replace select with Epoll
    (void) select(mxfd + 1, &rfds, &wfds,
      (fd_set *) 0, (struct timeval *) 0);

    /* ....after the select call.  We have activity. Search through
       the open fd's to find what to do. */

    /* Handle new UI/DB/manager connection requests */
    if ((newui_fd >= 0) && (FD_ISSET(newui_fd, &rfds)))
    {
      accept_ui_session();
    }

    /* process request from or data to one of the UI programs */
    /* pui = ConnHead; */
    for (auto i : connVect)
    {
      if (FD_ISSET(i.fd, &rfds))
      {
        handle_ui_request(&i);
      }
      else if (FD_ISSET(i.fd, &wfds))
      {
        handle_ui_output(&i);
      }
    }
  }
}

sarab::sqlp::sqlpFuncObj::sqlpFuncObj(std::thread::id id) {

  sarab::utils::utils ut{};
  m_parentId = ut.threadIdToString(id);

}

void sarab::sqlp::sqlpFuncObj::operator()() {

  std::unique_ptr<sarab::sqlp::sqlpUnit> u(new sarab::sqlp::sqlpUnit{m_parentId});
  u->run();
  /*sarab::os::eventsVect events;
  struct msg myMsg;
  std::unique_ptr<sarab::unit::unit> u(new sarab::sqlp::sqlpUnit{m_parentId});
  LOG(ERROR) << "waiting for events in the thread";
  u->waitForEvents(events);
  LOG(ERROR) << "got it!!";
  u->sendMsg(&myMsg);*/
/*  for(int i = 0; i < 3; i++) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    u->waitForEvents(events);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    u->sendMsg(&myMsg);
    std::cout << "Ahlan!" << std::endl;
  }*/
    std::this_thread::sleep_for(std::chrono::seconds(1));
}
