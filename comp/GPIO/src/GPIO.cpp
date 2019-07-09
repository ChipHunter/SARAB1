#include "GPIO.h"
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<cstdlib>
#include<cstdio>
#include<fcntl.h>
#include<unistd.h>
#include<sys/epoll.h>
#include<pthread.h>
using namespace std;


/**
 *
 * @param number The GPIO number for the RPi
 */
GPIO::GPIO(int number) {
	number = number;
	debounceTime = 0;
	togglePeriod=100;
	toggleNumber=-1; //infinite number
	callbackFunction = NULL;
	threadRunning = false;

	ostringstream s;
	s << "gpio" << number;
	name = string(s.str());
	path = GPIO_PATH + name + "/";
	exportGPIO();
	// need to give Linux time to set up the sysfs structure
	usleep(250000); // 250ms delay
}

int GPIO::write(string path, string filename, string value){
   ofstream fs;
   fs.open((path + filename).c_str());
   if (!fs.is_open()){
	   perror("GPIO: write failed to open file ");
	   return -1;
   }
   fs << value;
   fs.close();
   return 0;
}

string GPIO::read(string path, string filename){
   ifstream fs;
   fs.open((path + filename).c_str());
   if (!fs.is_open()){
	   perror("GPIO: read failed to open file ");
    }
   string input;
   getline(fs,input);
   fs.close();
   return input;
}

int GPIO::write(string path, string filename, int value){
   stringstream s;
   s << value;
   return write(path,filename,s.str());
}

int GPIO::exportGPIO(){
   return write(GPIO_PATH, "export", number);
}

int GPIO::unexportGPIO(){
   return write(GPIO_PATH, "unexport", number);
}

int GPIO::setDirection(GPIO_DIRECTION dir){
   switch(dir){
   case INPUT: return write(path, "direction", "in");
      break;
   case OUTPUT:return write(path, "direction", "out");
      break;
   }
   return -1;
}

int GPIO::setValue(GPIO_VALUE value){
   switch(value){
   case HIGH: return write(path, "value", "1");
      break;
   case LOW: return write(path, "value", "0");
      break;
   }
   return -1;
}

int GPIO::setEdgeType(GPIO_EDGE value){
   switch(value){
   case NONE: return write(path, "edge", "none");
      break;
   case RISING: return write(path, "edge", "rising");
      break;
   case FALLING: return write(path, "edge", "falling");
      break;
   case BOTH: return write(path, "edge", "both");
      break;
   }
   return -1;
}

int GPIO::setActiveLow(bool isLow){
   if(isLow) return write(path, "active_low", "1");
   else return write(path, "active_low", "0");
}

int GPIO::setActiveHigh(){
   return setActiveLow(false);
}

GPIO_VALUE GPIO::getValue(){
	string input = read(path, "value");
	if (input == "0") return LOW;
	else return HIGH;
}

GPIO_DIRECTION GPIO::getDirection(){
	string input = read(path, "direction");
	if (input == "in") return INPUT;
	else return OUTPUT;
}

GPIO_EDGE GPIO::getEdgeType(){
	string input = read(path, "edge");
	if (input == "rising") return RISING;
	else if (input == "falling") return FALLING;
	else if (input == "both") return BOTH;
	else return NONE;
}

int GPIO::streamOpen(){
	stream.open((path + "value").c_str());
	return 0;
}
int GPIO::streamWrite(GPIO_VALUE value){
	stream << value << std::flush;
	return 0;
}
int GPIO::streamClose(){
	stream.close();
	return 0;
}

int GPIO::toggleOutput(){
	setDirection(OUTPUT);
	if ((bool) getValue()) setValue(LOW);
	else setValue(HIGH);
    return 0;
}

int GPIO::toggleOutput(int time){ return toggleOutput(-1, time); }
int GPIO::toggleOutput(int numberOfTimes, int time){
	setDirection(OUTPUT);
	toggleNumber = numberOfTimes;
	togglePeriod = time;
	threadRunning = true;
    if(pthread_create(&thread, NULL, &threadedToggle, static_cast<void*>(this))){
    	perror("GPIO: Failed to create the toggle thread");
    	threadRunning = false;
    	return -1;
    }
    return 0;
}

// This thread function is a friend function of the class
void* threadedToggle(void *value){
	GPIO *gpio = static_cast<GPIO*>(value);
	bool isHigh = (bool) gpio->getValue(); //find current value
	while(gpio->threadRunning){
		if (isHigh)	gpio->setValue(HIGH);
		else gpio->setValue(LOW);
		usleep(gpio->togglePeriod * 500);
		isHigh=!isHigh;
		if(gpio->toggleNumber>0) gpio->toggleNumber--;
		if(gpio->toggleNumber==0) gpio->threadRunning=false;
	}
	return 0;
}

// Blocking Poll - based on the epoll socket code in the epoll man page
int GPIO::waitForEdge(){
	setDirection(INPUT); // must be an input pin to poll its value
	int fd, i, epollfd, count=0;
	struct epoll_event ev;
	epollfd = epoll_create(1);
    if (epollfd == -1) {
	   perror("GPIO: Failed to create epollfd");
	   return -1;
    }
    if ((fd = open((path + "value").c_str(), O_RDONLY | O_NONBLOCK)) == -1) {
       perror("GPIO: Failed to open file");
       return -1;
    }

    //ev.events = read operation | edge triggered | urgent data
    ev.events = EPOLLIN | EPOLLET | EPOLLPRI;
    ev.data.fd = fd;  // attach the file file descriptor

    //Register the file descriptor on the epoll instance, see: man epoll_ctl
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
       perror("GPIO: Failed to add control interface");
       return -1;
    }
	while(count<=1){  // ignore the first trigger
		i = epoll_wait(epollfd, &ev, 1, -1);
		if (i==-1){
			perror("GPIO: Poll Wait fail");
			count=5; // terminate loop
		}
		else {
			count++; // count the triggers up
		}
	}
    close(fd);
    if (count==5) return -1;
	return 0;
}

// This thread function is a friend function of the class
void* threadedPoll(void *value){
	GPIO *gpio = static_cast<GPIO*>(value);
	while(gpio->threadRunning){
		gpio->callbackFunction(gpio->waitForEdge());
		usleep(gpio->debounceTime * 1000);
	}
	return 0;
}

int GPIO::waitForEdge(CallbackType callback){
	threadRunning = true;
	callbackFunction = callback;
    // create the thread, pass the reference, address of the function and data
    if(pthread_create(&thread, NULL, &threadedPoll, static_cast<void*>(this))){
    	perror("GPIO: Failed to create the poll thread");
    	threadRunning = false;
    	return -1;
    }
    return 0;
}

GPIO::~GPIO() {
	unexportGPIO();
}
