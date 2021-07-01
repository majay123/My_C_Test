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
 * ************Copyright 2021 MCD************
 * 
 * @version      : 
 * @Company      : HOPE
 * @Author       : MCD
 * @Date         : 2021-06-29 10:39:45
 * @LastEditors  : MCD
 * @LastEditTime : 2021-06-30 10:16:47
 * @FilePath     : /My_C_Test/point_test/common.h
 * @Description  : 
 * 
 * ******************************************
 */

#ifndef __COMMON_H__
#define __COMMON_H__

// #include <sqlite3.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <time.h>

#define MAX_BUFFER_SIZE	2048

#define PRINT_MCD_ENABLE  1
#define PRINT_LOG_ENABLE  0
#define NAME_MAX_LEN        (32)


#define MAX_DEVICES_NUM         (80)
#define DEVICE_NAME_LEN         (32)
#define DEVICE_ID_LEN           (32)
#define ELE_DEVICE_NUM          (4)
#define SCENE_TYPE_LEN          (16)
#define START_INDEX             (0)
#define SENSOR_DATA_LEN         (8)
#define MSG_CONTENT_SIZE        (1024)


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


#define PRINT_MCD_ENABLE 1


#if PRINT_MCD_ENABLE == 1
    #define print_mcd(format, arg...)                                                          \
    do                                                                                     			\
    {                                                                                      			\
        char ctime[30] = { 0 };																		\
        char ctime1[30] = { 0 };																	\
        struct tm tm1 = { 0 };																		\
        struct timespec ts; 																		\
        clock_gettime(CLOCK_REALTIME, &ts); 														\
        localtime_r(&ts.tv_sec,&tm1);																\
        strftime(ctime,sizeof(ctime),"%Y-%m-%d %H:%M:%S",&tm1); 									\
        snprintf(ctime1,sizeof(ctime),"%s.%.3ld",ctime,ts.tv_nsec/1000/1000);	                	\
        printf("\033[31m[--mcd--][%s]\033[0m:%s,%s,%d--- " format "\n", ctime1,__FILE__,__func__,__LINE__, ##arg); 	\
    } while (0)
#else
    #define print_mcd(format, arg...)   do {} while (0)
#endif  

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


/*!
 * \brief
 * 应用消息结构定义
 */
typedef struct
{
    /*! 消息类型 */
    int  type;
    /*! 消息内容 */
    char content[MSG_CONTENT_SIZE];
    /*! 发送者进程pid */
    pid_t sender_pid;
}msg_apps_t;

typedef struct
{
    uint8_t on;
    // uint8_t percent;
    int eleCata;
    char deviceName[DEVICE_NAME_LEN];
    char deviceId[DEVICE_NAME_LEN];
}ctrl_device_t;

typedef struct
{
    bool on;
    char ele_name[DEVICE_NAME_LEN];
    char deviceId[DEVICE_NAME_LEN];
}ele_device_t;

typedef struct
{
    char time[DEVICE_NAME_LEN];
    char power[SENSOR_DATA_LEN];        //power	电量	电量低时值为LOW
    uint16_t state;
    int16_t temp_value;                //temperature  2009 => 20.09℃         
    uint16_t wet_value;                 //wet  6522 => 35.22%|
    // char pm_value[SENSOR_DATA_LEN];     //pm	细颗粒物	单位ug/m3
    // char cho_value[SENSOR_DATA_LEN];    //cho	甲醛	数值30转换成0.03 mg/m3
    // char co2_value[SENSOR_DATA_LEN];    //co2	二氧化碳	单位：ppm
    // char voc_value[SENSOR_DATA_LEN];    //voc	易挥发的有机物	数值220转换成0.22 mg/m3
    uint16_t pm_value;                           //pm	细颗粒物	单位ug/m3
    uint16_t cho_value;                          //cho	甲醛	数值30转换成0.03 mg/m3
    uint16_t co2_value;                          //co2	二氧化碳	单位：ppm
    uint16_t voc_value;                          //voc	易挥发的有机物	数值220转换成0.22 mg/m3
}sensor_t;

typedef struct
{
    uint8_t dev_type;
    bool isHomepage;
    bool isDelete;
    bool flag;
    int eleCata;
    // uint8_t percent;
    uint8_t dev_status;
    char name[DEVICE_NAME_LEN];
    char deviceId[DEVICE_ID_LEN];
    bool online_state;
    // char device_status[DEVICE_NAME_LEN];
    // char power[SCENE_TYPE_LEN];
    sensor_t sensor_data;
    uint8_t ele_num;
    ele_device_t ele_dev[ELE_DEVICE_NUM];
}device_info_t;


//device struct max 80
typedef struct 
{
    int num;
    int deviceTotalNum;
    device_info_t *device_info;
}device_t;

//one scenen struct
typedef struct
{
    uint8_t index;
    uint8_t sce_type;
    bool isHomepage;
    char name[DEVICE_NAME_LEN];
    char type[SCENE_TYPE_LEN];
    char sceneId[DEVICE_NAME_LEN];
}scene_info_t;

typedef struct 
{
    int num;
    scene_info_t scene_info[MAX_DEVICES_NUM];
}all_scene_t;

typedef struct
{
    device_t mainpage_devices;
    all_scene_t mainpage_scenes;
}mainpage_all_t;

#endif // !__COMMON_H__