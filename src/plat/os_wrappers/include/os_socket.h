#ifndef OS_SOCKET_H_
#define OS_SOCKET_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <vector>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>


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

  OSSocket(const OSSocket&)            = delete;
  OSSocket(OSSocket&&)                 = default;
  OSSocket& operator=(const OSSocket&) = delete;
  OSSocket& operator=(OSSocket&&)      = default;

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

class SOSSocket {

  using sockAddrUn = struct sockaddr_un;

public:
  SOSSocket();
  ~SOSSocket();

  SOSSocket(const SOSSocket&)            = delete;
  SOSSocket(SOSSocket&&)                 = default;
  SOSSocket& operator=(const SOSSocket&) = delete;
  SOSSocket& operator=(SOSSocket&&)      = default;

  void send(struct msg* myMsg, std::string targetPath);
  void recv(struct msg* myMsg);
  int acceptCon();

  int          getFd()   const { return m_fd; }
  std::string  getAddr() const { return m_sckAddr; }

private:
  int                 m_fd;
  std::string         m_sckAddr;
  sockAddrUn          m_targetAddr;
  sarab::utils::utils m_utils;
};

class TCPSocket {

  using sockAddrIn = struct sockaddr_in;
  using sockIpAddr = struct in_addr;
  using SA         = struct sockaddr;

public:
  TCPSocket(int port);
  ~TCPSocket();

  TCPSocket(const TCPSocket&)            = delete;
  TCPSocket(TCPSocket&&)                 = default;
  TCPSocket& operator=(const TCPSocket&) = delete;
  TCPSocket& operator=(TCPSocket&&)      = default;

  void send(struct msg* myMsg, std::string targetPath);
  void recv(struct msg* myMsg);
  int acceptCon();

  int          getFd()   const { return m_fd; }
  std::string  getAddr() const { return "";}

private:
  int                 m_fd;
  int                 m_port;
  sarab::utils::utils m_utils;
};
}

}
#endif
