#include "os_epoll.h"
#include "os_carray.h"

using sarab::os::OSEpoll;

OSEpoll::OSEpoll() {
  
  m_epollFd = epoll_create(m_epollSize);
  if (m_epollFd == -1)
    throw std::system_error(errno, std::generic_category()); 

}

OSEpoll::~OSEpoll() {

}

void OSEpoll::addFd(int fd, int eventMask) {
 
  struct epoll_event ev;
  
  ev.events = eventMask;
  ev.data.fd = fd;

  if(epoll_ctl(m_epollFd, EPOLL_CTL_ADD, fd, &ev) == -1)
    throw std::system_error(errno, std::generic_category()); 

}

void OSEpoll::delFd(int fd) {
 
  if(epoll_ctl(m_epollFd, EPOLL_CTL_DEL, fd, nullptr) == -1)
    throw std::system_error(errno, std::generic_category()); 

}

void OSEpoll::modFd(int fd, int eventMask) {
 
  struct epoll_event ev;
  
  ev.events = eventMask;
  ev.data.fd = fd;

  if(epoll_ctl(m_epollFd, EPOLL_CTL_MOD, fd, &ev) == -1)
    throw std::system_error(errno, std::generic_category()); 

}

int OSEpoll::waitForEvents(eventsVect& events, const int maxEvents) {

  carray<struct epoll_event> arr(maxEvents);

  int ready = epoll_wait(m_epollFd, arr.getPtr(), maxEvents, -1);
  if (ready == -1) {
    throw std::system_error(errno, std::generic_category()); 
  }

  arr.copyToVector(events);

  return ready;

}
