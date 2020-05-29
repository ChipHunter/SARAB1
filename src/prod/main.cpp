#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <system_error>
#include <syslog.h>

#include "GPIO.h"

using namespace std;

int main() {
   openlog("slog", LOG_PID|LOG_CONS, LOG_USER);
    syslog(LOG_ERR, "%s:%d Can't close the file", __FUNCTION__, __LINE__);   
    closelog();
 /*try { 
  GPIO out(17);

  out.setDirection(OUTPUT);
  out.setActiveLow(true);
  out.setValue(LOW);
  usleep(6 * 1000000);
  out.setValue(HIGH);
  usleep(6 * 1000000);
 }*/
  /*  try {
   ADXL345 sensor(1,0x53);
   sensor.setResolution(ADXL345::NORMAL);
   sensor.setRange(ADXL345::PLUSMINUS_4_G);
   sensor.displayPitchAndRoll();
   } 
*/
  /*catch(std::system_error& error) {
    std::cout << "Error: " << error.code() << " - " << error.what() << '\n'; 
  }*/

  return 0;
}
  
  


