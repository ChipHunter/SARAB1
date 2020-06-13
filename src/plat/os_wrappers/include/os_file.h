#ifndef OS_FILE_H_
#define OS_FILE_H_
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace sarab {

namespace os {

class OSFile {
private:
  int m_fd;

public:
  OSFile(const char* name, int flags);
  ~OSFile();
  OSFile(const OSFile&)            = delete;
  OSFile(OSFile&&)                 = default;
  OSFile& operator=(const OSFile&) = delete;
  OSFile& operator=(OSFile&&)      = default;

  void write(const char* buf, int len) ;
  void read(char* buf, int len)        ;
  int  getFd()                         const {return m_fd;}

};

}

}

#endif /*OS_FILE_H_ */
