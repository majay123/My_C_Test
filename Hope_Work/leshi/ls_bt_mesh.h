/*
 * 
 * 　　┏┓　　　┏┓+ +
 * 　┏┛┻━━━┛┻┓ + +
 * 　┃　　　　　　　┃ 　
 * 　┃　　　━　　　┃ ++ + + +
 *  ████━████ ┃+
 * 　┃　　　　　　　┃ +
 * 　┃　　　┻　　　┃
 * 　┃　　　　　　　┃ + +
 * 　┗━┓　　　┏━┛
 * 　　　┃　　　┃　　　　　　　　　　　
 * 　　　┃　　　┃ + + + +
 * 　　　┃　　　┃
 * 　　　┃　　　┃ +  神兽保佑
 * 　　　┃　　　┃    代码无bug　　
 * 　　　┃　　　┃　　+　　　　　　　　　
 * 　　　┃　 　　┗━━━┓ + +
 * 　　　┃ 　　　　　　　┣┓
 * 　　　┃ 　　　　　　　┏┛
 * 　　　┗┓┓┏━┳┓┏┛ + + + +
 * 　　　　┃┫┫　┃┫┫
 * 　　　　┗┻┛　┗┻┛+ + + +
 * 
 * 
 * 
 * ************Copyright 2023 MCD************
 * 
 * @version      : 
 * @Company      : HOPE
 * @Author       : MCD
 * @Date         : 2023-03-16 16:36:52
 * @LastEditors  : MCD
 * @LastEditTime : 2023-03-18 15:03:05
 * @FilePath     : /My_C_Test/Hope_Work/leshi/ls_bt_mesh.h
 * @Description  : 
 * 
 * ******************************************
 */

#ifndef _LS_BT_MESH_H
#define _LS_BT_MESH_H

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUILD_UINT16(loByte, hiByte) \
    ((uint16_t)(((loByte)&0x00FF) + (((hiByte)&0x00FF) << 8)))

#define HI_UINT16(a) (((a) >> 8) & 0xFF)
#define LO_UINT16(a) ((a)&0xFF)

#define BREAK_UINT32(var, ByteNum) \
    (uint8_t)((uint32_t)(((var) >> ((ByteNum)*8)) & 0x00FF))

#define BUILD_UINT32(Byte0, Byte1, Byte2, Byte3) \
    ((uint32_t)((uint32_t)((Byte0)&0x00FF) + ((uint32_t)((Byte1)&0x00FF) << 8) + ((uint32_t)((Byte2)&0x00FF) << 16) + ((uint32_t)((Byte3)&0x00FF) << 24)))

#define REQUIRE(in, tag)                   \
    do {                                   \
        if (in) {                          \
            print_log("%s %s", #tag, #in); \
            goto tag;                      \
        }                                  \
    } while (0)
#define REQUIRE_NOLOG(in, tag)                \
    do {                                      \
        if (in) {                             \
            print_common("%s %s", #tag, #in); \
            goto tag;                         \
        }                                     \
    } while (0)
#define REQ_JSON_OBJ(des, item, tag) \
    cJSON_GetObjectItem(des, #item); \
    REQUIRE((item == NULL), tag)
#define REQ_JSON_PARSE(str, item, tag) \
    cJSON_Parse(str);                  \
    REQUIRE((item == NULL), tag)

#define LESHI_DATA_HEADER     (0x55AA)
#define LESHI_DATA_VERSION    (0x00)
#define LESHI_DATA_DEVKEY     "01"
#define LESHI_DATA_HEADER_LEN (0x06)
#define LESHI_DATA_CHECK_LEN  (0x01)
#define LESHI_MAX_CMD_LEN     (220)
#define LESHI_MAX_DATA_LEN    (220 - LESHI_DATA_HEADER_LEN - 1)

typedef struct
{
    uint8_t head[2];
    uint8_t version;
    uint8_t cmd;
    uint8_t data_len[2];
} leshi_mesh_header_t;

typedef struct
{
    leshi_mesh_header_t header;
    uint8_t *data;
    uint8_t crc;
} leshi_mesh_t;

typedef struct
{
    uint8_t dpid;
    uint8_t type;
    uint8_t len[2];
    uint8_t *value;
} _leshi_dp_t;

typedef struct
{
    uint8_t id_len;
    uint8_t *sub_id;
    _leshi_dp_t **datapoint;
} leshi_ctrl_data_t;

typedef enum {
    DP_TYPE_RAW,
    DP_TYPE_BOOL,
    DP_TYPE_VALUE,
    DP_TYPE_STRING,
    DP_TYPE_ENUM,
    DP_TYPE_BITMAP,
} dp_type_e;

typedef enum {
    LESHI_CMD_DEV_INFO = 0x01,
    LESHI_CMD_CHECK_WORK_MODE = 0x02,
    LESHI_CMD_GET_NET_STATUS = 0x03,
    LESHI_CMD_RESET_WIFI = 0x04,
    LESHI_CMD_RESET_WIFI_SET_MODE = 0x05,
    LESHI_CMD_ALLOW_SUB_DEVICE_JOIN = 0x06,
    LESHI_CMD_CLOSE_SUB_DEVICE_JOIN = 0x07,
    LESHI_CMD_SUB_DEVICE_ADD = 0x08,
    LESHI_CMD_SUB_DEVICE_DELETE = 0x09,
    LESHI_CMD_HEARTBEAT_CHECK = 0x0A,
    LESHI_CMD_SUB_DEVICES_STATUS_QUERY = 0x0B,
    LESHI_CMD_SUB_DEVICES_CONTROL = 0x0C,
    LESHI_CMD_SUB_DEVICES_STATUS_REPORT = 0x0D,
    LESHI_CMD_GROUP_ADD_SUB_DEVICES = 0x0E,
    LESHI_CMD_GROUP_DELETE_SUB_DEVICES = 0x0F,
    LESHI_CMD_GET_SYSTEM_DATE = 0x10,
    LESHI_CMD_GET_LOCAL_DATE = 0x11,
    LESHI_CMD_BATCH_ADD_SUB_DEVICES = 0x12,
    LESHI_CMD_RETURN_TO_ADD_DEVICE_RESULTS = 0x13,
    LESHI_CMD_CONTROL_GROUP_DEVICES = 0x14,
    LESHI_CMD_GET_WIFI_STATUS = 0x16,
    LESHI_CMD_RESTORE_FACTORY_SETTINGS = 0x17,
    LESHI_CMD_REPORT_REMOVAL_STATUS = 0x18,
    LESHI_CMD_CHECK_SUB_DEVICE_LIST = 0x1C,
    LESHI_CMD_MCU_STARTUP_UPGRADE = 0x1D,
    LESHI_CMD_MCU_UPGRADE_PACKAGE_TRANSFER = 0x1E,
} leshi_cmd_e;

void leshi_sure_band(void);
void leshi_datapoint_parse(void);
void leshi_ctrl_switch_dev(void);
void leshi_delete_one_dev(void);
void leshi_addto_group(void);
void leshi_group_ctrl_devs(void);
void leshi_delfrom_group(void);
void leshi_heart_beat(void);

#endif  // !_LS_BT_MESH_H