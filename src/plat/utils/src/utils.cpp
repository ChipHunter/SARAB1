#include "utils.h"
#include <sstream>

using namespace std;
using namespace sarab::utils;

string utils::computeSckAddr(thread::id id) {

  stringstream ss;
  ss << d.getSckLocation() << d.getSckNamePre() << id;

  return ss.str();

}

string utils::computeSckAddr(string id) const {

  return d.getSckLocation() + d.getSckNamePre() + id;

}
