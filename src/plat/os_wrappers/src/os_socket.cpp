#include <system_error>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <thread>

#include "os_socket.h"
#include "easylogging++.h"

using namespace sarab::os;

OSSocket::OSSocket() {

  sockAddrUn addr;

  m_sckAddr = m_utils.computeSckAddr(std::this_thread::get_id());

  m_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
  if(m_fd == -1) {
    LOG(ERROR) << "Can't create the socket: " << strerror(errno);
    throw std::system_error(errno, std::generic_category());
  }

  if(m_sckAddr.size() > sizeof(addr.sun_path) - 1) {
    LOG(ERROR) << "Socket path is too long! " << strerror(errno);
    // FIXME: throw user-defined exception
  }

  if(remove(m_sckAddr.c_str()) == -1 && errno != ENOENT) {
    LOG(ERROR) << "Can't remove the path " << strerror(errno);
    throw std::system_error(errno, std::generic_category());
  }

  memset(&addr, 0, sizeof(struct sockaddr_un));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, m_sckAddr.c_str(), sizeof(addr.sun_path) -1);

  if(bind(m_fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1) {
    LOG(ERROR) << "Can't bind the socket " << strerror(errno);
    throw std::system_error(errno, std::generic_category());
  }

  // memset the target's address only once
  memset(&m_targetAddr, 0, sizeof(sockAddrUn));
  m_targetAddr.sun_family = AF_UNIX;

}

OSSocket::~OSSocket() {

  if(close(m_fd) == -1)
    LOG(ERROR) << "Can't close the socket " << strerror(errno);

  if(remove(m_sckAddr.c_str()) == -1 && errno != ENOENT)
    LOG(ERROR) << "Can't remove the path: " << strerror(errno);
}

void OSSocket::send(struct msg* myMsg, std::string targetPath) {

  strncpy(m_targetAddr.sun_path, targetPath.c_str(), sizeof(m_targetAddr.sun_path) - 1);

  if(sendto(m_fd, &myMsg, sizeof(struct msg), 0, (struct sockaddr*)&m_targetAddr,
            sizeof(sockAddrUn)) != sizeof(struct msg)) {
    LOG(ERROR) << "Sending failed: " << strerror(errno);
    throw std::system_error(errno, std::generic_category());
  }

}

void OSSocket::recv(struct msg* myMsg) {

  int numBytes = recvfrom(m_fd, myMsg, sizeof(struct msg), 0, NULL, NULL);
  if(numBytes == -1) {
    LOG(ERROR) << "Recieving failed: " << strerror(errno);
    throw std::system_error(errno, std::generic_category());
  }

}
