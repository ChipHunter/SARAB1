#ifndef SQLPARSER_UNIT_H_
#define SQLPARSER_UNIT_H_

#include "unit.h"

namespace sarab {

namespace sqlp {

class sqlpFuncObj {
public:
  sqlpFuncObj(std::thread::id id);
  void operator()();


private:
  std::string m_parentId;
  sarab::utils::utils ut{};
};

}

}
#endif
