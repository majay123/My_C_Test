/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.8-dev */

#ifndef PB_DMESSAGE_PB_H_INCLUDED
#define PB_DMESSAGE_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Enum definitions */
/* protocol for hope 104 gateway gwservice communication with rs485 service */
typedef enum _messageType {
    messageType_MSGCODE_INVALID = 0,
    messageType_MSGCODE_DISCOVER_RS485_DEV_REQ = 1,
    messageType_MSGCODE_DISCOVER_RS485_DEV_RSP = 2,
    messageType_MSGCODE_CTRL_RS485_ACTION_REQ = 3,
    messageType_MSGCODE_CTRL_RS485_ACTION_RSP = 4,
    messageType_MSGCODE_GET_DEVS_STATUS_REQ = 5,
    messageType_MSGCODE_GET_DEVS_STATUS_RSP = 6,
    messageType_MSGCODE_GET_ONE_DEV_STATUS_REQ = 7,
    messageType_MSGCODE_GET_ONE_DEV_STATUS_RSP = 8,
    messageType_MSGCODE_DEVICE_READY_NOTIFY = 9,
    messageType_MSGCODE_DEVICE_NOT_READY_NOTIFY = 10,
    messageType_MSGCODE_JOIN_ACTION_REQ = 11,
    messageType_MSGCODE_JOIN_ACTION_RSP = 12,
    messageType_MSGCODE_LEAVE_ACTION_REQ = 13,
    messageType_MSGCODE_LEAVE_ACTION_RSP = 14,
    messageType_MSGCODE_SYNC_DEVICE_NOTIFY = 15,
    messageType_MSGCODE_SYNC_SCENE_NOTIFY = 16,
    messageType_MSGCODE_ADD_GROUP_SUB_DEVS_REQ = 17,
    messageType_MSGCODE_ADD_GROUP_SUB_DEVS_RSP = 18,
    messageType_MSGCODE_CTRL_GOURP_SUB_DEVS_REQ = 19,
    messageType_MSGCODE_CTRL_GOURP_SUB_DEVS_RSP = 20,
    messageType_MSGCODE_DELETE_GOURP_SUB_DEVS_REQ = 21,
    messageType_MSGCODE_DELETE_GOURP_SUB_DEVS_RSP = 22,
    messageType_MSGCODE_DELETE_SUB_DEVS_REQ = 23,
    messageType_MSGCODE_DELETE_SUB_DEVS_RSP = 24,
    messageType_MSGCODE_HEARTBEAT_NOTIFY = 25,
    messageType_MSGCODE_ADD_SUB_DEVICE_REQ = 26,
    messageType_MSGCODE_ADD_SUB_DEVICE_RSP = 27,
    messageType_MSGCODE_GET_GATEWAY_INFO_REQ = 28,
    messageType_MSGCODE_GET_GATEWAY_INFO_RSP = 29,
    messageType_MSGCODE_GATEWAY_OTA_INFO_REQ = 30,
    messageType_MSGCODE_GATEWAY_OTA_INFO_RSP = 31,
    messageType_MSGCODE_GET_GATEWAY_MACADDR_REQ = 32,
    messageType_MSGCODE_GET_GATEWAY_MACADDR_RSP = 33 /* MSGCODE_GATEWAY_START_OTA_REQ=30;
 MSGCODE_GATEWAY_START_OTA_RSP=31;
 MSGCODE_GATEWAY_OTA_PACKAGE_TRANSFER_REQ=32;
 MSGCODE_GATEWAY_OTA_PACKAGE_TRANSFER_RSP=33; */
} messageType;

typedef enum _messageDpType {
    messageDpType_DP_TYPE_RAW = 0,
    messageDpType_DP_TYPE_BOOL = 1,
    messageDpType_DP_TYPE_VALUE = 2,
    messageDpType_DP_TYPE_STRING = 3,
    messageDpType_DP_TYPE_ENUM = 4,
    messageDpType_DP_TYPE_BITMAP = 5
} messageDpType;

typedef enum _GatewayOta_gw_ota_sta {
    GatewayOta_gw_ota_sta_GW_OTA_FAILURE = 0,
    GatewayOta_gw_ota_sta_GW_OTA_SUCCESS = 1
} GatewayOta_gw_ota_sta;

/* Struct definitions */
typedef struct _Datapoint {
    uint32_t dpid;
    messageDpType type;
    uint32_t len;
    pb_size_t which_t_value;
    union {
        char v_raw[24];
        bool v_bool;
        int32_t v_value;
        char v_string[24];
        int32_t v_enum;
        int32_t v_bitmap;
    } t_value;
} Datapoint;

