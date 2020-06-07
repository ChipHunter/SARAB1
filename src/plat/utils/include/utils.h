#ifndef UTILS_H_
#define UTILS_H_

namespace sarab {

namespace utils {

class utils {
public:
  std::string getSckLocation() const { return sckLocation; }
  std::string getSckNamePre()  const { return sckNamePre;  }
  
private:
  const std::string sckLocation  = "/tmp/"; 
  const std::string sckNamePre   = "sck_"; 
};

}

}
#endif
