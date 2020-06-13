#include <system_error>
#include <unistd.h>
#include <string.h>

#include "os_file.h"
#include "easylogging++.h"


sarab::os::OSFile::OSFile(const char* name, int flags) {

  if((m_fd = open(name, flags)) == -1){
    LOG(ERROR) << "Can't open the file: " << strerror(errno);
    throw std::system_error(errno, std::generic_category());
  }

}

sarab::os::OSFile::~OSFile() {

  if (close(m_fd) == -1) {
    LOG(ERROR) << "Can't close the file: " << strerror(errno);
  }

}

void sarab::os::OSFile::write(const char* buf, int len) {

  if(::write(m_fd, buf, len) != len){
    if (errno) {
      LOG(ERROR) << "Can't write to the file: " << strerror(errno);
      throw std::system_error(errno, std::generic_category());
    } else {
      LOG(WARNING) << "Not all the requested buffer was written!";
    }
  }

}

void sarab::os::OSFile::read(char* buf, int len) {

  if(::read(m_fd, buf, len) != len){
    if (errno) {
      LOG(ERROR) << "Can't read from the file: " << strerror(errno);
      throw std::system_error(errno, std::generic_category());
    } else {
      LOG(WARNING) << "Not all the rquested buffer was read!";
    }
  }
}
