/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.4 */

#ifndef PB_RFILETRANSFER_PB_H_INCLUDED
#define PB_RFILETRANSFER_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Enum definitions */
typedef enum _image_type_t {
    image_type_t_FullResolution = 0,
    image_type_t_HalfResolution = 1,
    image_type_t_QuarterResolution = 2,
    image_type_t_Thumbnail = 3
} image_type_t;

/* Struct definitions */
typedef struct _antenna_telemetry {
    char dummy_field;
} antenna_telemetry;

typedef struct _battery_telemetry {
    char dummy_field;
} battery_telemetry;

typedef struct _eps_telemetry {
    char dummy_field;
} eps_telemetry;

typedef struct _camera_telemetry {
    uint32_t UpTime;
} camera_telemetry;

typedef struct _dosimeter_board_data {
    float ChannelZero;
    float ChannelOne;
    float ChannelTwo;
    float ChannelThree;
    float ChannelFour;
    float ChannelFive;
    float ChannelSix;
    float ChannelSeven;
} dosimeter_board_data;

typedef PB_BYTES_ARRAY_T(200) image_packet_Data_t;
typedef struct _image_packet {
    uint32_t Id;
    image_type_t Type;
    image_packet_Data_t Data;
} image_packet;

typedef struct _obc_telemetry {
    uint32_t Mode;
    uint32_t Uptime;
    uint32_t RtcTime;
    uint32_t RtcTemperature;
} obc_telemetry;

typedef struct _receiver_telemetry {
    float RxDoppler;
    float RxRssi;
    float BusVoltage;
    float TotalCurrent;
    float TxCurrent;
    float RxCurrent;
    float PowerAmplifierCurrent;
    float PowerAmplifierTemperature;
    float BoardTemperature;
    uint32_t Uptime;
    uint32_t Frames;
} receiver_telemetry;

typedef struct _transmitter_telemetry {
    float ReflectedPower;
    float ForwardPower;
    float BusVoltage;
    float TotalCurrent;
    float TxCurrent;
    float RxCurrent;
    float PowerAmplifierCurrent;
    float PowerAmplifierTemperature;
    float BoardTemperature;
    uint32_t Uptime;
} transmitter_telemetry;

typedef struct _dosimeter_data {
    dosimeter_board_data BoardOne;
    dosimeter_board_data BoardTwo;
} dosimeter_data;

typedef struct _transceiver_telemetry {
    receiver_telemetry Receiver;
    transmitter_telemetry Transmitter;
} transceiver_telemetry;

typedef struct _file_transfer_message {
    pb_size_t which_message;
    union {
        obc_telemetry ObcTelemetry;
        transceiver_telemetry TransceiverTelemetry;
        camera_telemetry CameraTelemetry;
        eps_telemetry EpsTelemetry;
        battery_telemetry BatteryTelemetry;
        antenna_telemetry AntennaTelemetry;
        dosimeter_data DosimeterData;
        image_packet ImagePacket;
    };
} file_transfer_message;


/* Helper constants for enums */
#define _image_type_t_MIN image_type_t_FullResolution
#define _image_type_t_MAX image_type_t_Thumbnail
#define _image_type_t_ARRAYSIZE ((image_type_t)(image_type_t_Thumbnail+1))


