#ifndef I2C_H_
#define I2C_H_

#include "os_file.h"

/**
 * @class I2CDevice
 * @brief Generic I2C Device class that can be used to connect to any
 * type of I2C device and read or write to its registers
 */
class I2CDevice{
private:
	int                     m_device;
  sarab::os::OSFile       m_file;

public:
  /**
  * Constructor for the I2CDevice class. It requires the bus's name and device address.  
  * @param i2c bus name.
  * @param device The device ID on the bus.
  **/
	I2CDevice(const char*, int device);

	virtual ~I2CDevice();

  /**
  * Write a single value to the I2C device. Used to set up the device to read from a
  * particular address.
  * @param value the value to write to the device
  * @return nothing 
  */
  virtual void writeI2C(char value);

  /**
  * Write a single byte value to a single register.
  * @param registerAddress The register address
  * @param value The value to be written to the register
  * @return nothing 
  */
	virtual void writeRegister(int registerAddress, char value);

  /**
  * Read a single register value from the address on the device.
  * @param registerAddress the address to read from
  * @return nothing 
  */
	virtual void readRegister(int registerAddress, char* buf);

  /**
  * Method to read a number of registers from a single device. This is much more efficient than
  * reading the registers individually. The from address is the starting address to read from, 
  * which defaults to 0x00.
  * @param number the number of registers to read from the device
  * @param fromAddress the starting address to read from
  * @param data a buffer to read to 
  * @return nothing 
  */
	virtual void readRegisters(int number, int fromAddress, char* data);

  /**
  * Method to dump the registers to the standard output. It inserts a return character after every
  * 16 values and displays the results in hexadecimal to give a standard output using the HEX() 
  * macro that is defined at the top of this file. The standard output will stay in hexadecimal 
  * format, hence the call on the last like.
  * @param number the total number of registers to dump, defaults to 0xff
  */
	//virtual void           debugDumpRegisters(unsigned int number = 0xff);
};

#endif /* I2C_H_ */
