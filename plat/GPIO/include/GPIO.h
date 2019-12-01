#ifndef GPIO_H_
#define GPIO_H_
#include <stdio.h>
#include <string.h>

#define GPIO_UPPER_DIR     "/sys/class/gpio/"
#define GPIO_MAX_NAME_SIZE 7 // e.g. gpio12
#define GPIO_ROOT_DIR_SIZE 24 //e.g. /sys/class/gpio/gpio12/ 

enum GPIO_DIRECTION { INPUT, OUTPUT               };
enum GPIO_VALUE     { LOW=0, HIGH=1               };
enum GPIO_EDGE      { NONE, RISING, FALLING, BOTH };

class GPIO {
private:
	int  gpioPinNumber;

	char gpioPathComplete   [ GPIO_ROOT_DIR_SIZE                        ];
  char pathToDirectionFile[ GPIO_ROOT_DIR_SIZE + strlen("direction" ) ];
  char pathToValueFile    [ GPIO_ROOT_DIR_SIZE + strlen("value"     ) ];
  char pathToEdgeFile     [ GPIO_ROOT_DIR_SIZE + strlen("edge"      ) ];
  char pathToActiveLowFile[ GPIO_ROOT_DIR_SIZE + strlen("active_low") ];
  
  int dirFileFd, edgeFileFd, valueFileFd, activeLowFileFd;
public:
	GPIO(int);                     
  ~GPIO();  // destructor unexports the pin

  // getters/setters
  int            getGpioPinNumber() const { return gpioPinNumber; }
  const char*    getGpioPath()      const { return gpioPathComplete;}
  void           setDirection(GPIO_DIRECTION);
	GPIO_DIRECTION getDirection();
	void           setValue(GPIO_VALUE);
	GPIO_VALUE     getValue();
	void           setActiveLow(bool isLow=true);  // low=1, high=0
	void           setActiveHigh();                // default state
	int            getActiveLow();                
  void           setEdgeType(GPIO_EDGE); 
  GPIO_EDGE      getEdgeType();

private:
  void writeGPIO(int fd, const char* value);
	void writeGPIO(int fd, int value);
	void readGPIO(int fd, char str[], int len);
  void exportGPIO();
	void unexportGPIO();
};

#endif /* GPIO_H_ */
