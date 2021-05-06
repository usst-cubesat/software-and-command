/**
 * @file RUart.c
 * @date May 3, 2021
 * @author Addi Amaya (caa746) & Jacob Waskowic (jaw352)
 */


#include "RUart.h"

/***************************************************************************************************
                                             PUBLIC API
***************************************************************************************************/

//TODO: Add proper values for timeGuard and rx timeout
UARTconfig cameraConfig = {AT91C_US_USMODE_NORMAL, 57600, 1, rs232_uart, 14400};

/**
 * Sends the given data over UART
 * @param data: A pointer to the data to send over UART
 * @param size: The number of bytes to be sent
 * @return An integer error code. 0 indicates success
 */
int UART_tx(const unsigned char* data, unsigned int size) {
	return UART_write(bus0_uart, data, size);
}


/**
 * Receives data over UART and stores it in the given buffer
 * @param data: A buffer to store the received data in
 * @param size: The number of bytes to recieve over UART
 * @return An integer error code. 0 indicates success
 */
int UART_rx(unsigned char* data, unsigned int size) {
	return UART_read(bus0_uart, data, size);
}

/**
 * Initializes UART and enables receiving on the bus
 * @return An integer error code. 0 indicates success
 */
int UART_init() {
	int err = UART_start(bus0_uart, cameraConfig);
	if(err != 0) {
		return err;
	}
	return UART_setRxEnabled(bus0_uart, TRUE);
}