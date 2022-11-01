/**
 * @file RCommunicationTasks.c
 * @date January 30, 2022
 * @author Tyrel Kostyk (tck290) and Matthew Buglass (mab839)
 */

#include <RCommunicationTasks.h>
#include <RTransceiver.h>
#include <RProtocolService.h>
#include <RTelecommandService.h>
#include <RFileTransferService.h>
#include <RCommunicationStateMachine.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>

#include <string.h>

#include <RFileTransferService.h>
#include <RCommon.h>
#include <RTransceiver.h>
#include <RMessage.h>
#include <string.h>
#include <RCommon.h>



/***************************************************************************************************
                                   DEFINITIONS & PRIVATE GLOBALS
***************************************************************************************************/



/** Communication Receive Task delay (in ms). */
#define COMMUNICATION_RX_TASK_DELAY_MS			1000//((portTickType)1)

/** Communication Transmit Task delay (in ms) during typical operation. */
#define COMMUNICATION_TX_TASK_SHORT_DELAY_MS	100//((portTickType)1)

/**
 * Communication Transmit Task delay (in ms) when the transmitter's buffer is full.
 *
 * Transmission speed: 9600 bps => roughly 1 byte per ms; This delay should long enough to transmit
 * one full frame.
 */
#define COMMUNICATION_TX_TASK_LONG_DELAY_MS		1000/((portTickType)TRANCEIVER_TX_MAX_FRAME_SIZE)


///** Abstraction of the response states */
//typedef enum _response_state_t {
//	responseStateIdle	= 0,	///> Awaiting response from Ground Station
//	responseStateReady	= 1,	///> Ready to transmit to Ground Station
//} response_state_t;
//
//
///** Abstraction of the ACK/NACK return types */
//typedef enum _response_t {
//	responseAck		= protocol_message_Ack_tag,	///> Acknowledge (the message was received properly)
//	responseNack	= protocol_message_Nack_tag,	///> Negative Acknowledge (the message was NOT received properly)
//} response_t;
//
//
///** Abstraction of the communication modes */
//typedef enum _comm_mode_t {
//	commModeQuiet			= -1,	///> Prevent downlink transmissions and automatic state changes
//	commModeIdle			= 0,	///> Not in a pass
//	commModeTelecommand		= 1,	///> Receiving Telecommands from Ground Station
//	commModeFileTransfer	= 2,	///> Transmitting data to the Ground Station
//} comm_mode_t;
//
//
///** Co-ordinates tasks during the telecommand phase */
//typedef struct _telecommand_state_t {
//	response_state_t transmitReady;	///> Whether the Satellite is ready to transmit a response (ACK, NACK, etc.)
//	response_t responseToSend;		///> What response to send, when ready
//} telecommand_state_t;
//
//
///** Co-ordinates tasks during the file transfer phase */
//typedef struct _file_transfer_state_t {
//	response_state_t transmitReady;		///> Whether the Satellite is ready to transmit another Frame (telemetry, etc.)
//	response_t responseReceived;		///> What response was received (ACK, NACK, etc.) regarding the previous message
//	uint8_t transmissionErrors;			///> Error counter for recording consecutive NACKs
//} file_transfer_state_t;
//
//
///** Wrapper structure for communications co-ordination */
//typedef struct _communication_state_t {
//	comm_mode_t mode;					///> The current state of the Communications Tasks
//	telecommand_state_t telecommand;	///> The state during the Telecommand mode
//	file_transfer_state_t fileTransfer;	///> The state during the File Transfer mode
//} communication_state_t;


///** Timer for pass mode */
//static xTimerHandle passTimer;
//
///** Timer for quiet mode */
//static xTimerHandle quietTimer;
//
///** Communication co-orditation structure */
//static communication_state_t state = { 0 };


/***************************************************************************************************
                                       PRIVATE FUNCTION STUBS
***************************************************************************************************/





/***************************************************************************************************
                                           FREERTOS TASKS
***************************************************************************************************/

/**
 * Receive and process frames received by the Transceiver's receiver buffer (when available).
 *
 * Constantly checks for the existence of received frames within the Transceiver; if they exist,
 * this task will retrieve and process them. If they are telecommands, they are immediately sent off
 * to another module for further processing. If the received message is an ACK/NACK, this task
 * updates the proper flags (local and private to this module) to continue communication operations
 * as guided by our protocol (single ACK-NACK responses per message).
 *
 * @note	This is a high priority task, and must never be disabled for extented periods of time.
 * @note	When an operational error occurs (e.g. a call to the transceiver module failed), this
 * 			Task will simply ignore the operation and try again next time. Lower level modules
 * 			(e.g. the Transceiver) are responsible for reporting those errors to the system.
 * @param	parameters Unused.
 */
