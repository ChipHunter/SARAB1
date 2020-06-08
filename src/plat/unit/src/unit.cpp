#include "unit.h"

using namespace sarab::unit;
using namespace sarab::utils;
using namespace sarab::os;

using namespace std;

unit::unit(string parentId) {

  if(!parentId.empty()) {
    m_parentId = parentId;
    m_parentAddr = m_utils.getSckLocation() + m_utils.getSckNamePre() + parentId;
  }

  // FIXME: see which flags we should set
  m_epoll.addFd(m_uSck.getFd(), EPOLLIN | EPOLLET);

}


void unit::waitForEvents(eventsVect& events) {

  m_epoll.waitForEvents(events, m_maxEvents);

}

void unit::addFd(int fd, int mask) {

  m_epoll.addFd(fd, mask);

}

void unit::delFd(int fd) {

  m_epoll.delFd(fd);

}

void unit::modFd(int fd, int mask) {

  m_epoll.modFd(fd, mask);

}

void unit::sendMsg(struct msg* myMsg, string destAddr) {

  if (!destAddr.empty()) {

    /* in case we are in the parent thread we need to know to which child
       we are sending */
    m_uSck.send(myMsg, destAddr);

  } else {

    /* in case we are in a child thread, we send and receive only from
       the only parent */
    m_uSck.send(myMsg, m_parentAddr);

  }

}

void unit::recvMsg(struct msg* myMsg) {

  m_uSck.recv(myMsg);

}
