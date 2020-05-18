#ifndef OS_SOCKET_H_
#define OS_SOCKET_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <vector>
#include <sys/un.h>

namespace sarab {

namespace os {

// FIXME: this is just temporary for testing purposes
struct msg {
  char senderPath[48];
  char message[148];
};

class OSSocket {

  using sockAddrUn = struct sockaddr_un;

public:
  OSSocket();
  ~OSSocket();

  void send(struct msg* myMsg, std::string targetPath);
  void recv(struct msg* myMsg);

private:
        int         m_fd;
        std::string m_path;
  const std::string m_pathStart = "/tmp/sck_";
        sockAddrUn  m_targetAddr;
};

}

}
#endif
