#ifndef SQLPARSER_UNIT_H_
#define SQLPARSER_UNIT_H_

#include <string>
#include <vector>

#include "unit.h"

extern "C" {
  #include "librta.h"
}

// FIXME: move them somewhere else
#define ROW_COUNT  2

/* #define DB_PORT 8888 */


/*  -structure definitions */

namespace sarab {

namespace sqlp {

struct ui
{
  static inline const int maxCmd = 1000;
  static inline const int maxRsp = 50000;
  static inline const int maxUi  = 20;

  int        fd;         /* FD of TCP conn (=-1 if not in use) */
  int        cmdindx;    /* Index of next location in cmd buffer */
  char       cmd[maxCmd]; /* SQL command from UI program */
  int        rspfree;    /* Number of free bytes in rsp buffer */
  char       rsp[maxRsp]; /* SQL response to the UI program */
  int        o_port;     /* Other-end TCP port number */
  int        o_ip;       /* Other-end IP address */
  llong      nbytin;     /* number of bytes read in */
  llong      nbytout;    /* number of bytes sent out */
  int        ctm;        /* connect time (==time();) */
  int        cdur;       /* duration time (== now()-ctm;) */
};

struct MyData
{
  int      myint;
};

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
  /* int  listen_on_port(int port); */
  void accept_ui_session();
  void handle_ui_output(struct ui* pui);
  void handle_ui_request(struct ui* pui);

private:
  std::vector<struct ui> connVect;
  const int portNr = 8888;

  sarab::os::TCPSocket tcpSck{portNr};
};

}

}

#endif