typedef struct _leshiBTmeshDeviceMsg {
    char deviceId[16]; /* device id sub device id */
    char devicePid[16]; /* device pid */
    bool online; /* online status (status) */
    int32_t dp_size;
    Datapoint dp[8]; /* datapoint(ctrl and status) */
    int32_t groupId; /* group id */
    pb_size_t which_rets;
    union {
        int32_t ret;
    } rets;
    char deviceVer[12]; /* device version */
    int32_t hb_time; /* heartbeat time */
} leshiBTmeshDeviceMsg;

typedef struct _btActionMsg {
    pb_size_t which_actionId;
    union {
        char deviceId[16]; /* control device id sub device id */
        int32_t groupId; /* control group id(don't need response) */
    } actionId;
    char deviceName[16]; /* display device name(for switch name or device name) */
    char devicePid[16]; /* device pid */
    bool online; /* online status (status) */
    int32_t dp_size;
    Datapoint dp[4]; /* datapoint(ctrl and status) only one */
    int32_t ret;
} btActionMsg;

typedef struct _Gatewayinfo {
    char v[16];
    uint32_t m;
    uint32_t cap;
    char gw_macaddr[16];
} Gatewayinfo;

typedef struct _GatewayOta {
    char GwOtaPath[32];
    GatewayOta_gw_ota_sta OtaSta;
} GatewayOta;

/* main page push and pop message */
typedef struct _combineMsg {
    uint32_t index; /* ordinal of item */
    bool has_action;
    btActionMsg action;
} combineMsg;

typedef struct _commonMessage {
    uint64_t serial; /* message sequence number */
    uint32_t combo_size;
    combineMsg combo[30];
    int32_t groupId;
} commonMessage;