void CommunicationRxTask(void* parameters) {

	// ignore the input parameter
	(void)parameters;

	int error = 0;				// error detection
	uint16_t rxFrameCount = 0;	// number of frames currently in the receiver's buffer
	uint16_t rxMessageSize = 0;	// size (in bytes) of a received frame
	uint8_t rxMessage[TRANCEIVER_RX_MAX_FRAME_SIZE] = { 0 };	// input buffer for received frames

	uint8_t messageTag = 0;
	int z = -1;
	uint16_t sizeToRead[3] = {15, 13, 13};


	while (1) {
		if(z < 2){
			z++;
		}
		// get the number of frames currently in the receive buffer
		rxFrameCount = 1;//0;
		error = 0;
		//error = transceiverRxFrameCount(&rxFrameCount);
		debugPrint("Here: \n");
		// obtain frames when present
		if (rxFrameCount > 0 && error == 0) {

			// obtain new frame from the transceiver
			rxMessageSize = 0;
			memset(rxMessage, 0, sizeof(rxMessage));
			error = transceiverGetFrame(rxMessage, &rxMessageSize);
			rxMessageSize = sizeToRead[z];
			debugPrint("Message size: %d \n", rxMessageSize);
			debugPrint("Message: ");
			for (int j = 0; j < rxMessageSize; j++){
				debugPrint("%x",rxMessage[j]);
			}
			debugPrint("\n");

			// attempt to extract a protocol messages

			messageTag = telecommandHandle(rxMessage, rxMessageSize);
			debugPrint("messageTag for telecommandHandle: %d\n", messageTag);

			// messageTag = protocolHandle(rxMessage, rxMessageSize);
			// debugPrint("messageTag for protocolHandle: %d\n", messageTag);
			// if a protocol message
			if (messageTag != 0){

				// handle protocol
				switch (messageTag) {

					// ack
					case (protocol_message_Ack_tag):
						ackReceived();
						break;

					// nack
					case (protocol_message_Nack_tag):
						nackReceived();
						break;

					default:
						nackReceived();
						break;
				}
			}
			/*

			// otherwise, attempt to extract and execute a telecommand message
			else{

				messageTag = telecommandHandle(rxMessage, rxMessageSize);
				debugPrint("messageTag for telecommandHandle: %d\n", messageTag);
				// if a valid message
				if (messageTag != 0){

					// handle protocol
					switch (messageTag) {

						// beginPass
						case (telecommand_message_BeginPass_tag):
							beginPass();
							break;

						// beginFileTransfer
						case (telecommand_message_BeginFileTransfer_tag):
							beginFileTransfer();
							break;

						// ceaseTransmission
						case (telecommand_message_CeaseTransmission_tag):
							ceaseTransmission();
							break;

						// resumeTransmission
						case (telecommand_message_ResumeTransmission_tag):
							resumeTransmission();
							break;

						// updateTime
						case (telecommand_message_UpdateTime_tag):
							updateTime();
							break;

						// reset
						case (telecommand_message_Reset_tag):
							resetSat();
							break;

						default:
							// todo: should the default be to send a nack?
							sendNack();
							break;
					}
				}
				// if no telecommand message successfully received, send a nack
				else{
					sendNack();
				}
			}*/
		}
		vTaskDelay(500);//COMMUNICATION_RX_TASK_DELAY_MS);
	}
}





/**
 * Transmits outgoing frames to the Transceiver's transmitter buffer.
 *
 * When NOT in a pass mode, this task does nothing. When in a pass and in the telecommand mode
 * (i.e. receiving telecommands), this task is responsible for transmitting the appropriate ACKs
 * (or NACKs) and updating the flags (that are local and private to this module). When in a pass
 * and in the File Transfer mode, this task is responsible for transmitting frames that are ready
 * for downlink transmission. Preparation of downlink messages is done by another module prior to
 * the pass duration.
 *
 * @note	This is a high priority task.
 * @note	When an operational error occurs (e.g. a call to the transceiver module failed), this
 * 			Task will simply ignore the operation and try again next time. Lower level modules
 * 			(e.g. the Transceiver) are responsible for reporting these errors to the system.
 * @param	parameters Unused.
 */
void CommunicationTxTask(void* parameters) {

	// ignore the input parameter
	(void)parameters;

	int error = 0;												// error detection
	uint8_t txSlotsRemaining = TRANCEIVER_TX_MAX_FRAME_COUNT;	// number of open frame slots in the transmitter's buffer
	uint8_t txMessageSize = 0;									// size (in bytes) of an outgoing frame
	uint8_t txMessage[TRANCEIVER_TX_MAX_FRAME_SIZE] = { 0 };	// output buffer for messages to be transmitted

	while (1) {
		txMessageSize = getNextFrame(txMessage);
		debugPrint("txMessageSize = %d\n", txMessageSize);
		// send the message
		if (txMessageSize > 0)
			error = transceiverSendFrame(txMessage, txMessageSize, &txSlotsRemaining);

		// increase Task delay time when the Transmitter's buffer is full to give it time to transmit
		if (txSlotsRemaining > 0)
			vTaskDelay(COMMUNICATION_TX_TASK_SHORT_DELAY_MS);
		else
			vTaskDelay(COMMUNICATION_TX_TASK_LONG_DELAY_MS);
	}
}



