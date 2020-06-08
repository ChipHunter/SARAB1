#ifndef OS_SOCKET_H_
#define OS_SOCKET_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <vector>
#include <sys/un.h>

#include "utils.h"

// FIXME: this is just temporary for testing purposes
struct msg {
  char senderPath[48];
  char message[148];
};

namespace sarab {

namespace os {


class OSSocket {

  using sockAddrUn = struct sockaddr_un;

public:
  OSSocket();
  ~OSSocket();

  void send(struct msg* myMsg, std::string targetPath);
  void recv(struct msg* myMsg);

  int          getFd()   const { return m_fd; }
  std::string  getAddr() const { return m_sckAddr; }

private:
  int                 m_fd;
  std::string         m_sckAddr;
  sockAddrUn          m_targetAddr;
  sarab::utils::utils m_utils;
};

}

}
#endif
