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

MyData mydata[5];
/* struct ui* ConnHead;  /1* head of linked list of UI conns *1/ */
/* int nui = 0;   /1* number of open UI connections *1/ */
/***************************************************************
 *   Here is the sample application column definitions.
 **************************************************************/
RTA_COLDEF   mycolumns[] = {
{
  "mytable",                /* the table name */
  "myint",                  /* the column name */
  RTA_INT,                  /* it is an integer */
  sizeof(int),              /* number of bytes */
  offsetof(struct MyData, myint), /* location in struct */
  RTA_DISKSAVE,             /* save to disk */
  (int (*)()) 0,            /* called before read */
  (int (*)()) 0,            /* called after write */
"A sample integer in a table"},
{
  "mytable",                /* the table name */
  "myfloat",                /* the column name */
  RTA_FLOAT,                /* it is a float */
  sizeof(float),            /* number of bytes */
  offsetof(struct MyData, myfloat), /* location in struct */
  0,                        /* no flags */
  (int (*)()) 0,            /* called before read */
  (int (*)()) 0,            /* called after write */
"A sample float in a table"},
{
  "mytable",                /* the table name */
  "notes",                  /* the column name */
  RTA_STR,                  /* it is a string */
  NOTE_LEN,                 /* number of bytes */
  offsetof(struct MyData, notes), /* location in struct */
  RTA_DISKSAVE,             /* save to disk */
  (int (*)()) 0,            /* called before read */
  reverse_str,              /* called after write */
"A sample note string in a table"},
{
  "mytable",                /* the table name */
  "seton",                  /* the column name */
  RTA_STR,                  /* it is a string */
  NOTE_LEN,                 /* number of bytes */
  offsetof(struct MyData, seton), /* location in struct */
  RTA_READONLY,             /* a read-only column */
  (int (*)()) 0,            /* called before read */
  (int (*)()) 0,            /* called after write */
"Sample of a field computed by a write callback.  Seton"
  " is the reverse of the 'notes' field."},
};

