#include <system_error>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

#include "os_file.h"

using sarab::os::OSFile;

OSFile::OSFile(const char* name, int flags) {

  if((m_fd = open(name, flags)) == -1){
    throw std::system_error(errno, std::generic_category());
  }

}

OSFile::~OSFile() {

  if (close(m_fd) == -1) {
    syslog(LOG_ERR, "%s:%d Can't close the file: %s", __FUNCTION__, __LINE__
        , strerror(errno));   
  }

}

void OSFile::write(const char* buf, int len) const {

  if(::write(m_fd, buf, len) != len){
    if (errno) {
      syslog(LOG_ERR, "%s:%d Can't write to the file: %s", __FUNCTION__, 
          __LINE__, strerror(errno));   
      throw std::system_error(errno, std::generic_category());
  
    } else {
      syslog(LOG_WARNING, "%s:%d Not all the requested buffer was written!", __FUNCTION__, 
          __LINE__);   
    }
  }

}

void OSFile::read(char* buf, int len) const {

  if(::read(m_fd, buf, len) != len){
    if (errno) {
      syslog(LOG_ERR, "%s:%d Can't read from the file: %s", __FUNCTION__, 
          __LINE__, strerror(errno));   
      throw std::system_error(errno, std::generic_category());
    } else {
      syslog(LOG_WARNING, "%s:%d Not all the rquested buffer was read!", __FUNCTION__, 
          __LINE__);   
    }
  }
}
