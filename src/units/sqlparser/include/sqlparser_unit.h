#ifndef SQLPARSER_UNIT_H_
#define SQLPARSER_UNIT_H_

#include <string>
#include <vector>

#include "unit.h"

extern "C" {
  #include "librta.h"
}

//FIXME: remove define
/* Maximum number of UI/Posgres connections */
#define MX_UI     (20)

/* Max size of a Postgres packet from/to the UI's */
#define MXCMD      (1000)
#define MXRSP     (50000)

/* Note length and number of rows in the "mydata" table. */
#define NOTE_LEN   20
#define ROW_COUNT  20

#define DB_PORT 8888

int reverse_str(char *tbl, char *col, char *sql, void *pr, int rowid,
                void *por);
/***************************************************************
 * the information kept for TCP connections to UI programs
 **************************************************************/
struct ui
{
  struct ui  *prevconn;   /* Points to previous conn in linked list */
  struct ui  *nextconn;   /* Points to next conn in linked list */
  int      fd;         /* FD of TCP conn (=-1 if not in use) */
  int      cmdindx;    /* Index of next location in cmd buffer */
  char     cmd[MXCMD]; /* SQL command from UI program */
  int      rspfree;    /* Number of free bytes in rsp buffer */
  char     rsp[MXRSP]; /* SQL response to the UI program */
  int      o_port;     /* Other-end TCP port number */
  int      o_ip;       /* Other-end IP address */
  llong    nbytin;     /* number of bytes read in */
  llong    nbytout;    /* number of bytes sent out */
  int      ctm;        /* connect time (==time();) */
  int      cdur;       /* duration time (== now()-ctm;) */
};

/*  -structure definitions */
/***************************************************************
 * the structure for the sample application table.  This is
 * where you would put your real application tables.
 **************************************************************/
struct MyData
{
  int      myint;
  float    myfloat;
  char     notes[NOTE_LEN];
  char     seton[NOTE_LEN];
};

namespace sarab {

namespace sqlp {



class sqlpFuncObj {
public:
  sqlpFuncObj(std::thread::id id);
  void operator()();


private:
  std::string m_parentId;
};

class sqlpUnit : public sarab::unit::unit {
public:
  sqlpUnit(std::string parentId);

  ~sqlpUnit()                           = default;
  sqlpUnit(const sqlpUnit&)             = default;
  sqlpUnit(sqlpUnit&&)                  = default;
  sqlpUnit& operator=(const sqlpUnit&)  = default;
  sqlpUnit& operator=(sqlpUnit&&)       = default;

  int run();

private:
  int  listen_on_port(int port);
  void accept_ui_session(int srvfd);
  void handle_ui_output(struct ui* pui);
  void handle_ui_request(struct ui* pui);

private:
  std::vector<struct ui> connVect;
//  sarab::os::TCPSocket tcpSck{};
};

}

}

#endif
