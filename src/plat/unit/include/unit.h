#ifndef UNIT_H_
#define UNIT_H_

#include "os_socket.h"
#include "os_epoll.h"
#include "utils.h"

namespace sarab {

namespace unit {

/* A base class that defines the concept of a unit. Which is  a logical entity that
 * runs inside its own thread and communicate with the main thread through messages
 * using unix sockets */
class unit {
public:
  unit(std::string parentId = "");
  virtual ~unit() {};

  /* block waiting for events to appear and update "events" accordingly
   * @param events: a vector to fill with the occured events
   * @return:       nothing */
  void waitForEvents(sarab::os::eventsVect& events);

  /* add a new fd to the list of fds to be monitored
   * @param fd:   the new fd to be added
   * @param mask: the event mask (e.g. EPOLLIN)
   * @return:     nothing */
  void addFd(int fd, int mask);

  /* delete an fd from the list of fds to be monitored
   * @param fd: the fd to be deleted
   * @return:   nothing */
  void delFd(int fd);

  /* modifiy an existing fd in the list of fds to be monitored
   * @param fd:   the fd to be modified
   * @param mask: the new event mask (e.g. EPOLLIN)
   * @return:     nothing */
  void modFd(int fd, int mask);

  /*  send a message
   * @param myMsg:    the message to be sent
   * @param destAddr: destination address
   * @return:         nothing*/
  void sendMsg(struct msg* myMsg, std::string destAddr = "");

  /*  receive a message
   * @param myMsg:    the message to be received
   * @return:         nothing */
  void recvMsg(struct msg* myMsg);

private:
  const int                 m_maxEvents  = 10;
        std::string         m_parentId   = "";
        std::string         m_parentAddr = "";
        sarab::os::OSSocket m_uSck{};
        sarab::os::OSEpoll  m_epoll{};
        sarab::utils::utils m_utils{};
};


}

}
#endif
