#include "os_socket.h"

#include <syslog.h>
#include <system_error>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <thread>
#include <sstream>

using namespace sarab::os;

OSSocket::OSSocket() {

  sockAddrUn addr;

  std::stringstream ss;
  ss << std::this_thread::get_id();
  m_path = m_pathStart + ss.str();

  m_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
  if(m_fd == -1) {
    syslog(LOG_ERR, "%s:%d Can't create the socket: %s", __FUNCTION__,
                  __LINE__, strerror(errno));
    throw std::system_error(errno, std::generic_category());
  }

  if(m_path.size() > sizeof(addr.sun_path) - 1) {
    syslog(LOG_ERR, "%s:%d Socket path is too long!", __FUNCTION__, __LINE__);
    // FIXME: throw user-defined exception
  }

  if(remove(m_path.c_str()) == -1 && errno != ENOENT) {
    syslog(LOG_ERR, "%s:%d Can't remove the path %s", __FUNCTION__,
                  __LINE__, strerror(errno));
    throw std::system_error(errno, std::generic_category());
  }

  memset(&addr, 0, sizeof(struct sockaddr_un));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, m_path.c_str(), sizeof(addr.sun_path) -1);

  if(bind(m_fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1) {
    syslog(LOG_ERR, "%s:%d Can't bind the socket: %s", __FUNCTION__,
                  __LINE__, strerror(errno));
    throw std::system_error(errno, std::generic_category());
  }

  // memset the target's address only once
  memset(&m_targetAddr, 0, sizeof(sockAddrUn));
  m_targetAddr.sun_family = AF_UNIX;

}

OSSocket::~OSSocket() {

  if(close(m_fd) == -1)
    syslog(LOG_ERR, "%s:%d Can't close the socket: %s", __FUNCTION__,
                  __LINE__, strerror(errno));

  if(remove(m_path.c_str()) == -1 && errno != ENOENT)
    syslog(LOG_ERR, "%s:%d Can't remove the path: %s", __FUNCTION__,
                  __LINE__, strerror(errno));
}

void OSSocket::send(struct msg* myMsg, std::string targetPath) {

  strncpy(m_targetAddr.sun_path, targetPath.c_str(), sizeof(m_targetAddr.sun_path) - 1);

  if(sendto(m_fd, myMsg, sizeof(struct msg), 0, (struct sockaddr*)&m_targetAddr,
            sizeof(sockAddrUn)) != sizeof(struct msg)) {
    syslog(LOG_ERR, "%s:%d Sending failed: %s", __FUNCTION__,
                  __LINE__, strerror(errno));
    throw std::system_error(errno, std::generic_category());
  }

}

void OSSocket::recv(struct msg* myMsg) {

  int numBytes = recvfrom(m_fd, myMsg, sizeof(struct msg), 0, NULL, NULL);
  if(numBytes == -1) {
    syslog(LOG_ERR, "%s:%d Recieving failed: %s", __FUNCTION__,
                  __LINE__, strerror(errno));
    throw std::system_error(errno, std::generic_category());
  }

}