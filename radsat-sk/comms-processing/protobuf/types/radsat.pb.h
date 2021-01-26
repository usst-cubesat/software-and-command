/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.4 */

#ifndef PB_RADSAT_PB_H_INCLUDED
#define PB_RADSAT_PB_H_INCLUDED
#include <pb.h>
#include "proto/fileTransfer.pb.h"
#include "proto/telecommands.pb.h"

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Struct definitions */
typedef struct _radsatMessage {
    pb_size_t which_topic;
    union {
        fileTransferMessage FileTransferMessage;
        telecommandMessage TelecommandMessage;
    } topic;
} radsatMessage;


#ifdef __cplusplus
extern "C" {
#endif

/* Initializer values for message structs */
#define radsatMessage_init_default               {0, {fileTransferMessage_init_default}}
#define radsatMessage_init_zero                  {0, {fileTransferMessage_init_zero}}

/* Field tags (for use in manual encoding/decoding) */
#define radsatMessage_FileTransferMessage_tag    1
#define radsatMessage_TelecommandMessage_tag     2

/* Struct field encoding specification for nanopb */
#define radsatMessage_FIELDLIST(X, a) \
X(a, STATIC,   ONEOF,    MESSAGE,  (topic,FileTransferMessage,topic.FileTransferMessage),   1) \
X(a, STATIC,   ONEOF,    MESSAGE,  (topic,TelecommandMessage,topic.TelecommandMessage),   2)
#define radsatMessage_CALLBACK NULL
#define radsatMessage_DEFAULT NULL
#define radsatMessage_topic_FileTransferMessage_MSGTYPE fileTransferMessage
#define radsatMessage_topic_TelecommandMessage_MSGTYPE telecommandMessage

extern const pb_msgdesc_t radsatMessage_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define radsatMessage_fields &radsatMessage_msg

/* Maximum encoded size of messages (where known) */
#define radsatMessage_size                       231

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif