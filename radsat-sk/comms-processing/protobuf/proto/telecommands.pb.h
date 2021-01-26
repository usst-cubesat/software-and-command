/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.4 */

#ifndef PB_PROTO_TELECOMMANDS_PB_H_INCLUDED
#define PB_PROTO_TELECOMMANDS_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Struct definitions */
typedef struct _ceaseTransmission {
    int32_t delay;
} ceaseTransmission;

typedef struct _telecommandMessage {
    pb_size_t which_message;
    union {
        ceaseTransmission CeaseTransmission;
    } message;
} telecommandMessage;


#ifdef __cplusplus
extern "C" {
#endif

/* Initializer values for message structs */
#define telecommandMessage_init_default          {0, {ceaseTransmission_init_default}}
#define ceaseTransmission_init_default           {0}
#define telecommandMessage_init_zero             {0, {ceaseTransmission_init_zero}}
#define ceaseTransmission_init_zero              {0}

/* Field tags (for use in manual encoding/decoding) */
#define ceaseTransmission_delay_tag              1
#define telecommandMessage_CeaseTransmission_tag 1

/* Struct field encoding specification for nanopb */
#define telecommandMessage_FIELDLIST(X, a) \
X(a, STATIC,   ONEOF,    MESSAGE,  (message,CeaseTransmission,message.CeaseTransmission),   1)
#define telecommandMessage_CALLBACK NULL
#define telecommandMessage_DEFAULT NULL
#define telecommandMessage_message_CeaseTransmission_MSGTYPE ceaseTransmission

#define ceaseTransmission_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, INT32,    delay,             1)
#define ceaseTransmission_CALLBACK NULL
#define ceaseTransmission_DEFAULT NULL

extern const pb_msgdesc_t telecommandMessage_msg;
extern const pb_msgdesc_t ceaseTransmission_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define telecommandMessage_fields &telecommandMessage_msg
#define ceaseTransmission_fields &ceaseTransmission_msg

/* Maximum encoded size of messages (where known) */
#define telecommandMessage_size                  13
#define ceaseTransmission_size                   11

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
