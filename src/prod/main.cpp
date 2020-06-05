#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main() {
  el::Configurations conf("/tmp/easyloggingpp.conf");
  el::Loggers::reconfigureLogger("default", conf);

  int var = 4;
  LOG(ERROR) << "My first info log using default logger " << ", with the var " << var;

  return 0;
}
  
  


