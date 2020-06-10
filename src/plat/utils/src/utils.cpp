#include "utils.h"
#include <sstream>

std::string sarab::utils::utils::computeSckAddr(std::thread::id id) {

  std::stringstream ss;
  ss << d.getSckLocation() << d.getSckNamePre() << id;

  return ss.str();

}

std::string sarab::utils::utils::computeSckAddr(std::string id) const {

  return d.getSckLocation() + d.getSckNamePre() + id;

}

std::string sarab::utils::utils::threadIdToString(std::thread::id id) {

  std::stringstream ss;

  ss << id;

  return ss.str();

}
