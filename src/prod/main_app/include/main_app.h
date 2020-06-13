#ifndef MAIN_APP_H_
#define MAIN_APP_H_

#include "defs.h"

namespace sarab {

namespace mainapp {

class mainApp {
public:
  mainApp();
  ~mainApp()                          = default;
  mainApp(const mainApp&)             = default;
  mainApp(mainApp&&)                  = default;
  mainApp& operator=(const mainApp&)  = default;
  mainApp& operator=(mainApp&&)       = default;

  void init();
  void run();

private:
  sarab::defs::defs df{};
};

}

}
#endif
