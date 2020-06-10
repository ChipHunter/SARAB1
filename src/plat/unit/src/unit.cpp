#include "unit.h"
#include "easylogging++.h"

using eventType = sarab::defs::defs::defs::eventType;

sarab::unit::unit::unit(std::string parentId) {

  if(!parentId.empty()) {
    m_parentId = parentId;
    m_parentAddr = m_utils.computeSckAddr(parentId);
  }

  // FIXME: see which flags we should set
  m_epoll.addFd(m_uSck.getFd(), EPOLLIN | EPOLLET);

}


void sarab::unit::unit::waitForEvents(sarab::os::eventsVect& events) {

  m_epoll.waitForEvents(events, m_maxEvents);

}

void sarab::unit::unit::addFd(int fd, eventType event) {

  switch(event) {
    case eventType::EREAD:
      m_epoll.addFd(fd, EPOLLIN);
      break;
    case eventType::EWRITE:
      m_epoll.addFd(fd, EPOLLOUT);
      break;
    case eventType::EREWR:
      m_epoll.addFd(fd, EPOLLIN | EPOLLOUT);
      break;
    default:
      LOG(ERROR) << "Not a valid a event!";
      //FIXME: add user-defined exceptions
  }
}

void sarab::unit::unit::delFd(int fd) {

  m_epoll.delFd(fd);

}

void sarab::unit::unit::modFd(int fd, eventType event) {

  switch(event) {
    case eventType::EREAD:
      m_epoll.modFd(fd, EPOLLIN);
      break;
    case eventType::EWRITE:
      m_epoll.modFd(fd, EPOLLOUT);
      break;
    case eventType::EREWR:
      m_epoll.modFd(fd, EPOLLIN | EPOLLOUT);
      break;
    default:
      LOG(ERROR) << "Not a valid a event!";
      //FIXME: add user-defined exceptions
  }

}

void sarab::unit::unit::sendMsg(struct msg* myMsg, std::string destAddr) {

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

void sarab::unit::unit::recvMsg(struct msg* myMsg) {

  m_uSck.recv(myMsg);

}