#ifdef __cplusplus
extern "C" {
#endif

/* Initializer values for message structs */
#define file_transfer_message_init_default       {0, {obc_telemetry_init_default}}
#define obc_telemetry_init_default               {0, 0, 0, 0}
#define receiver_telemetry_init_default          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
#define transmitter_telemetry_init_default       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
#define transceiver_telemetry_init_default       {receiver_telemetry_init_default, transmitter_telemetry_init_default}
#define camera_telemetry_init_default            {0}
#define eps_telemetry_init_default               {0}
#define battery_telemetry_init_default           {0}
#define antenna_telemetry_init_default           {0}
#define dosimeter_board_data_init_default        {0, 0, 0, 0, 0, 0, 0, 0}
#define dosimeter_data_init_default              {dosimeter_board_data_init_default, dosimeter_board_data_init_default}
#define image_packet_init_default                {0, _image_type_t_MIN, {0, {0}}}
#define file_transfer_message_init_zero          {0, {obc_telemetry_init_zero}}
#define obc_telemetry_init_zero                  {0, 0, 0, 0}
#define receiver_telemetry_init_zero             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
#define transmitter_telemetry_init_zero          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
#define transceiver_telemetry_init_zero          {receiver_telemetry_init_zero, transmitter_telemetry_init_zero}
#define camera_telemetry_init_zero               {0}
#define eps_telemetry_init_zero                  {0}
#define battery_telemetry_init_zero              {0}
#define antenna_telemetry_init_zero              {0}
#define dosimeter_board_data_init_zero           {0, 0, 0, 0, 0, 0, 0, 0}
#define dosimeter_data_init_zero                 {dosimeter_board_data_init_zero, dosimeter_board_data_init_zero}
#define image_packet_init_zero                   {0, _image_type_t_MIN, {0, {0}}}

/* Field tags (for use in manual encoding/decoding) */
#define camera_telemetry_UpTime_tag              1
#define dosimeter_board_data_ChannelZero_tag     1
#define dosimeter_board_data_ChannelOne_tag      2
#define dosimeter_board_data_ChannelTwo_tag      3
#define dosimeter_board_data_ChannelThree_tag    4
#define dosimeter_board_data_ChannelFour_tag     5
#define dosimeter_board_data_ChannelFive_tag     6
#define dosimeter_board_data_ChannelSix_tag      7
#define dosimeter_board_data_ChannelSeven_tag    8
#define image_packet_Id_tag                      1
#define image_packet_Type_tag                    2
#define image_packet_Data_tag                    3
#define obc_telemetry_Mode_tag                   1
#define obc_telemetry_Uptime_tag                 2
#define obc_telemetry_RtcTime_tag                3
#define obc_telemetry_RtcTemperature_tag         4
#define receiver_telemetry_RxDoppler_tag         1
#define receiver_telemetry_RxRssi_tag            2
#define receiver_telemetry_BusVoltage_tag        3
#define receiver_telemetry_TotalCurrent_tag      4
#define receiver_telemetry_TxCurrent_tag         5
#define receiver_telemetry_RxCurrent_tag         6
#define receiver_telemetry_PowerAmplifierCurrent_tag 7
#define receiver_telemetry_PowerAmplifierTemperature_tag 8
#define receiver_telemetry_BoardTemperature_tag  9
#define receiver_telemetry_Uptime_tag            10
#define receiver_telemetry_Frames_tag            11
#define transmitter_telemetry_ReflectedPower_tag 1
#define transmitter_telemetry_ForwardPower_tag   2
#define transmitter_telemetry_BusVoltage_tag     3
#define transmitter_telemetry_TotalCurrent_tag   4
#define transmitter_telemetry_TxCurrent_tag      5
#define transmitter_telemetry_RxCurrent_tag      6
#define transmitter_telemetry_PowerAmplifierCurrent_tag 7
#define transmitter_telemetry_PowerAmplifierTemperature_tag 8
#define transmitter_telemetry_BoardTemperature_tag 9
#define transmitter_telemetry_Uptime_tag         10
#define dosimeter_data_BoardOne_tag              1
#define dosimeter_data_BoardTwo_tag              2
#define transceiver_telemetry_Receiver_tag       1
#define transceiver_telemetry_Transmitter_tag    2
#define file_transfer_message_ObcTelemetry_tag   1
#define file_transfer_message_TransceiverTelemetry_tag 2
#define file_transfer_message_CameraTelemetry_tag 3
#define file_transfer_message_EpsTelemetry_tag   4
#define file_transfer_message_BatteryTelemetry_tag 5
#define file_transfer_message_AntennaTelemetry_tag 6
#define file_transfer_message_DosimeterData_tag  7
#define file_transfer_message_ImagePacket_tag    8

/* Struct field encoding specification for nanopb */
#define file_transfer_message_FIELDLIST(X, a) \
X(a, STATIC,   ONEOF,    MESSAGE,  (message,ObcTelemetry,ObcTelemetry),   1) \
X(a, STATIC,   ONEOF,    MESSAGE,  (message,TransceiverTelemetry,TransceiverTelemetry),   2) \
X(a, STATIC,   ONEOF,    MESSAGE,  (message,CameraTelemetry,CameraTelemetry),   3) \
X(a, STATIC,   ONEOF,    MESSAGE,  (message,EpsTelemetry,EpsTelemetry),   4) \
X(a, STATIC,   ONEOF,    MESSAGE,  (message,BatteryTelemetry,BatteryTelemetry),   5) \
X(a, STATIC,   ONEOF,    MESSAGE,  (message,AntennaTelemetry,AntennaTelemetry),   6) \
X(a, STATIC,   ONEOF,    MESSAGE,  (message,DosimeterData,DosimeterData),   7) \
X(a, STATIC,   ONEOF,    MESSAGE,  (message,ImagePacket,ImagePacket),   8)
#define file_transfer_message_CALLBACK NULL
#define file_transfer_message_DEFAULT NULL
#define file_transfer_message_message_ObcTelemetry_MSGTYPE obc_telemetry
#define file_transfer_message_message_TransceiverTelemetry_MSGTYPE transceiver_telemetry
#define file_transfer_message_message_CameraTelemetry_MSGTYPE camera_telemetry
#define file_transfer_message_message_EpsTelemetry_MSGTYPE eps_telemetry
#define file_transfer_message_message_BatteryTelemetry_MSGTYPE battery_telemetry
#define file_transfer_message_message_AntennaTelemetry_MSGTYPE antenna_telemetry
#define file_transfer_message_message_DosimeterData_MSGTYPE dosimeter_data
#define file_transfer_message_message_ImagePacket_MSGTYPE image_packet

#define obc_telemetry_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   Mode,              1) \
X(a, STATIC,   SINGULAR, UINT32,   Uptime,            2) \
X(a, STATIC,   SINGULAR, UINT32,   RtcTime,           3) \
X(a, STATIC,   SINGULAR, UINT32,   RtcTemperature,    4)
#define obc_telemetry_CALLBACK NULL
#define obc_telemetry_DEFAULT NULL

#define receiver_telemetry_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, FLOAT,    RxDoppler,         1) \
X(a, STATIC,   SINGULAR, FLOAT,    RxRssi,            2) \
X(a, STATIC,   SINGULAR, FLOAT,    BusVoltage,        3) \
X(a, STATIC,   SINGULAR, FLOAT,    TotalCurrent,      4) \
X(a, STATIC,   SINGULAR, FLOAT,    TxCurrent,         5) \
X(a, STATIC,   SINGULAR, FLOAT,    RxCurrent,         6) \
X(a, STATIC,   SINGULAR, FLOAT,    PowerAmplifierCurrent,   7) \
X(a, STATIC,   SINGULAR, FLOAT,    PowerAmplifierTemperature,   8) \
X(a, STATIC,   SINGULAR, FLOAT,    BoardTemperature,   9) \
X(a, STATIC,   SINGULAR, UINT32,   Uptime,           10) \
X(a, STATIC,   SINGULAR, UINT32,   Frames,           11)
#define receiver_telemetry_CALLBACK NULL
#define receiver_telemetry_DEFAULT NULL

#define transmitter_telemetry_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, FLOAT,    ReflectedPower,    1) \
X(a, STATIC,   SINGULAR, FLOAT,    ForwardPower,      2) \
X(a, STATIC,   SINGULAR, FLOAT,    BusVoltage,        3) \
X(a, STATIC,   SINGULAR, FLOAT,    TotalCurrent,      4) \
X(a, STATIC,   SINGULAR, FLOAT,    TxCurrent,         5) \
X(a, STATIC,   SINGULAR, FLOAT,    RxCurrent,         6) \
X(a, STATIC,   SINGULAR, FLOAT,    PowerAmplifierCurrent,   7) \
X(a, STATIC,   SINGULAR, FLOAT,    PowerAmplifierTemperature,   8) \
X(a, STATIC,   SINGULAR, FLOAT,    BoardTemperature,   9) \
X(a, STATIC,   SINGULAR, UINT32,   Uptime,           10)
#define transmitter_telemetry_CALLBACK NULL
#define transmitter_telemetry_DEFAULT NULL

#define transceiver_telemetry_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, MESSAGE,  Receiver,          1) \
X(a, STATIC,   SINGULAR, MESSAGE,  Transmitter,       2)
#define transceiver_telemetry_CALLBACK NULL
#define transceiver_telemetry_DEFAULT NULL
#define transceiver_telemetry_Receiver_MSGTYPE receiver_telemetry
#define transceiver_telemetry_Transmitter_MSGTYPE transmitter_telemetry

#define camera_telemetry_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   UpTime,            1)
#define camera_telemetry_CALLBACK NULL
#define camera_telemetry_DEFAULT NULL

#define eps_telemetry_FIELDLIST(X, a) \

#define eps_telemetry_CALLBACK NULL
#define eps_telemetry_DEFAULT NULL

#define battery_telemetry_FIELDLIST(X, a) \

#define battery_telemetry_CALLBACK NULL
#define battery_telemetry_DEFAULT NULL

#define antenna_telemetry_FIELDLIST(X, a) \

#define antenna_telemetry_CALLBACK NULL
#define antenna_telemetry_DEFAULT NULL

#define dosimeter_board_data_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, FLOAT,    ChannelZero,       1) \
X(a, STATIC,   SINGULAR, FLOAT,    ChannelOne,        2) \
X(a, STATIC,   SINGULAR, FLOAT,    ChannelTwo,        3) \
X(a, STATIC,   SINGULAR, FLOAT,    ChannelThree,      4) \
X(a, STATIC,   SINGULAR, FLOAT,    ChannelFour,       5) \
X(a, STATIC,   SINGULAR, FLOAT,    ChannelFive,       6) \
X(a, STATIC,   SINGULAR, FLOAT,    ChannelSix,        7) \
X(a, STATIC,   SINGULAR, FLOAT,    ChannelSeven,      8)
#define dosimeter_board_data_CALLBACK NULL
#define dosimeter_board_data_DEFAULT NULL

#define dosimeter_data_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, MESSAGE,  BoardOne,          1) \
X(a, STATIC,   SINGULAR, MESSAGE,  BoardTwo,          2)
#define dosimeter_data_CALLBACK NULL
#define dosimeter_data_DEFAULT NULL
#define dosimeter_data_BoardOne_MSGTYPE dosimeter_board_data
#define dosimeter_data_BoardTwo_MSGTYPE dosimeter_board_data

#define image_packet_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   Id,                1) \
X(a, STATIC,   SINGULAR, UENUM,    Type,              2) \
X(a, STATIC,   SINGULAR, BYTES,    Data,              3)
#define image_packet_CALLBACK NULL
#define image_packet_DEFAULT NULL

extern const pb_msgdesc_t file_transfer_message_msg;
extern const pb_msgdesc_t obc_telemetry_msg;
extern const pb_msgdesc_t receiver_telemetry_msg;
extern const pb_msgdesc_t transmitter_telemetry_msg;
extern const pb_msgdesc_t transceiver_telemetry_msg;
extern const pb_msgdesc_t camera_telemetry_msg;
extern const pb_msgdesc_t eps_telemetry_msg;
extern const pb_msgdesc_t battery_telemetry_msg;
extern const pb_msgdesc_t antenna_telemetry_msg;
extern const pb_msgdesc_t dosimeter_board_data_msg;
extern const pb_msgdesc_t dosimeter_data_msg;
extern const pb_msgdesc_t image_packet_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define file_transfer_message_fields &file_transfer_message_msg
#define obc_telemetry_fields &obc_telemetry_msg
#define receiver_telemetry_fields &receiver_telemetry_msg
#define transmitter_telemetry_fields &transmitter_telemetry_msg
#define transceiver_telemetry_fields &transceiver_telemetry_msg
#define camera_telemetry_fields &camera_telemetry_msg
#define eps_telemetry_fields &eps_telemetry_msg
#define battery_telemetry_fields &battery_telemetry_msg
#define antenna_telemetry_fields &antenna_telemetry_msg
#define dosimeter_board_data_fields &dosimeter_board_data_msg
#define dosimeter_data_fields &dosimeter_data_msg
#define image_packet_fields &image_packet_msg

/* Maximum encoded size of messages (where known) */
#define file_transfer_message_size               214
#define obc_telemetry_size                       24
#define receiver_telemetry_size                  57
#define transmitter_telemetry_size               51
#define transceiver_telemetry_size               112
#define camera_telemetry_size                    6
#define eps_telemetry_size                       0
#define battery_telemetry_size                   0
#define antenna_telemetry_size                   0
#define dosimeter_board_data_size                40
#define dosimeter_data_size                      84
#define image_packet_size                        211

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
