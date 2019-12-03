#ifndef I2C_H_
#define I2C_H_

#define I2C_0 "/dev/i2c-0"
#define I2C_1 "/dev/i2c-1"


/**
 * @class I2CDevice
 * @brief Generic I2C Device class that can be used to connect to any
 * type of I2C device and read or write to its registers
 */
class I2CDevice{
private:
	unsigned int m_bus, m_device;
	int          m_file;

public:
  /**
  * Constructor for the I2CDevice class. It requires the bus number and device number. 
  * The constructor opens a file handle to the I2C device, which is destroyed when 
  * the destructor is called
  * @param bus The bus number.
  * @param device The device ID on the bus.
  **/
	I2CDevice(unsigned int bus, unsigned int device);

  /**
  * Closes the file on destruction, provided that it has not already been closed.
  **/
	virtual ~I2CDevice();

  /**
  * Open a connection to an I2C device
  * @return 1 on failure to open to the bus or device, 0 on success.
  */
	virtual int openI2C();

  /**
  * Close the file handles and sets a temporary state to -1.
  */
	virtual int closeI2C();

  /**
  * Write a single value to the I2C device. Used to set up the device to read from a
  * particular address.
  * @param value the value to write to the device
  * @return 1 on failure to write, 0 on success.
  */
  virtual int writeI2C(unsigned char value);

  /**
  * Write a single byte value to a single register.
  * @param registerAddress The register address
  * @param value The value to be written to the register
  * @return 1 on failure to write, 0 on success.
  */
	virtual int writeRegister(unsigned int registerAddress, unsigned char value);

  /**
  * Read a single register value from the address on the device.
  * @param registerAddress the address to read from
  * @return the byte value at the register address.
  */
	virtual int readRegister(unsigned int registerAddress, unsigned char* buf);

  /**
  * Method to read a number of registers from a single device. This is much more efficient than
  * reading the registers individually. The from address is the starting address to read from, 
  * which defaults to 0x00.
  * @param number the number of registers to read from the device
  * @param fromAddress the starting address to read from
  * @return a pointer of type unsigned char* that points to the first element in the block of 
  * registers
  */
	virtual int readRegisters(unsigned int number, unsigned int fromAddress, 
                                        unsigned char* data);

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
