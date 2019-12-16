#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <system_error>
#include "GPIO.h"
#include "os_file.h"

/*********************************************************************/

GPIO::GPIO(int pinNumber) {
	
  gpioPinNumber = pinNumber; 
  
	exportGPIO();
  
  // need to give Linux time to set up the sysfs structure	
  usleep(250000); // 250ms delay
  
  snprintf(gpioPathComplete, sizeof(gpioPathComplete), "%sgpio%d/", 
           GPIO_UPPER_DIR, gpioPinNumber);
  
  /* create path to all the needed files */

  snprintf(pathToDirectionFile, sizeof(pathToDirectionFile), "%sdirection", 
           gpioPathComplete);  
	
  snprintf(pathToValueFile, sizeof(pathToValueFile), "%svalue", 
           gpioPathComplete);  

	snprintf(pathToEdgeFile, sizeof(pathToEdgeFile), "%sedge", 
           gpioPathComplete);  
  

	snprintf(pathToActiveLowFile, sizeof(pathToActiveLowFile), "%sactive_low", 
           gpioPathComplete);  
  
}

/*********************************************************************/

void GPIO::exportGPIO(){
  
  char str[4] = {0};
  char filePath[strlen(GPIO_UPPER_DIR) + strlen("export") + 1] = {0};
  
  snprintf(filePath, sizeof(filePath), "%sexport", GPIO_UPPER_DIR);  
  snprintf(str, sizeof(str), "%d", gpioPinNumber);

  OSFile file(filePath, O_WRONLY);
  file.write(str, sizeof(str));

}

/*********************************************************************/

void GPIO::unexportGPIO(){
  
  char str[4] = {0};
  char filePath[strlen(GPIO_UPPER_DIR) + strlen("unexport") + 1] = {0};
  
  snprintf(filePath, sizeof(filePath), "%sunexport", GPIO_UPPER_DIR);  
  snprintf(str, sizeof(str), "%d", gpioPinNumber);

  OSFile file(filePath, O_WRONLY);
  file.write(str, sizeof(str));

}

/*********************************************************************/

GPIO_DIRECTION GPIO::getDirection() {
	
  char str[4] = {0};
  GPIO_DIRECTION res = INPUT;
  
  OSFile file(pathToDirectionFile, O_RDONLY);
  file.read(str, sizeof(str));

  if (strncmp(str, "in", 2) == 0) {
    res = INPUT;
  } else {
    res = OUTPUT;
  }

  return res;

}

/*********************************************************************/

void GPIO::setDirection(GPIO_DIRECTION dir)
{
  
  OSFile file(pathToDirectionFile, O_WRONLY);

  switch(dir){
  case INPUT: 
    file.write("in", strlen("in"));
    break;
  case OUTPUT:
    file.write("out", strlen("out"));
    break;
   }
   
}

/*********************************************************************/

void GPIO::setValue(GPIO_VALUE value) {
  
  char str[2];

  OSFile file(pathToValueFile, O_WRONLY);

  switch(value){
  case HIGH: 
    snprintf(str, sizeof(str), "%d", 1);
    file.write(str, sizeof(str));
    break;
  case LOW: 
    snprintf(str, sizeof(str), "%d", 0);
    file.write(str, sizeof(str));
    break;
  }
   
}

/*********************************************************************/

void GPIO::setEdgeType(GPIO_EDGE value) {
  
  OSFile file(pathToEdgeFile, O_WRONLY);
  
  switch(value) {
    case NONE: 
      file.write("none", strlen("none"));
      break;
    case RISING:
      file.write("rising", strlen("rising"));
      break;
   case FALLING:  
      file.write("falling", strlen("falling"));
      break;
   case BOTH:  
      file.write("both", strlen("both"));
      break;
  }

}

/*********************************************************************/

void GPIO::setActiveLow(bool isLow){

  char str[2];
  OSFile file(pathToActiveLowFile, O_WRONLY);

  if(isLow) {
    snprintf(str, sizeof(str), "%d", 1);
    file.write(str, sizeof(str));
  } else {
    snprintf(str, sizeof(str), "%d", 0);
    file.write(str, sizeof(str));
  }

}

/*********************************************************************/

void GPIO::setActiveHigh() {
   
  setActiveLow(false);

}

/*********************************************************************/

int GPIO::getActiveLow() {

  int res = 0;
	char str[2] = {0};

  OSFile file(pathToActiveLowFile, O_RDONLY);
  file.read(str, sizeof(str));
	
	res = atoi(str);

  return res;

}

/*********************************************************************/

GPIO_VALUE GPIO::getValue() {
	
  char str[2] = {0};
  GPIO_VALUE res = LOW;

  OSFile file(pathToValueFile, O_RDONLY);
  file.read(str, sizeof(str));

	if (strncmp(str, "0", 1) == 0) {
    res = LOW;
  } else { 
    res = HIGH;
  }
   
  return res;

}

/*********************************************************************/

GPIO_EDGE GPIO::getEdgeType() {

	char str[8] = {0};
  GPIO_EDGE res = NONE;

  OSFile file(pathToEdgeFile, O_RDONLY);
  file.read(str, sizeof(str));

  if (strncmp(str, "rising", 6) == 0) {
    res = RISING;
  } else if(strncmp(str, "falling", 7) == 0){
    res = FALLING;
  } else if(strncmp(str, "both", 4) == 0){
		res = BOTH;
	} else {
		res = NONE;
	}
  
  return res;
}

/*********************************************************************/

GPIO::~GPIO() {
  
  unexportGPIO();

}

/*********************************************************************/
