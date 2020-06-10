#include "easylogging++.h"
#include <thread>
#include <sstream>
#include <chrono>
#include "unit.h"
#include "os_thread.h"
#include "sqlparser_unit.h"
#include <memory>
// initialize easyloggingpp

INITIALIZE_EASYLOGGINGPP

using namespace std;
using namespace sarab::os;
using namespace sarab::unit;
using namespace sarab::utils;
using namespace sarab::thread;
using namespace sarab::sqlp;


int main() {
  el::Configurations conf("/tmp/easyloggingpp.conf");
  el::Loggers::reconfigureLogger("default", conf);


  sqlpFuncObj sqlpFunc{this_thread::get_id()};
  guardedThread t{sqlpFunc};
  unit u{""};

  struct msg myMsg;
  utils ut{};
  string childAddr = ut.computeSckAddr(t.getThreadId());
  eventsVect events;

  this_thread::sleep_for(chrono::seconds(3));

  for(int i = 0; i < 3; i++) {


    u.sendMsg(&myMsg, childAddr);

    u.waitForEvents(events);

    cout << "Ahlan!" << endl;

    this_thread::sleep_for(chrono::seconds(1));
  }


  this_thread::sleep_for(chrono::seconds(3));

  cout << "be Tarek! :D" << endl;

  return 0;
}
