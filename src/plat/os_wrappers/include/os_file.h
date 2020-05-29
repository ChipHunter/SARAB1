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
  virtual ~OSFile();

  void write(const char* buf, int len) const;
  void read(char* buf, int len)        const;
  int  getFd()                         const {return m_fd;};

};

}

}

#endif /*OS_FILE_H_ */
