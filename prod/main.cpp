#include "GPIO.h"
#include <unistd.h>

int main(){
  GPIO gpio27(27);


  gpio27.setActiveLow(true);
  printf("gpio27 edge type is %d \n", gpio27.getActiveLow());


  gpio27.setActiveHigh();
  printf("gpio27 edge type is %d \n", gpio27.getActiveLow());

  printf("done \n");
  return 0;
}
