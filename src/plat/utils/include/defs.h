#ifndef DEFS_H_
#define DEFS_H_

#include <string>

namespace sarab {

namespace defs {

class defs {
public:
  enum class eventType { EREAD, EWRITE, EREWR, UNDEF };

  std::string getSckLocation()     const { return m_sckLocation;      }
  std::string getSckNamePre()      const { return m_sckNamePre;       }
  std::string getLogConfLocation() const { return m_logConfLocation;  }

private:
  const std::string m_sckLocation       = "/tmp/";
  const std::string m_sckNamePre        = "sck_";
  const std::string m_logConfLocation   = "/tmp/easyloggingpp.conf";
};

}

}

#endif
