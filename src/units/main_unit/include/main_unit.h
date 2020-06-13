#ifndef MAIN_UNIT_H_
#define MAIN_UNIT_H_

#include <string>

#include "unit.h"

namespace sarab {

namespace mainu {

class mainUnit : public sarab::unit::unit {
public:
  mainUnit(std::string parentId) : sarab::unit::unit(parentId) {}

  ~mainUnit()                           = default;
  mainUnit(const mainUnit&)             = default;
  mainUnit(mainUnit&&)                  = default;
  mainUnit& operator=(const mainUnit&)  = default;
  mainUnit& operator=(mainUnit&&)       = default;
};

}

}

#endif