#ifdef __cplusplus
extern "C" {
#endif

/* Helper constants for enums */
#define _messageType_MIN messageType_MSGCODE_INVALID
#define _messageType_MAX messageType_MSGCODE_GET_GATEWAY_MACADDR_RSP
#define _messageType_ARRAYSIZE ((messageType)(messageType_MSGCODE_GET_GATEWAY_MACADDR_RSP+1))

#define _messageDpType_MIN messageDpType_DP_TYPE_RAW
#define _messageDpType_MAX messageDpType_DP_TYPE_BITMAP
#define _messageDpType_ARRAYSIZE ((messageDpType)(messageDpType_DP_TYPE_BITMAP+1))

#define _GatewayOta_gw_ota_sta_MIN GatewayOta_gw_ota_sta_GW_OTA_FAILURE
#define _GatewayOta_gw_ota_sta_MAX GatewayOta_gw_ota_sta_GW_OTA_SUCCESS
#define _GatewayOta_gw_ota_sta_ARRAYSIZE ((GatewayOta_gw_ota_sta)(GatewayOta_gw_ota_sta_GW_OTA_SUCCESS+1))

#define Datapoint_type_ENUMTYPE messageDpType




#define GatewayOta_OtaSta_ENUMTYPE GatewayOta_gw_ota_sta




/* Initializer values for message structs */
#define Datapoint_init_default                   {0, _messageDpType_MIN, 0, 0, {""}}
#define leshiBTmeshDeviceMsg_init_default        {"", "", 0, 0, {Datapoint_init_default, Datapoint_init_default, Datapoint_init_default, Datapoint_init_default, Datapoint_init_default, Datapoint_init_default, Datapoint_init_default, Datapoint_init_default}, 0, 0, {0}, "", 0}
#define btActionMsg_init_default                 {0, {""}, "", "", 0, 0, {Datapoint_init_default, Datapoint_init_default, Datapoint_init_default, Datapoint_init_default}, 0}
#define Gatewayinfo_init_default                 {"", 0, 0, ""}
#define GatewayOta_init_default                  {"", _GatewayOta_gw_ota_sta_MIN}
#define combineMsg_init_default                  {0, false, btActionMsg_init_default}
#define commonMessage_init_default               {0, 0, {combineMsg_init_default, combineMsg_init_default, combineMsg_init_default, combineMsg_init_default, combineMsg_init_default, combineMsg_init_default, combineMsg_init_default, combineMsg_init_default, combineMsg_init_default, combineMsg_init_default, combineMsg_init_default, combineMsg_init_default, combineMsg_init_default, combineMsg_init_default, combineMsg_init_default, combineMsg_init_default, combineMsg_init_default, combineMsg_init_default, combineMsg_init_default, combineMsg_init_default, combineMsg_init_default, combineMsg_init_default, combineMsg_init_default, combineMsg_init_default, combineMsg_init_default, combineMsg_init_default, combineMsg_init_default, combineMsg_init_default, combineMsg_init_default, combineMsg_init_default}, 0}
#define Datapoint_init_zero                      {0, _messageDpType_MIN, 0, 0, {""}}
#define leshiBTmeshDeviceMsg_init_zero           {"", "", 0, 0, {Datapoint_init_zero, Datapoint_init_zero, Datapoint_init_zero, Datapoint_init_zero, Datapoint_init_zero, Datapoint_init_zero, Datapoint_init_zero, Datapoint_init_zero}, 0, 0, {0}, "", 0}
#define btActionMsg_init_zero                    {0, {""}, "", "", 0, 0, {Datapoint_init_zero, Datapoint_init_zero, Datapoint_init_zero, Datapoint_init_zero}, 0}
#define Gatewayinfo_init_zero                    {"", 0, 0, ""}
#define GatewayOta_init_zero                     {"", _GatewayOta_gw_ota_sta_MIN}
#define combineMsg_init_zero                     {0, false, btActionMsg_init_zero}
#define commonMessage_init_zero                  {0, 0, {combineMsg_init_zero, combineMsg_init_zero, combineMsg_init_zero, combineMsg_init_zero, combineMsg_init_zero, combineMsg_init_zero, combineMsg_init_zero, combineMsg_init_zero, combineMsg_init_zero, combineMsg_init_zero, combineMsg_init_zero, combineMsg_init_zero, combineMsg_init_zero, combineMsg_init_zero, combineMsg_init_zero, combineMsg_init_zero, combineMsg_init_zero, combineMsg_init_zero, combineMsg_init_zero, combineMsg_init_zero, combineMsg_init_zero, combineMsg_init_zero, combineMsg_init_zero, combineMsg_init_zero, combineMsg_init_zero, combineMsg_init_zero, combineMsg_init_zero, combineMsg_init_zero, combineMsg_init_zero, combineMsg_init_zero}, 0}

/* Field tags (for use in manual encoding/decoding) */
#define Datapoint_dpid_tag                       1
#define Datapoint_type_tag                       2
#define Datapoint_len_tag                        3
#define Datapoint_v_raw_tag                      4
#define Datapoint_v_bool_tag                     5
#define Datapoint_v_value_tag                    6
#define Datapoint_v_string_tag                   7
#define Datapoint_v_enum_tag                     8
#define Datapoint_v_bitmap_tag                   9
#define leshiBTmeshDeviceMsg_deviceId_tag        2
#define leshiBTmeshDeviceMsg_devicePid_tag       5
#define leshiBTmeshDeviceMsg_online_tag          6
#define leshiBTmeshDeviceMsg_dp_size_tag         7
#define leshiBTmeshDeviceMsg_dp_tag              8
#define leshiBTmeshDeviceMsg_groupId_tag         9
#define leshiBTmeshDeviceMsg_ret_tag             10
#define leshiBTmeshDeviceMsg_deviceVer_tag       11
#define leshiBTmeshDeviceMsg_hb_time_tag         12
#define btActionMsg_deviceId_tag                 1
#define btActionMsg_groupId_tag                  2
#define btActionMsg_deviceName_tag               3
#define btActionMsg_devicePid_tag                5
#define btActionMsg_online_tag                   6
#define btActionMsg_dp_size_tag                  7
#define btActionMsg_dp_tag                       8
#define btActionMsg_ret_tag                      10
#define Gatewayinfo_v_tag                        1
#define Gatewayinfo_m_tag                        2
#define Gatewayinfo_cap_tag                      3
#define Gatewayinfo_gw_macaddr_tag               4
#define GatewayOta_GwOtaPath_tag                 1
#define GatewayOta_OtaSta_tag                    2
#define combineMsg_index_tag                     1
#define combineMsg_action_tag                    7
#define commonMessage_serial_tag                 1
#define commonMessage_combo_size_tag             2
#define commonMessage_combo_tag                  3
#define commonMessage_groupId_tag                4

/* Struct field encoding specification for nanopb */
#define Datapoint_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   dpid,              1) \
X(a, STATIC,   SINGULAR, UENUM,    type,              2) \
X(a, STATIC,   SINGULAR, UINT32,   len,               3) \
X(a, STATIC,   ONEOF,    STRING,   (t_value,v_raw,t_value.v_raw),   4) \
X(a, STATIC,   ONEOF,    BOOL,     (t_value,v_bool,t_value.v_bool),   5) \
X(a, STATIC,   ONEOF,    INT32,    (t_value,v_value,t_value.v_value),   6) \
X(a, STATIC,   ONEOF,    STRING,   (t_value,v_string,t_value.v_string),   7) \
X(a, STATIC,   ONEOF,    INT32,    (t_value,v_enum,t_value.v_enum),   8) \
X(a, STATIC,   ONEOF,    INT32,    (t_value,v_bitmap,t_value.v_bitmap),   9)
#define Datapoint_CALLBACK NULL
#define Datapoint_DEFAULT NULL

#define leshiBTmeshDeviceMsg_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, STRING,   deviceId,          2) \
X(a, STATIC,   SINGULAR, STRING,   devicePid,         5) \
X(a, STATIC,   SINGULAR, BOOL,     online,            6) \
X(a, STATIC,   SINGULAR, INT32,    dp_size,           7) \
X(a, STATIC,   FIXARRAY, MESSAGE,  dp,                8) \
X(a, STATIC,   SINGULAR, INT32,    groupId,           9) \
X(a, STATIC,   ONEOF,    INT32,    (rets,ret,rets.ret),  10) \
X(a, STATIC,   SINGULAR, STRING,   deviceVer,        11) \
X(a, STATIC,   SINGULAR, INT32,    hb_time,          12)
#define leshiBTmeshDeviceMsg_CALLBACK NULL
#define leshiBTmeshDeviceMsg_DEFAULT NULL
#define leshiBTmeshDeviceMsg_dp_MSGTYPE Datapoint

#define btActionMsg_FIELDLIST(X, a) \
X(a, STATIC,   ONEOF,    STRING,   (actionId,deviceId,actionId.deviceId),   1) \
X(a, STATIC,   ONEOF,    INT32,    (actionId,groupId,actionId.groupId),   2) \
X(a, STATIC,   SINGULAR, STRING,   deviceName,        3) \
X(a, STATIC,   SINGULAR, STRING,   devicePid,         5) \
X(a, STATIC,   SINGULAR, BOOL,     online,            6) \
X(a, STATIC,   SINGULAR, INT32,    dp_size,           7) \
X(a, STATIC,   FIXARRAY, MESSAGE,  dp,                8) \
X(a, STATIC,   SINGULAR, INT32,    ret,              10)
#define btActionMsg_CALLBACK NULL
#define btActionMsg_DEFAULT NULL
#define btActionMsg_dp_MSGTYPE Datapoint

#define Gatewayinfo_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, STRING,   v,                 1) \
X(a, STATIC,   SINGULAR, UINT32,   m,                 2) \
X(a, STATIC,   SINGULAR, UINT32,   cap,               3) \
X(a, STATIC,   SINGULAR, STRING,   gw_macaddr,        4)
#define Gatewayinfo_CALLBACK NULL
#define Gatewayinfo_DEFAULT NULL

#define GatewayOta_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, STRING,   GwOtaPath,         1) \
X(a, STATIC,   SINGULAR, UENUM,    OtaSta,            2)
#define GatewayOta_CALLBACK NULL
#define GatewayOta_DEFAULT NULL

#define combineMsg_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   index,             1) \
X(a, STATIC,   OPTIONAL, MESSAGE,  action,            7)
#define combineMsg_CALLBACK NULL
#define combineMsg_DEFAULT NULL
#define combineMsg_action_MSGTYPE btActionMsg

#define commonMessage_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT64,   serial,            1) \
X(a, STATIC,   SINGULAR, UINT32,   combo_size,        2) \
X(a, STATIC,   FIXARRAY, MESSAGE,  combo,             3) \
X(a, STATIC,   SINGULAR, INT32,    groupId,           4)
#define commonMessage_CALLBACK NULL
#define commonMessage_DEFAULT NULL
#define commonMessage_combo_MSGTYPE combineMsg

extern const pb_msgdesc_t Datapoint_msg;
extern const pb_msgdesc_t leshiBTmeshDeviceMsg_msg;
extern const pb_msgdesc_t btActionMsg_msg;
extern const pb_msgdesc_t Gatewayinfo_msg;
extern const pb_msgdesc_t GatewayOta_msg;
extern const pb_msgdesc_t combineMsg_msg;
extern const pb_msgdesc_t commonMessage_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define Datapoint_fields &Datapoint_msg
#define leshiBTmeshDeviceMsg_fields &leshiBTmeshDeviceMsg_msg
#define btActionMsg_fields &btActionMsg_msg
#define Gatewayinfo_fields &Gatewayinfo_msg
#define GatewayOta_fields &GatewayOta_msg
#define combineMsg_fields &combineMsg_msg
#define commonMessage_fields &commonMessage_msg

/* Maximum encoded size of messages (where known) */
#define Datapoint_size                           39
#define GatewayOta_size                          35
#define Gatewayinfo_size                         46
#define btActionMsg_size                         239
#define combineMsg_size                          248
#define commonMessage_size                       7558
#define leshiBTmeshDeviceMsg_size                421

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
