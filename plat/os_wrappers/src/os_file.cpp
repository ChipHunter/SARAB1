#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <system_error>
#include <unistd.h>

#include "os_file.h"



OSFile::OSFile(const char* name, int flags) {

  if((m_fd = open(name, flags)) == -1){
    throw std::system_error(errno, std::generic_category());
  }

}

OSFile::~OSFile() {

  if (close(m_fd) == -1) {
   //FIXME: log a message
  }

}

void OSFile::write(const char* buf, int len) const {

  if(::write(m_fd, buf, len) != len){
    if (errno)
      throw std::system_error(errno, std::generic_category());
    // FIXME: else log a message that the complate buffer was not written
  }

}

void OSFile::read(char* buf, int len) const {

  if(::read(m_fd, buf, len) != len){
    if (errno)
      throw std::system_error(errno, std::generic_category());
    // FIXME: else log a message that the complate buffer was not written
  }

}
