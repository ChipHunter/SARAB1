#include "easylogging++.h"
#include "main_app.h"

// initialize easyloggingpp
INITIALIZE_EASYLOGGINGPP



int main() {

  sarab::mainapp::mainApp ma{};

  ma.init();

  ma.run();

  return 0;
}
