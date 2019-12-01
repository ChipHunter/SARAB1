#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#include "GPIO.h"

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

void GPIO::writeGPIO(int fd, const char* value)
{
  
  if (write(fd, value, strlen(value)) != strlen(value)) {
    printf("error writing to the file: %s \n", strerror(errno));
  }

}

/*********************************************************************/

void GPIO::writeGPIO(int fd, int value){
  
  char str[4] = {0};

  snprintf(str, sizeof(str), "%d", value);

  if (write(fd, str, sizeof(str)) != sizeof(str)) {
    printf("error writing to the file: %s \n", strerror(errno));
  }

}

/*********************************************************************/

void GPIO::readGPIO(int fd, char str[], int len){

  int ret = read(fd, str, len);  
  if (ret < 0) {
    printf("error reading from the file: %s \n", strerror(errno));
  }

}
/*********************************************************************/

void GPIO::exportGPIO(){
  
  char filePath[strlen(GPIO_UPPER_DIR) + strlen("export") + 1] = {0};
  
  snprintf(filePath, sizeof(filePath), "%sexport", GPIO_UPPER_DIR);  
 
  int fd = open(filePath, O_WRONLY, O_TRUNC);
  if (fd == -1) {
    printf("error opening the file: %s\n", strerror(errno));
  } else {
    writeGPIO(fd, gpioPinNumber);
  }

  if (close(fd) == -1) {
    printf("error closing the file: %s \n", strerror(errno));
  }

}

/*********************************************************************/

void GPIO::unexportGPIO(){
  
  char filePath[strlen(GPIO_UPPER_DIR) + strlen("unexport") + 1] = {0};
  
  snprintf(filePath, sizeof(filePath), "%sunexport", GPIO_UPPER_DIR);  

  int fd = open(filePath, O_WRONLY, O_TRUNC);
  if (fd == -1) {
    printf("error opening the file: %s\n", strerror(errno));
  } else {
    writeGPIO(fd, gpioPinNumber);
  }

  if (close(fd) == -1) {
    printf("error closing the file: %s \n", strerror(errno));
  }
}

/*********************************************************************/

GPIO_DIRECTION GPIO::getDirection() {
	
  char str[4] = {0};
  GPIO_DIRECTION res = INPUT;
  
  int fd = open(pathToDirectionFile, O_RDONLY);
  if (fd == -1) {
    printf("error opening the file: %s \n", strerror(errno));
  }

  readGPIO(fd, str, 4);
  
  if (strncmp(str, "in", 2) == 0) {
    res = INPUT;
  } else {
    res = OUTPUT;
  }

  if (close(fd) == -1) {
    printf("error closing the file: %s \n", strerror(errno));
  } 

  return res;

}

/*********************************************************************/

void GPIO::setDirection(GPIO_DIRECTION dir)
{
  
  int fd = open(pathToDirectionFile, O_WRONLY);
  if (fd == -1) {
    printf("error opening the file: %s \n", strerror(errno));
  }

  switch(dir){
  case INPUT: 
    writeGPIO(fd, "in");
    break;
  case OUTPUT:
    writeGPIO(fd, "out");
    break;
   }
   
 if (close(fd) == -1) {
  printf("error closing the file: %s \n", strerror(errno));
 } 

}

/*********************************************************************/

void GPIO::setValue(GPIO_VALUE value) {
  
  int fd = open(pathToValueFile, O_WRONLY);
  if (fd == -1) {
    printf("error opening the file: %s \n", strerror(errno));
  }

  switch(value){
  case HIGH: 
    writeGPIO(fd, 1);
    break;
  case LOW: 
    writeGPIO(fd, 0);
    break;
  }
   
  if (close(fd) == -1) {
    printf("error closing the file: %s \n", strerror(errno));
  } 

}

/*********************************************************************/

void GPIO::setEdgeType(GPIO_EDGE value) {
  
  int fd = open(pathToEdgeFile, O_WRONLY);
  if (fd == -1) {
    printf("error opening the file: %s \n", strerror(errno));
  }

  switch(value) {
    case NONE: 
      writeGPIO(fd, "none");
      break;
    case RISING:
      writeGPIO(fd, "rising");
      break;
   case FALLING:  
      writeGPIO(fd, "falling");
      break;
   case BOTH:  
      writeGPIO(fd, "both");
      break;
  }

  if (close(fd) == -1) {
    printf("error closing the file: %s \n", strerror(errno));
  } 

}

/*********************************************************************/

void GPIO::setActiveLow(bool isLow){

  int fd = open(pathToActiveLowFile, O_WRONLY);
  if (fd == -1) {
    printf("error opening the file: %s \n", strerror(errno));
  }

  if(isLow) {
    writeGPIO(fd, 1);
  } else {
    writeGPIO(fd, 0);
  }

  if (close(fd) == -1) {
    printf("error closing the file: %s \n", strerror(errno));
  } 

}

/*********************************************************************/

void GPIO::setActiveHigh() {
   
  setActiveLow(false);

}

/*********************************************************************/

int GPIO::getActiveLow() {

  int fd = open(pathToActiveLowFile, O_RDONLY);
  if (fd == -1) {
    printf("error opening the file: %s \n", strerror(errno));
  }

  int res = 0;
	int len = 2;
	char str[len] = {0};

  readGPIO(fd, str, len);
	
	res = atoi(str);

  if (close(fd) == -1) {
    printf("error closing the file: %s \n", strerror(errno));
  } 

  return res;

}

/*********************************************************************/

GPIO_VALUE GPIO::getValue() {
	
  int fd = open(pathToValueFile, O_RDONLY);
  if (fd == -1) {
    printf("error opening the file: %s \n", strerror(errno));
  }
  char str[2] = {0};
  GPIO_VALUE res = LOW;

  readGPIO(fd, str, 2);

	if (strncmp(str, "0", 1) == 0) {
    res = LOW;
  } else { 
    res = HIGH;
  }
   
  if (close(fd) == -1) {
    printf("error closing the file: %s \n", strerror(errno));
  } 

  return res;

}

/*********************************************************************/

GPIO_EDGE GPIO::getEdgeType() {

	char str[8] = {0};
  GPIO_EDGE res = NONE;

  int fd = open(pathToEdgeFile, O_RDONLY);
  if (fd == -1) {
    printf("error opening the file: %s \n", strerror(errno));
  }
  readGPIO(fd, str, 8);

  if (strncmp(str, "rising", 6) == 0) {
    res = RISING;
  } else if(strncmp(str, "falling", 7) == 0){
    res = FALLING;
  } else if(strncmp(str, "both", 4) == 0){
		res = BOTH;
	} else {
		res = NONE;
	}
  
  if (close(fd) == -1) {
    printf("error closing the file: %s \n", strerror(errno));
  } 
	
  return res;
}

/*********************************************************************/

GPIO::~GPIO() {
  
  unexportGPIO();

}

/*********************************************************************/
