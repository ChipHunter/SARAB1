#ifndef DEFS_H_
#define DEFS_H_

namespace sarab {

namespace defs {

class defs {
public:
  enum class eventType { EREAD, EWRITE, EREWR, UNDEF };

  std::string getSckLocation() const { return m_sckLocation; }
  std::string getSckNamePre()  const { return m_sckNamePre;  }

private:
  const std::string m_sckLocation  = "/tmp/";
  const std::string m_sckNamePre   = "sck_";
};



}

}

#endif
