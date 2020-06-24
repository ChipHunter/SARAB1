#ifndef SQLPARSER_UNIT_H_
#define SQLPARSER_UNIT_H_

#include <string>

#include "unit.h"

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
private:
  sarab::os::TCPSocket tcpSck{};
};

}

}

#endif
