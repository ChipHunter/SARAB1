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

void funcToRun(string id);

void func(thread::id id) {

  utils ut{};
  cout << "from thread! " << ut.computeSckAddr(id) << endl;

}

int main() {
  el::Configurations conf("/tmp/easyloggingpp.conf");
  el::Loggers::reconfigureLogger("default", conf);


//  funcObj* func = new sqlparserFunc(this_thread::get_id());
  sarab::sqlp::funcObj func1{this_thread::get_id()};
  guardedThread t1{func1};
//  stringstream parentAddr;
//  parentAddr << this_thread::get_id();
//  thread t1{funcToRun, parentAddr.str()};
/*
  unit u("");

  struct msg myMsg;

  stringstream childAddr;
  childAddr << t1.get_id();

  eventsVect events;

  this_thread::sleep_for(chrono::seconds(3));

  for(int i = 0; i < 3; i++) {


    u.sendMsg(&myMsg, "/tmp/sck_" + childAddr.str());

    u.waitForEvents(events);

    cout << "Ahlan!" << endl;

    this_thread::sleep_for(chrono::seconds(1));
  }


  this_thread::sleep_for(chrono::seconds(3));

  cout << "be Tarek! :D" << endl;

  t1.join();
*/
  return 0;
}



void funcToRun(string id) {

  eventsVect events;
  struct msg myMsg;

  unit u(id);

  for(int i = 0; i < 3; i++) {
    this_thread::sleep_for(chrono::seconds(1));

    u.waitForEvents(events);

    this_thread::sleep_for(chrono::seconds(1));

    u.sendMsg(&myMsg);

    cout << "Mar7abn!" << endl;
  }
}

