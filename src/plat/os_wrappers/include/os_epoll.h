#ifndef OS_EPOLL_H_
#define OS_EPOLL_H_

#include <vector>
#include <sys/epoll.h>
#include <system_error>

namespace sarab {

namespace os {

using eventsVect = std::vector<struct epoll_event>;

class OSEpoll {
public:
  OSEpoll();
  ~OSEpoll();

  OSEpoll(const OSEpoll&)            = delete;
  OSEpoll(OSEpoll&&)                 = default;
  OSEpoll& operator=(const OSEpoll&) = delete;
  OSEpoll& operator=(OSEpoll&&)      = default;

  void addFd(int fd, int eventMask);
  void delFd(int fd);
  void modFd(int fd, int eventMask);
  int  waitForEvents(eventsVect& events, const int maxEvents);

private:
  int       m_epollFd;
  const int m_epollSize = 10; /*FIXME: find a better way*/
};

}

}



#endif /* OS_EPOLL_H */
