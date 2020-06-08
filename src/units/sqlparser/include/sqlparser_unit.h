#ifndef SQLPARSER_UNIT_H_
#define SQLPARSER_UNIT_H_

#include <iostream>
#include "unit.h"

namespace sarab {

namespace sqlp {

class funcObj {
public:
  funcObj(std::thread::id id) : m_parentId{id} {}
  void operator()() { std::cout << "from thread!" << ut.computeSckAddr(m_parentId) << std::endl; };

private:
  const std::thread::id m_parentId;
  sarab::utils::utils ut{};
};

}

}
#endif
