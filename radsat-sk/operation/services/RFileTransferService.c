/**
 * @file RFileTransferService.c
 * @date February 25, 2022
 * @author Tyrel Kostyk (tck290)
 */

#include <RFileTransferService.h>
#include <RCommon.h>
#include <RTransceiver.h>
#include <RMessage.h>
#include <string.h>
#include <RCommon.h>
#include <RFram.h>


/**
 * Ensure that our message sizes never exceed the transceiver's max frame size.
 * NOTE: This check allows for 15 bytes of overhead (i.e. from the header) which is more than we currently need. */
#if ((PROTO_MAX_ENCODED_SIZE + 15) > (TRANCEIVER_TX_MAX_FRAME_SIZE))
#error "Encoded protobuf message size (plus header) exceeds maximum transceiver frame size!! Reduce size of header or max protobuf messages"
#endif


/***************************************************************************************************
                                   DEFINITIONS & PRIVATE GLOBALS
***************************************************************************************************/

/** The max number of frames locally stored at one time. */
#define MAX_FRAME_COUNT 		(200)

/** Error code for internal issues regarding frame cursors (wrap-around, etc.). */
#define ERROR_CURSOR			(-1)

/** Error code for failed message wrapping. */
#define ERROR_MESSAGE_WRAPPING	(-2)

/** Struct that defines a prepared FRAM frame and its total size. */
typedef struct _fram_frame_t {
	uint8_t size;								///> The size (in bytes) of the entire frame
	uint8_t data[TRANCEIVER_TX_MAX_FRAME_SIZE];	///> The buffer holding the prepared frame
} fram_frame_t;

/** Local frame for FRAM operations. */
static fram_frame_t fram_frame = { 0 };

/** Cursor for writing to the frame FIFO (starts ahead of the read cursor). */
static uint8_t frameWriteCursor = 1;

/** Cursor for reading from the frame FIFO. */
static uint8_t frameReadCursor = 0;


/***************************************************************************************************
                                             PUBLIC API
***************************************************************************************************/

/**
 * Initialize the write/read cursors with the last values in FRAM.
 * Requires that FRAM has already been initialized.
 * @return 0 for success, non-zero for failure.
 */
int fileTransferInit(void) {
	uint16_t framCursors[2] = {0};

	// Read the last stored cursor values
	int error = framRead(&framCursors, FRAM_WRITE_CURSOR_ADDR, 4);

	if (error == SUCCESS) {
		frameWriteCursor = 0; //framCursors[0];
		frameReadCursor = 199; //framCursors[1];

		if (frameReadCursor >= MAX_FRAME_COUNT)
			frameReadCursor = 0;
		if (frameWriteCursor >= MAX_FRAME_COUNT)
			frameWriteCursor = frameReadCursor + 1;
	}

	return error;
}

/**
 * Increment the internal FIFO and provide the frame at that location.
 *
 * Invalidates (deletes) the previous frame before moving on.
 *
 * @param frame Pointer to a buffer that the frame will be placed into. Set by function.
 * @return The size of the frame placed into the buffer; 0 on error.
 */
uint8_t fileTransferNextFrame(uint8_t* frame) {

	// ensure that there is a valid frame ahead
	uint8_t nextFrameReadCursor = frameReadCursor + 1;
	if (nextFrameReadCursor == MAX_FRAME_COUNT)
		nextFrameReadCursor = 0;

	void* framDataAddr = FRAM_DATA_START_ADDR + (nextFrameReadCursor * FRAM_DATA_FRAME_SIZE);
	printf("\n\r Reading %i bytes of data in FRAM at cursor/address: %i/%x \n\r", FRAM_DATA_FRAME_SIZE, nextFrameReadCursor, framDataAddr);
	framRead(&fram_frame, framDataAddr, FRAM_DATA_FRAME_SIZE);
	printf("%i, ", fram_frame.size);
	for (uint8_t i=0; i<FRAM_DATA_FRAME_SIZE-1; i++) {
		printf("%i, ", fram_frame.data[i]);
	}
	printf(" --- Read finished --- \n\r");

	if (fram_frame.size == 0)
		return SUCCESS;

	// invalidate the current (now previous frame)
	fram_frame_t emptyFrame = {0};
	framDataAddr = FRAM_DATA_START_ADDR + (frameReadCursor * FRAM_DATA_FRAME_SIZE);
	framWrite(&emptyFrame, framDataAddr, FRAM_DATA_FRAME_SIZE);

	// increment the read cursor
	frameReadCursor++;
	if (frameReadCursor == MAX_FRAME_COUNT)
		frameReadCursor = 0;
	// save read cursor value in FRAM
	framWrite(&frameReadCursor, FRAM_READ_CURSOR_ADDR, 2);

	// transfer the new (now current) frame into the provided buffer
	memcpy(frame, fram_frame.data, fram_frame.size);

	return fram_frame.size;
}


/**
 * Provide the current frame.
 *
 * @param frame Pointer to a buffer that the frame will be placed into. Set by function.
 * @return The size of the frame placed into the buffer; 0 on error.
 */
