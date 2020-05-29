#include"I2CDevice.h"
#include<fcntl.h>
#include<stdio.h>
#include<iomanip>
#include<unistd.h>
#include<sys/ioctl.h>
#include<linux/i2c.h>
#include<linux/i2c-dev.h>
#include<system_error>

#define HEX(x) setw(2) << setfill('0') << hex << (int)(x)

/******************************************************************************/

I2CDevice::I2CDevice(const char* name, int device) 
: m_file(name, O_RDWR) {

	m_device = device;

  if(ioctl(m_file.getFd(), I2C_SLAVE, m_device) < 0){
    throw std::system_error(errno, std::generic_category());
  }

}

/******************************************************************************/

I2CDevice::~I2CDevice() {}

/******************************************************************************/

void I2CDevice::writeRegister(int registerAddress, char value) {

  char buffer[3];
  buffer[0] = registerAddress;
  buffer[1] = value;
  m_file.write(buffer, 3);

}

/******************************************************************************/

void I2CDevice::writeI2C(char value) {

   char buffer[2];
  buffer[0] = value;
  m_file.write(buffer, 2);

}

/******************************************************************************/

void I2CDevice::readRegister(int registerAddress, char* buf){

  writeI2C(registerAddress);
  m_file.read(buf, 1);

}

/******************************************************************************/

void I2CDevice::readRegisters(int number, int fromAddress, char* data){

	writeI2C(fromAddress);
  m_file.read(data, number);

}

/******************************************************************************/

/*
void I2CDevice::debugDumpRegisters(unsigned int number){
	cout << "Dumping Registers for Debug Purposes:" << endl;
	unsigned char *registers = readRegisters(number);
	for(int i=0; i<(int)number; i++){
		cout << HEX(*(registers+i)) << " ";
		if (i%16==15) cout << endl;
	}
	cout << dec;
}

*/
