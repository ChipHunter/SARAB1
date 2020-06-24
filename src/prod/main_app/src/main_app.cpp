#include "main_app.h"
#include "os_thread.h"
#include "easylogging++.h"
#include "main_unit.h"
#include "utils.h"
#include "sqlparser_unit.h"

sarab::mainapp::mainApp::mainApp() {}

void sarab::mainapp::mainApp::init() {

  // init easyloggingpp
  el::Configurations conf(df.getLogConfLocation());
  el::Loggers::reconfigureLogger("default", conf);

}

void sarab::mainapp::mainApp::run() {

  // starting threads
  sarab::sqlp::sqlpFuncObj sqlpF{std::this_thread::get_id()};
  sarab::os::guardedThread sqlpT{sqlpF};
  sarab::utils::utils ut{};

  sarab::os::eventsVect events;
  //struct msg myMsg;
  std::unique_ptr<sarab::unit::unit> u(new sarab::mainu::mainUnit{""});
  std::this_thread::sleep_for(std::chrono::seconds(1));
  LOG(ERROR) << "waiting for events in main";
  u->waitForEvents(events);
  LOG(ERROR) << "ending the program";

  /*  u->sendMsg(&myMsg, ut.computeSckAddr(ut.threadIdToString(sqlpT.getThreadId())) );

  for(int i = 0; i < 3; i++) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    u->waitForEvents(events);
    u->sendMsg(&myMsg, ut.computeSckAddr(ut.threadIdToString(sqlpT.getThreadId())) );
    std::cout << "Mar7abn!" << std::endl;
  }*/
}


