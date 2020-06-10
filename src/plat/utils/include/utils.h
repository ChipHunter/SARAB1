#ifndef UTILS_H_
#define UTILS_H_

#include <thread>

#include "defs.h"

namespace sarab {

namespace utils {

class utils {
public:
  std::string computeSckAddr(std::thread::id id);
  std::string computeSckAddr(std::string id) const;
  std::string threadIdToString(std::thread::id id);

private:
  sarab::defs::defs d{};
};

}

}
#endif
