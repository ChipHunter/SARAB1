#include "utils.h"
#include <sstream>

using namespace std;
using namespace sarab::utils;

string utils::computeSckAddr(thread::id id) {

  stringstream ss;
  ss << sckLocation << sckNamePre << id;

  return ss.str();

}
