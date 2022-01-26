/**
 * @file RI2c.c
 * @date June 7, 2021
 * @author Addi Amaya (caa746)
 */

#include <RI2c.h>
#include <string.h>
#include <hal/Drivers/I2C.h>


/***************************************************************************************************
                                             PUBLIC API
***************************************************************************************************/

/**
 * Initializes the I2C driver
 *
 * @return 0 for success, non-zero for failure. See hal/Drivers/I2C.h for details.
 */
int i2cInit(void) {

	int error = I2C_start(I2C_BUSSPEED_HZ, I2C_TRANSFERTIMEOUT);
	return error;
}


/**
 * Writes data to a slave
 *
 * @note this is a blocking call
 * @pre i2cInit must be successful
 * @param slaveAddress address of the slave where to make the transfer
 * @param data Memory location of the data to be written
 * @param size Number of bytes to be written to the I2C slave
 * @return 0 for success, non-zero for failure. See hal/Drivers/I2C.h for details.
 */
int i2cTransmit(uint16_t slaveAddress, const uint8_t* data, uint16_t size) {

	int error = I2C_write(slaveAddress, data, size);
	return error;
}


/**
 * Reads data from a slave
 *
 * @note this is a blocking call
 * @pre i2cInit must be successful
 * @param slaveAddress Address of the slave where to make the transfer.
 * @param data Memory location to store the data read from the I2C slave. Must be able to store size bytes.
 * @param size Number of bytes to be read from the I2C slave.
 * @return 0 for success, non-zero for failure. See hal/Drivers/I2C.h for details.
 */
int i2cRecieve(uint16_t slaveAddress, uint8_t* data, uint16_t size) {

	int error = I2C_read(slaveAddress, data, size);
	return error;
}


/**
 * Queues a transfer into the I2C driver
 *
 * @note this is a non-blocking call
 * @pre i2cInit must be successful
 * @param slaveAddress Address of the slave where to make the transfer.
 * @param writeSize Number of bytes to be written to the I2C slave.
 * @param readSize Number of bytes to be read from the I2C slave
 * @param writeData Memory location of the data to be written to the I2C slave
 * @param readData Memory location to store the data read from the I2C slave
 * @return 0 for success, non-zero for failure. See hal/Drivers/I2C.h for details.
 *
 */
int i2cQueue(uint16_t slaveAddress, uint16_t writeSize, uint16_t readSize, uint8_t* writeData, volatile uint8_t* readData) {

	struct _I2CgenericTransfer master;
	master.slaveAddress = slaveAddress;
	master.direction = writeRead_i2cDir;
	master.writeSize = writeSize;
	master.readSize = readSize;
	master.writeData = writeData;
	master.readData = readData;
	master.writeReadDelay = WRITEREAD_DELAY;
	uint16_t status = I2C_getCurrentTransferStatus();
	master.result = status;

	int error = I2C_queueTransfer(&master);
	return error;
}