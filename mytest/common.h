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
 * @
 * ************Copyright 2021 MCD************
 * 
 * @version      : 
 * @Company      : HOPE
 * @Author       : MCD
 * @Date         : 2020-12-17 16:26:47
 * @LastEditors  : MCD
 * @LastEditTime : 2023-04-10 13:09:52
 * @FilePath     : /My_C_Test/mytest/common.h
 * @Description  : 
 * @
 * ******************************************
 */

#ifndef __COMMON_H__
#define __COMMON_H__

// #include <sqlite3.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_BUFFER_SIZE	2048

#define PRINT_MCD_ENABLE  1
#define PRINT_LOG_ENABLE  0
#define NAME_MAX_LEN        (32)

#define MAX_DEVICES_NUM         (80)
#define DEVICE_NAME_LEN         (32)
#define DEVICE_ID_LEN           (32)
#define ELE_DEVICE_NUM          (4)
#define SCENE_TYPE_LEN          (16)

#define MQTT_PORT               ("ff")
#define MQTT_APPLICATUIN        ("gwui")
#define MQTT_DESTINATION        ("gwservice")

#define MOSQUITTO_TOPIC_IN              ("/mqtt/hope/gwui")
#define MOSQUITTO_TOPIC_OUT             ("/mqtt/hope/gwservice")
#define MOSQUITTO_MSG_DIST              ("gwui")
#define MOSQUITTO_MSG_SRC               ("gwservice")
#define MOSQUITTO_TOPIC_ZB				("topic")				//zigbee service topic

// mqtt message code
#define MQTT_MSGCODE_CHECK_BIND_STATUS_REQ               (1)
#define MQTT_MSGCODE_CHECK_BIND_STATUS_RSP               (2)
#define MQTT_MSGCODE_BIND_ACTION_REQ                        (3)
#define MQTT_MSGCODE_BIND_ACTION_RSP                        (4)
#define MQTT_MSGCODE_UNBIND_ACTION_REQ                   (5)
#define MQTT_MSGCODE_UNBIND_ACTION_RSP                  (6)
#define MQTT_MSGCODE_JOIN_ACTION_REQ                  (7)
#define MQTT_MSGCODE_JOIN_ACTION_RSP                  (8)
#define MQTT_MSGCODE_LEAVE_ACTION_REQ                  (9)
#define MQTT_MSGCODE_LEAVE_ACTION_RSP                  (10)
#define MQTT_MSGCODE_PUSH_DEV_ACTION_REQ                  (11)
#define MQTT_MSGCODE_PUSH_DEV_ACTION_RSP                  (12)
#define MQTT_MSGCODE_POP_DEV_ACTION_REQ                  (13)
#define MQTT_MSGCODE_POP_DEV_ACTION_RSP                  (14)
#define MQTT_MSGCODE_GET_ALL_DEV_REQ                  (15)
#define MQTT_MSGCODE_GET_ALL_DEV_RSP                  (16)
#define MQTT_MSGCODE_GET_ALL_SCENE_REQ                  (17)
#define MQTT_MSGCODE_GET_ALL_SCENE_RSP                  (18)
#define MQTT_MSGCODE_CTRL_DEV_ACTION_REQ                  (19)
#define MQTT_MSGCODE_CTRL_DEV_ACTION_RSP                  (20)
#define MQTT_MSGCODE_CTRL_SCENE_ACTION_REQ                  (21)
#define MQTT_MSGCODE_CTRL_SCENE_ACTION_RSP                  (22)
#define MQTT_MSGCODE_PUSH_SCENE_ACTION_REQ                  (23)
#define MQTT_MSGCODE_PUSH_SCENE_ACTION_RSP                  (24)
#define MQTT_MSGCODE_POP_SCENE_ACTION_REQ                  (25)
#define MQTT_MSGCODE_POP_SCENE_ACTION_RSP                  (26)
#define MQTT_MSGCODE_TOTAL_DEV_ACTION_REQ                  (27)
#define MQTT_MSGCODE_TOTAL_DEV_ACTION_RSP                  (28)
#define MQTT_MSGCODE_TOTAL_SCENE_ACTION_REQ                  (29)
#define MQTT_MSGCODE_TOTAL_SCENE_ACTION_RSP                  (30)
#define MQTT_MSGCODE_POP_ALL_ACTION_REQ                  (31)
#define MQTT_MSGCODE_POP_ALL_ACTION_RSP                  (32)

typedef enum
{
    HOPE_DB_LIGHT_TYPE = 1,         //灯
    HOPE_DB_CURTAIN_TYPE,           //窗帘
    HOPE_DB_AIR_CONDITIONER_TYPE,   //空调
    HOPE_DB_SCENE_TYPE,             //情景
}db_info_type_e;

// action
typedef enum
{
    HOPE_DB_ACTION_NULL = 0,        //无动作
    HOPE_DB_ACTION_CLOSE = 1,       //设备关
    HOPE_DB_ACTION_OPEN,            //设备开
    HOPE_DB_ACTION_STOP,            //设备停止
    HOPE_DB_ACTION_MODE,            //设置模式（空调）
    HOPE_DB_ACTION_SPEED,           //设置风速（空调）
    // HOPE_DB_ACTION_TO,              //设置温度,风速（空调）
    HOPE_DB_ACTION_ADD,             //控制加（空调）
    HOPE_DB_ACTION_REDUCE,          //控制减（空调）
}db_action_e;

