/**
 * @file RProtobuf.h
 * @date January 25, 2021
 * @author Tyrel Kostyk (tck290)
 */

#ifndef RPROTOBUF_H_
#define RPROTOBUF_H_

#include <pb_encode.h>
#include <pb_decode.h>

#include <RRadsat.pb.h>


/***************************************************************************************************
                                             DEFINITIONS
***************************************************************************************************/

/** The maximum size of a NanoPB encoded message (not including the message header). */
#define PROTO_MAX_ENCODED_SIZE	(radsat_message_size)


/***************************************************************************************************
                                             PUBLIC API
***************************************************************************************************/

uint8_t protoEncode(radsat_message* rawMessage, uint8_t* outgoingBuffer);
int protoDecode(uint8_t* incomingBuffer, radsat_message* decodedMessage);


#endif /* RPROTOBUF_H_ */
