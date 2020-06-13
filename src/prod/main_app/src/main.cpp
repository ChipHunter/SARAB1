#include "easylogging++.h"
#include <thread>
#include <sstream>
#include <chrono>
#include "unit.h"
#include "sqlparser_unit.h"
#include "main_unit.h"
#include "main_app.h"

// initialize easyloggingpp
INITIALIZE_EASYLOGGINGPP



int main() {

  sarab::mainapp::mainApp ma{};

  ma.init();

  ma.run();

  return 0;
}
