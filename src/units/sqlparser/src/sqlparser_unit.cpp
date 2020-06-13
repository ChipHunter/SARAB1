#include <iostream>
#include <thread>
#include <chrono>
#include <memory>

#include "sqlparser_unit.h"
#include "utils.h"
#include "easylogging++.h"

sarab::sqlp::sqlpFuncObj::sqlpFuncObj(std::thread::id id) {

  sarab::utils::utils ut{};
  m_parentId = ut.threadIdToString(id);

}

void sarab::sqlp::sqlpFuncObj::operator()() {

  sarab::os::eventsVect events;
  struct msg myMsg;
  std::unique_ptr<sarab::unit::unit> u(new sarab::sqlp::sqlpUnit{m_parentId});

  for(int i = 0; i < 3; i++) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    u->waitForEvents(events);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    u->sendMsg(&myMsg);
    std::cout << "Ahlan!" << std::endl;
  }
    std::this_thread::sleep_for(std::chrono::seconds(1));
}