uint8_t fileTransferCurrentFrame(uint8_t* frame) {

	// read current frame from FRAM
	void* framDataAddr = FRAM_DATA_START_ADDR + (frameReadCursor * FRAM_DATA_FRAME_SIZE);
	printf("\n\r Reading %i bytes of data in FRAM at cursor/address: %i/%x \n\r", FRAM_DATA_FRAME_SIZE, frameReadCursor, framDataAddr);
	framRead(&fram_frame, framDataAddr, FRAM_DATA_FRAME_SIZE);
	printf("%i, ", fram_frame.size);
	for (uint8_t i=0; i<FRAM_DATA_FRAME_SIZE-1; i++) {
		printf("%i, ", fram_frame.data[i]);
	}
	printf("\n\r --- Read finished --- \n\r");

	// only provide a frame if the cursor is pointing at a valid frame
	if (fram_frame.size > 0)
		memcpy(frame, fram_frame.data, fram_frame.size);

	// return the size of the frame
	return fram_frame.size;
}


/**
 * Prepare a message for downlink and add it to the internal FIFO.
 *
 * @param message Pointer to the raw Protobuf message to be prepared.
 * @param size The size (in bytes) of the message.
 * @param messageTag The Protobuf tag of the message.
 * @return 0 on success, -1 on internal cursor error, -2 on message wrapping error, otherwise see hal/errors.h.
 */
int fileTransferAddMessage(const void* message, uint8_t size, uint16_t messageTag) {

	// ensure input pointer is valid
	if (message == 0) {
		return E_INPUT_POINTER_NULL;
	}

	// ensure size parameter is valid
	if (size == 0 || size > (uint8_t)PROTO_MAX_ENCODED_SIZE)
		return E_PARAM_OUTOFBOUNDS;

	// ensure we are not about to overwrite frames that have not been read
	if (frameWriteCursor == frameReadCursor)
		return ERROR_CURSOR;

	// create new RADSAT-SK message to populate
	radsat_message newMessage = { 0 };
	newMessage.which_service = radsat_message_FileTransferMessage_tag;
	newMessage.FileTransferMessage.which_message = messageTag;

	// internal message data will go immediately after the "which message" property of the struct
	// TODO: confirm that this works
	void* newMessageAddr = &(newMessage.FileTransferMessage.which_message) + sizeof(newMessage.FileTransferMessage.which_message);
	memcpy(newMessageAddr, message, size);

	// wrap new message
	fram_frame.size = messageWrap(&newMessage, fram_frame.data);

	printf("\n\r new message size: %i \n\r", fram_frame.size);
	printf("\n\r --- Cursor values --- \n\r");
	uint16_t framCursors[2] = {0};
	framRead(&framCursors, FRAM_WRITE_CURSOR_ADDR, 4);
	printf(" frameWriteCursor: %i \n\r", frameWriteCursor);
	printf(" frameReadCursor: %i \n\r", frameReadCursor);
	printf(" %x (FRAM WRITE):  %i \n\r", FRAM_WRITE_CURSOR_ADDR, framCursors[0]);
	printf(" %x (FRAM READ):   %i \n\r", FRAM_READ_CURSOR_ADDR, framCursors[1]);

	// increment the FRAM address for the new frame
	void* framDataAddr = FRAM_DATA_START_ADDR + (frameWriteCursor * FRAM_DATA_FRAME_SIZE);
	// write data in FRAM
	printf("\n\r Writing %i bytes of data in FRAM at cursor/address: %i/%x \n\r", FRAM_DATA_FRAME_SIZE, frameWriteCursor, framDataAddr);
	framWrite(&fram_frame, framDataAddr, FRAM_DATA_FRAME_SIZE);
	printf(" --- Write finished --- \n\r");
	printf(" First 5 bytes written: ");
	for (uint8_t i=0; i<5; i++) {
		printf("%i, ", fram_frame.data[i]);
	}

	printf("\n\r Reading %i bytes of data in FRAM at cursor/address: %i/%x \n\r", FRAM_DATA_FRAME_SIZE, frameWriteCursor, framDataAddr);
	fram_frame_t framData = {0};
	framRead(&framData, framDataAddr, FRAM_DATA_FRAME_SIZE);
	printf("%i, ", framData.size);
	for (uint8_t i=0; i<FRAM_DATA_FRAME_SIZE-1; i++) {
		printf("%i, ", framData.data[i]);
	}
	printf("\n\r --- Read finished --- \n\r");

	// return error if message wrapping failed
	if (fram_frame.size == 0)
		return ERROR_MESSAGE_WRAPPING;

	// upon success, increment cursor
	frameWriteCursor++;
	if (frameWriteCursor == MAX_FRAME_COUNT)
		frameWriteCursor = 0;
	// save write cursor value in FRAM
	framWrite(&frameWriteCursor, FRAM_WRITE_CURSOR_ADDR, 2);

	// return success
	return SUCCESS;
}