RTA_TBLDEF MyTable {
  "mytable",                /* table name */
  mydata,                   /* address of table */
  sizeof(struct MyData),    /* length of each row */
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

int reverse_str(char *tbl, char *col, char *sql,
                    void *pr, int rowid, void *por) {

  int      i, j;       /* loop counters */

  i = strlen(mydata[rowid].notes) - 1; /* -1 to ignore NULL */
  for (j = 0; i >= 0; i--, j++)
  {
    if (mydata[rowid].notes[i] == '<' || mydata[rowid].notes[i] == '>')
      mydata[rowid].notes[i] = '.';
    mydata[rowid].seton[j] = mydata[rowid].notes[i];
  }
  mydata[rowid].seton[j] = (char) 0;

  if (mydata[rowid].myint == 999)
    return(1);
  return(0);

}

sarab::sqlp::sqlpUnit::sqlpUnit(std::string parentId) : sarab::unit::unit(parentId) {

  //addFd(tcpSck.getFd(), eventType::EREAD);

}

int sarab::sqlp::sqlpUnit::listen_on_port(int port) {

  int      srvfd;      /* FD for our listen server socket */
  struct sockaddr_in srvskt;
  int      adrlen;
  int      flags;

  adrlen = sizeof(struct sockaddr_in);
  (void) memset((void *) &srvskt, 0, (size_t) adrlen);
  srvskt.sin_family = AF_INET;
  srvskt.sin_addr.s_addr = INADDR_ANY;
  srvskt.sin_port = htons(port);
  if ((srvfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    fprintf(stderr, "Unable to get socket for port %d.", port);
    exit(1);
  }
  flags = fcntl(srvfd, F_GETFL, 0);
  flags |= O_NONBLOCK;
  (void) fcntl(srvfd, F_SETFL, flags);
  if (bind(srvfd, (struct sockaddr *) &srvskt, adrlen) < 0)
  {
    fprintf(stderr, "Unable to bind to port %d\n", port);
    exit(1);
  }
  if (listen(srvfd, 1) < 0)
  {
    fprintf(stderr, "Unable to listen on port %d\n", port);
    exit(1);
  }
  return (srvfd);

}

void sarab::sqlp::sqlpUnit::handle_ui_output(struct ui *pui) {
  int      ret;        /* write() return value */

  if (pui->rspfree < MXRSP)
  {
    ret = write(pui->fd, pui->rsp, (MXRSP - pui->rspfree));
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
      /* Free the UI struct */
      /*if (pui->prevconn) {
        (pui->prevconn)->nextconn = pui->nextconn;
        std::cout << "dsdsds" << std::endl;
      } else {
        ConnHead = pui->nextconn;
        std::cout << "ConnHead changed" << std::endl;
      }
      if (pui->nextconn)
        (pui->nextconn)->prevconn = pui->prevconn;
      free(pui);
      nui--;
      */
      return;
    }
    else if (ret == (MXRSP - pui->rspfree))
    {
      pui->rspfree = MXRSP;
      pui->nbytout += ret;
    }
    else
    {
      /* we had a partial write.  Adjust the buffer */
      (void) memmove(pui->rsp, &(pui->rsp[ret]),
        (MXRSP - pui->rspfree - ret));
      pui->rspfree += ret;
      pui->nbytout += ret;  /* # bytes sent on conn */
    }
  }
}

void sarab::sqlp::sqlpUnit::handle_ui_request(struct ui *pui) {
  int      ret;        /* a return value */
  int      dbstat;     /* a return value */
  int      t;          /* a temp int */

  /* We read data from the connection into the buffer in the ui struct.
     Once we've read all of the data we can, we call the DB routine to
     parse out the SQL command and to execute it. */
  ret = read(pui->fd, &(pui->cmd[pui->cmdindx]), (MXCMD - pui->cmdindx));

  pui->cmdindx += ret;
  pui->nbytin += ret;

  /* The commands are in the buffer. Call the DB to parse and execute
     them */
  do
  {
    t = pui->cmdindx;                        /* packet in length */
    dbstat = rta_dbcommand(pui->cmd,         /* packet in */
      &(pui->cmdindx),                       /* packet in length */
      &(pui->rsp[MXRSP - pui->rspfree]),     /* ptr to out buf */
      &(pui->rspfree));                      /* N bytes at out */
    t -= pui->cmdindx;                       /* t = # bytes consumed */
    /* move any trailing SQL cmd text up in the buffer */
    (void) memmove(pui->cmd, &(pui->cmd[t]), t);
  } while (dbstat == RTA_SUCCESS);
  /* the command is done (including side effects).  Send any reply back
     to the UI.  You may want to check for RTA_CLOSE here. */
  handle_ui_output(pui);
}

void sarab::sqlp::sqlpUnit::accept_ui_session(int srvfd) {

  int      newuifd;    /* New UI FD */
  u_int    adrlen;     /* length of an inet socket address */
  struct sockaddr_in cliskt; /* socket to the UI/DB client */
  int      flags;      /* helps set non-blocking IO */
  struct ui newUi;       /* pointer to the new UI struct */
 // struct ui      *pui;        /* pointer to a UI struct */


  /* Accept the connection */
  adrlen = sizeof(struct sockaddr_in);
  newuifd = accept(srvfd, (struct sockaddr *) &cliskt, &adrlen);
  if (newuifd < 0) {
    syslog(LOG_ERR, "Manager accept() error");
    return;
  }

  /* We've accepted the connection.  Now get a UI structure.
     Are we at our limit?  If so, drop the oldest conn. */
  if (connVect.size() >= MX_UI)
  {
    LOG(WARNING) << "no manager connections";

    /* oldest conn is one at head of linked list.  Close it and
       promote next oldest to the top of the linked list.  */
    close(connVect[0].fd);
    //pui = ConnHead->nextconn;
    //free(ConnHead);
    //nui--;
    //ConnHead = pui;
    //ConnHead->prevconn = (struct ui *) NULL;
    connVect.erase(connVect.begin());
  }

  //pnew = malloc(sizeof (struct ui));
  //if (pnew == (struct ui *) NULL)
  //{
    /* Unable to allocate memory for new connection.  Log it,
       then drop new connection.  Try to go on.... */
    //syslog(LOG_ERR, "Unable to allocate memory");
    //close(newuifd);
    //return;
  //}
  //nui++;       /* increment number of UI structs alloc'ed */

  /* OK, we've got the UI struct, now add it to end of list */
 /* if (ConnHead == (struct ui *) NULL)
  {
    std::cout << "Hallo!" << pnew << std::endl;
    ConnHead = pnew;
    pnew->prevconn = (struct ui *) NULL;
    pnew->nextconn = (struct ui *) NULL;
  }
  else
  {
    pui = ConnHead;
    while (pui->nextconn != (struct ui *) NULL)
      pui = pui->nextconn;
    pui->nextconn = pnew;
    pnew->prevconn = pui;
    pnew->nextconn = (struct ui *) NULL;
  }
*/
  /* UI struct is now at end of list.  Fill it in.  */
  newUi.fd = newuifd;
  flags = fcntl(newUi.fd, F_GETFL, 0);
  flags |= O_NONBLOCK;
  (void) fcntl(newUi.fd, F_SETFL, flags);
  newUi.o_ip = (int) cliskt.sin_addr.s_addr;
  newUi.o_port = (int) ntohs(cliskt.sin_port);
  newUi.cmdindx = 0;
  newUi.rspfree = MXRSP;
  newUi.ctm = (int) time((time_t *) 0);
  newUi.nbytin = 0;
  newUi.nbytout = 0;
  connVect.push_back(newUi);

}

int sarab::sqlp::sqlpUnit::run()
{
  fd_set   rfds;       /* read bit masks for select statement */
  fd_set   wfds;       /* write bit masks for select statement */
  int      mxfd;       /* Maximum FD for the select statement */
  int      newui_fd = -1; /* FD to TCP socket accept UI conns */
  int      i;          /* generic loop counter */
  struct ui      *pui;        /* pointer to a UI struct */
  struct ui      *nextpui;    /* points to next UI in list */



  /* Init */
  /* ConnHead = (struct ui *) NULL; */

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

    /* open UI/DB/manager listener if needed */
    if (newui_fd < 0)
    {
      newui_fd = listen_on_port(DB_PORT);
    }
    FD_SET(newui_fd, &rfds);
    mxfd = (newui_fd > mxfd) ? newui_fd : mxfd;

    /* for each UI conn .... */
    /* pui = ConnHead; */
    for(auto i : connVect)
    {
      if (i.rspfree < MXRSP) /* Data to send? */
      {
        FD_SET(i.fd, &wfds);
        mxfd = (i.fd > mxfd) ? i.fd : mxfd;
      }
      else
      {
        FD_SET(i.fd, &rfds);
        mxfd = (i.fd > mxfd) ? i.fd : mxfd;
      }
      /* pui = pui->nextconn; */
    }

    /* Wait for some something to do */
    (void) select(mxfd + 1, &rfds, &wfds,
      (fd_set *) 0, (struct timeval *) 0);

    /* ....after the select call.  We have activity. Search through
       the open fd's to find what to do. */

    /* Handle new UI/DB/manager connection requests */
    if ((newui_fd >= 0) && (FD_ISSET(newui_fd, &rfds)))
    {
      accept_ui_session(newui_fd);
    }

    /* process request from or data to one of the UI programs */
    /* pui = ConnHead; */
    for (auto i : connVect)
    {
      /* Get next UI now since pui struct may be freed in handle_ui.. */
      /* nextpui = pui->nextconn; */
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
