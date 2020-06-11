/*  comms-processing_tasks.c

    Created by Minh Phong Bill Truong on April 28th 2020
 */
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include <stdio.h>
#include <comms-processing_tasks.h>

//==============================================================================
//                                   External
//==============================================================================
/**
 * This Queue is used to store received and unprocessed packets
 */
QueueHandle_t rawPacketReceiveQueue;

/**
 * This Queue is used to store deparsed packets that will be used by transmitTask
 */
QueueHandle_t packetsToTransmit;

/**
 * Used to 
 */
SemaphoreHandle_t deparseSignal;

//==============================================================================
//                                  FUNCTIONS
//==============================================================================

/**
 *  @brief Call functions required to deparse packet
 *  
 *  The deparser task receives the data that is to be sent down (clarify) and calls the
 *  required functions to handle the formatting of the data into a packet. 
 * 
 *  @header	"software-and-command/main-applicatoion/comms-processing_tasks.c"
 *	@param	Unused
 *	@pre	None
 * 	@post	Formatted packet is stored in memory
 * 	@return	None
 */
void deparseTask( void* pvparameters ) 
{
    uint16_t  rawOutgoingPacket;
    uint16_t  deparsedOutgoingPacket;

    while(1) 
    {
        if ( RAW_PACKETS_QUEUED )
        {
            // Wait until signal to deparse packet is given.
            xSemaphoreTake( deparseSignal, portMAX_DELAY )
        }
        else
        {
            // Execute any tasks of equal priority that are ready now.
            taskYIELD();
        }

        // Wait for portMAX_Delay period for data to become available on the queue.
        xQueueReceive( rawPacketReceiveQueue, rawOutgoingPacket, portMAX_DELAY );

        &deparsedOutgoingPacket = deparseData( &rawOutgoingPacket ); // Stub function call
        &deparsedOutgoingPacket = encode( deparsedOutgoingPacket ); // Stub function call

        // Send data to the tail of the deparsed packets queue.
        xQueueSendToBack( packetsToTransmit, deparsedOutgoingPacket, portMAX_DELAY);
    }
}