// typedef enum
// {
//     HOPE_DB_ATTRIBUTE_NULL = 0,             //无动作
//     HOPE_DB_ATTRIBUTE_TEMPERATURE = 1,      //温度
//     HOPE_DB_ATTRIBUTE_WIND_SPEED,           //速度
//     HOPE_DB_ATTRIBULE_VALUE,                //模式
// }db_attribute_e;

// 空调模式 attribute
typedef enum
{
    MODE_COOL = 0,
    MODE_HEAT,
    MODE_AIR,
    MODE_DRY,
}db_ac_mode_e;

// 空调风速 attribute
typedef enum
{
    VALUE_NULL = 0,
    VALUE_LOW = 1,
    VALUE_MIDDLE,
    VALUE_HIGH,
    VALUE_AUTO,
}db_ac_speed_e;

typedef struct
{
    char dev_type;
    char name[NAME_MAX_LEN];
    char action;
    char attribute;
}db_ctrl_t;

typedef struct  
{
    uint8_t action;
    char name_action[NAME_MAX_LEN];
    char do_action[NAME_MAX_LEN];
    // uint8_t attribute;
}_db_rs485_dis_t;

typedef struct
{
    uint8_t mode;
    char m_name[NAME_MAX_LEN];
}_db_rs485_ms_t;

#if PRINT_MCD_ENABLE == 1
    #define print_mcd(format, arg...)   do { printf("\033[31m[----mcd----]\033[0m:%s,%d--- "format"\n",__FILE__,__LINE__, ## arg);}\
                                        while (0)
#else
    #define print_mcd(format, arg...)   do {} while (0)
#endif

#if PRINT_LOG_ENABLE == 1
    #define print_log(format, arg...)   do { printf("\033[31m[----log----]\033[0m:%s,%d--- "format"\n",__FILE__,__LINE__, ## arg);}\
                                        while (0)
#else
    #define print_log(format, arg...)   do {} while (0)
#endif


#define BREAK_UINT32( var, ByteNum ) \
          (uint8_t)((uint32_t)(((var) >>((ByteNum) * 8)) & 0x00FF))

#define BUILD_UINT32(Byte0, Byte1, Byte2, Byte3) \
          ((uint32_t)((uint32_t)((Byte0) & 0x00FF) \
          + ((uint32_t)((Byte1) & 0x00FF) << 8) \
          + ((uint32_t)((Byte2) & 0x00FF) << 16) \
          + ((uint32_t)((Byte3) & 0x00FF) << 24)))

#define BUILD_UINT16(loByte, hiByte) \
          ((uint16_t)(((loByte) & 0x00FF) + (((hiByte) & 0x00FF) << 8)))

#define HI_UINT16(a) (((a) >> 8) & 0xFF)
#define LO_UINT16(a) ((a) & 0xFF)

#define LO_UINT24(a) ((a) & 0xFFF)

#define BUILD_UINT8(hiByte, loByte) \
          ((uint8_t)(((loByte) & 0x0F) + (((hiByte) & 0x0F) << 4)))

#define HI_UINT8(a) (((a) >> 4) & 0x0F)
#define LO_UINT8(a) ((a) & 0x0F)

#define REQUIRE(in,tag)                     do{if(in){print_log("%s %s", #tag, #in); goto tag;}}while(0)
#define REQUIRE_NOLOG(in,tag)              	do{if(in){print_log("%s %s", #tag, #in); goto tag;}}while(0)
#define REQ_JSON_OBJ(des,item,tag)        	cJSON_GetObjectItem(des,#item); \
                                                           REQUIRE((item==NULL),tag)
#define REQ_JSON_PARSE(str,item,tag)     	cJSON_Parse(str); \
                                                            REQUIRE((item==NULL),tag)

#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))
#define ARRAY_SIZE(A)    (sizeof(A)/sizeof((A)[0]))


/*!
 * \brief
 * 应用消息结构定义
 */
typedef struct
{
    /*! 消息类型 */
    int  code;
    /*! 消息内容 */
    char *content;
    /*! 消息长度 */
    int size;
}mqtt_msg_apps_t;

typedef struct
{
    bool on;
    char ele_name[DEVICE_NAME_LEN];
    char deviceId[DEVICE_NAME_LEN];
}ele_device_t;


typedef struct
{
    uint8_t dev_type;
    bool isHomepage;
    bool isDelete;
    bool flag;
    uint16_t eleCata;
    char name[DEVICE_NAME_LEN];
    char deviceId[DEVICE_ID_LEN];
    bool online_state;
    char time[DEVICE_NAME_LEN];
    char device_status[DEVICE_NAME_LEN];
    uint8_t ele_num;
    ele_device_t ele_dev[ELE_DEVICE_NUM];
}device_info_t;

typedef struct 
{
    int num;
    device_info_t device_info[MAX_DEVICES_NUM];
}device_t;

typedef struct
{
    uint8_t index;
    char name[DEVICE_NAME_LEN];
    char type[SCENE_TYPE_LEN];
    char sceneId[DEVICE_NAME_LEN];
}scene_info_t;

typedef struct 
{
    int num;
    scene_info_t scenes_info[MAX_DEVICES_NUM];
}scene_t;


typedef struct
{
    uint8_t code;       //mqtt msg code
    bool aciton;        //0 bind start, 1 bind stop
    uint8_t join_time;  //start search device, 0 is stop search, < 0 is search time
    uint8_t startIndex; //MQTT_MSGCODE_GET_ALL_DEV_REQ MQTT_MSGCODE_GET_ALL_SCENE_REQ
    uint8_t itemCount;  //MQTT_MSGCODE_GET_ALL_DEV_REQ MQTT_MSGCODE_GET_ALL_SCENE_REQ
}mqtt_msg_t;

#endif // !__COMMON_H__