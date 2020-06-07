#include "easylogging++.h"
#include <thread>
#include <sstream>
#include <chrono>
#include "unit.h"

// initialize easyloggingpp
INITIALIZE_EASYLOGGINGPP

using namespace std;
using namespace sarab::os;
using namespace sarab::units;

void funcToRun(string id);

int main() {
  el::Configurations conf("/tmp/easyloggingpp.conf");
  el::Loggers::reconfigureLogger("default", conf);

  stringstream parentAddr;
  parentAddr << this_thread::get_id();
  thread t1{funcToRun, parentAddr.str()};

  unit u("");

  struct msg myMsg;

  stringstream childAddr;
  childAddr << t1.get_id();

  eventsVect events;

  this_thread::sleep_for(chrono::seconds(3));

  for(int i = 0; i < 3; i++) {


    u.sendMsg(&myMsg, "/tmp/sck_" + childAddr.str());

    u.waitForEvents(events);

    cout << "Mar7abn!" << endl;

    this_thread::sleep_for(chrono::seconds(1));
  }


  this_thread::sleep_for(chrono::seconds(3));

  cout << "be Tarek! :D" << endl;

  t1.join();

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

    cout << "Ahaln!" << endl;
  }
}

