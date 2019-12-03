#include"I2CDevice.h"
#include<fcntl.h>
#include<stdio.h>
#include<iomanip>
#include<unistd.h>
#include<sys/ioctl.h>
#include<linux/i2c.h>
#include<linux/i2c-dev.h>

#define HEX(x) setw(2) << setfill('0') << hex << (int)(x)

/******************************************************************************/

I2CDevice::I2CDevice(unsigned int bus, unsigned int device) {

	m_file = -1;
	m_bus = bus;
	m_device = device;
	openI2C();

}

/******************************************************************************/

I2CDevice::~I2CDevice() {
	
  if(m_file != -1) 
    closeI2C();

}

/******************************************************************************/

int I2CDevice::openI2C(){

  int ret = 0;
  char name[16];
  
  if(m_bus == 0) 
    snprintf(name, sizeof(name) ,I2C_0);
  else 
    snprintf(name, sizeof(name), I2C_1);

  if((m_file = open(name, O_RDWR)) == -1){
    perror("I2C: failed to open the bus\n");
    ret=  1;
  }

  if(ioctl(m_file, I2C_SLAVE, m_device) < 0){
    perror("I2C: Failed to connect to the device\n");
    ret = 1;
  }

  return ret;

}

/******************************************************************************/

int I2CDevice::closeI2C(){

  int ret = 0;
	if (close(m_file) == -1) {
    perror("I2C: Failed to close the file \n");
    ret = 1;
  }
	m_file = -1;

  return ret;
}

/******************************************************************************/

int I2CDevice::writeRegister(unsigned int registerAddress, unsigned char value){

  int ret = 0;
  unsigned char buffer[2];
  buffer[0] = registerAddress;
  buffer[1] = value;
  if(write(m_file, buffer, 2) != 2){
    perror("I2C: Failed write to the device\n");
    ret = 1;
  }

  return ret;

}

/******************************************************************************/

int I2CDevice::writeI2C(unsigned char value){

  int ret = 0;
  unsigned char buffer[1];
  buffer[0]=value;
  if (write(m_file, buffer, 1) != 1){
    perror("I2C: Failed to write to the device\n");
    ret = 1;
  }

  return ret;

}

/******************************************************************************/

int I2CDevice::readRegister(unsigned int registerAddress, unsigned char* buf){

  int ret = 0;
  writeI2C(registerAddress);
  if(read(m_file, buf, 1) != 1){
    perror("I2C: Failed to read in the value.\n");
    ret = 1;
  }

  return ret;

}

/******************************************************************************/

int I2CDevice::readRegisters(unsigned int number, unsigned int fromAddress,
                             unsigned char* data){

  int ret = 0;
	writeI2C(fromAddress);
  if(read(m_file, data, number) != (int)number){
     perror("IC2: Failed to read in the full buffer.\n");
   ret = 1;
  }

	return  ret;

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
