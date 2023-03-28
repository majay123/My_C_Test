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
 * @LastEditTime : 2023-03-27 15:13:53
 * @FilePath     : /My_C_Test/Hope_Work/leshi/include/ls_bt_mesh.h
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

#include "queue.h"

#define LESHI_DATA_HEADER1                     (0x55)
#define LESHI_DATA_HEADER2                     (0xAA)
#define LESHI_DATA_VERSION                     (0x00)
#define LESHI_DATA_DEVKEY                      "01"
#define LESHI_DATA_HEADER_LEN                  (0x06)
#define LESHI_DATA_CHECK_LEN                   (0x01)
#define LESHI_MAX_CMD_LEN                      (220)
#define LESHI_MAX_DATA_LEN                     (220 - LESHI_DATA_HEADER_LEN - 1)
#define LESHI_DATAPOINT_HEADER_LEN             (0x04)
#define LESHI_UPGRADE_PACKET_TRANSFER_DATA_LEN (0x01)
#define LESHI_UPGRADE_PACKET_TRANSFER_128BY    (128)
#define LESHI_UPGRADE_PACKET_TRANSFER_256BY    (256)
#define LESHI_UPGRADE_PACKET_TRANSFER_512BY    (512)
#define LESHI_UPGRADE_PACKET_TRANSFER_1024BY   (1024)



/* ********************************Product ID******************************** */

#define PID_LESHI_BTMESH_MONOCHROMATIC_LAMP           ("drmec7gltih2y8dk")  // 单色灯
#define PID_LESHI_BTMESH_DOUBLE_COLOR_LAMP            ("fkxcslivaluonzdp")  // 双色灯
#define PID_LESHI_BTMESH_RGB_COLOR_LAMP               ("bajiy6qlrhkojes1")  // RGB灯
#define PID_LESHI_BTMESH_WYRGB_COLOR_LAMP             ("zvkvdjg9zugnj24o")  // WYRGB灯
#define PID_LESHI_BTMESH_DOUBLE_COLOR_AUXILIARY_LAMP  ("6esfe09radrtnhrw")  // 双色灯+辅灯
#define PID_LESHI_BTMESH_MONOCHROMATIC_AUXILIARY_LAMP ("ahxotwaemo4laafa")  // 单色灯+辅灯
#define PID_LESHI_BTMESH_WRGB_COLOR_LAMP              ("ovjddr13qzti3ak1")  // WRGB灯
#define PID_LESHI_BTMESH_SWITCH_CONTROLLER            ("qr3d4hqa1cvmryqh")  // 开关控制器
#define PID_LESHI_BTMESH_CLASSROOM_MONOCHROMATIC_LAMP ("o4tsf1polfhm5ihi")  // 教室单色灯
#define PID_LESHI_BTMESH_ILLUMINANCE_SENSOR           ("fksd53uubkkrrzlt")  // 照度传感器
#define PID_LESHI_BTMESH_86_HUMAN_PRESENCE_SENSOR     ("oexsr8buz4ssaikc")  // 86人体存在传感器
#define PID_LESHI_BTMESH_8601_HUMAN_PRESENCE_SENSOR   ("lmaacizghyyjojk9")  // 8601人体存在传感器
#define PID_LESHI_BTMESH_ELECTRIC_CURTAIN             ("h536ubqx1tui1jsj")  // 电动窗帘
#define PID_LESHI_BTMESH_ON_OFF_DEVICE                ("qr3d4hqa1cvmryqh")  // 通断器
#define PID_LESHI_BTMESH_01_SWITCH                    ("tj8sr3yxp0cqalbk")  // 1路开关
#define PID_LESHI_BTMESH_02_SWITCH                    ("5zkpavp5cxonvvxl")  // 2路开关
#define PID_LESHI_BTMESH_03_SWITCH                    ("kbp6a3sa72p3tgpg")  // 3路开关
#define PID_LESHI_BTMESH_04_SWITCH                    ("lroj54nnitbqwfct")  // 4路开关
#define PID_LESHI_BTMESH_01_SWITCH_CFG_01_KEY         ("hlx1f6gjshxoda1w")  // 1 路开关-按键可配置-1开1键
#define PID_LESHI_BTMESH_02_SWITCH_CFG_02_KEY         ("ndamshy1ooxwcauz")  // 2 路开关-按键可配置-2开2键
#define PID_LESHI_BTMESH_03_SWITCH_CFG_03_KEY         ("2lzmpca0avpyoalu")  // 3 路开关-按键可配置-3开3键
#define PID_LESHI_BTMESH_04_SWITCH_CFG_04_KEY         ("zrwrkvpjintwhdov")  // 4 路开关-按键可配置-4开4键
#define PID_LESHI_BTMESH_04_SWITCH_CFG_08_KEY         ("bb2g2m8jbmngg08t")  // 4 路开关-按键可配置-4开8键
#define PID_LESHI_BTMESH_PLUG_IN_CARD_FOR_POWER_04    ("4ckwmpiilpnqolvn")  // 插卡取电04
#define PID_LESHI_BTMESH_THERMOSTAT                   ("u6abdnaoesdttqsb")  // 温控器
#define PID_LESHI_BTMESH_DOOR_LOCK                    ("oziw4jskdo4wuasy")  // 门锁
#define PID_LESHI_BTMESH_WATER_SOAKING                ("q0jy234acen6fpcv")  // 水浸
#define PID_LESHI_BTMESH_SMOKE                        ("pnnd48aqvxtvmrdu")  // 烟雾
#define PID_LESHI_BTMESH_FUEL_GAS                     ("gvswh2hpggabuagh")  // 燃气
#define PID_LESHI_BTMESH_01_SWITCH_SCENE_PANEL        ("pavlgpew3swkppx6")  // 1 键情景面板
#define PID_LESHI_BTMESH_02_SWITCH_SCENE_PANEL        ("axlaycksi24fi5za")  // 2 键情景面板
#define PID_LESHI_BTMESH_03_SWITCH_SCENE_PANEL        ("cny1dsyww6uxwif6")  // 3 键情景面板
#define PID_LESHI_BTMESH_04_SWITCH_SCENE_PANEL        ("eeadlj0gkwm9nbcq")  // 4 键情景面板
#define PID_LESHI_BTMESH_05_SWITCH_SCENE_PANEL        ("wlqadn8nivobqsfn")  // 5 键情景面板
#define PID_LESHI_BTMESH_06_SWITCH_SCENE_PANEL        ("m3j6hfimgelnzqos")  // 6 键情景面板
#define PID_LESHI_BTMESH_TEMPERATURE_HUMIDITY_SENSOR  ("fumjev3k4txhavh9")  // 温湿度传感器
#define PID_LESHI_BTMESH_REMOTE_CONTROL               ("fkruzgm7aui46xby")  // 遥控器

/* ********************************Datapoint ID******************************** */

/* 调光开关 */
#define DPID_SWITCH_LED    20   //开关-布尔类型
#define DPID_STATE_VALUE   21   //模式-数值-【0-WY 模式    1-RBG 模】
#define DPID_BRIGHT_VALUE  22   //亮度-数值-千分比
#define DPID_TEMP_VALUE    23   //色温-数值-【0-1000】
#define DPID_RGB_VALUE     24   //RGB 彩光-字符串-【HSV】

#define DPID_SPECIAL_VALUE 101  //特殊命令-数值-【0-禁止对码， 1-禁止情景保存， 2-允许对码， 3-允许情景保存】

#define DPID_SUBSW1_VALUE  102  //辅灯  1 开关-布尔变量
#define DPID_SUBSW2_VALUE  103  //辅灯  2 开关-布尔变量

#define DPID_SUBAUOT_SW    102  //教室灯自动调光开关-布尔变量

/* 恒照度控制器 */
#define DPID_BRINGHT_VALUE    2    //环境照度上报-数值-【0-20000】
#define DPID_BRAUOT_SW        102  //自动控制开关-布尔变量
#define DPID_BRINGHT_SETVALUE 103  //设置照度-数值-【0-20000】
#define DPID_BRINGHT_EVALUE   104  //照度误差百分比-数值-【0-100】

/* 电动窗帘 */
#define DPID_CURTAIN_SW    1  //电机开关-布尔变量
#define DPID_PERCENT       2  //设置开合百分比-数值-【0-100】
#define DPID_STATE_PERCENT 3  //当前开合百分比-数值-【0-100】

/* 插座类 */
#define DPID_SOCKET1 1 //插座开关-布尔变量 
#define DPID_SOCKET2 2 //插座开关-布尔变量 
#define DPID_SOCKET3 3 //插座开关-布尔变量 
#define DPID_SOCKET4 4 //插座开关-布尔变量

/* 86 面板 */
#define DPID_86SW1 1 //插座开关 1-布尔变量 
#define DPID_86SW2 2 //插座开关 2-布尔变量 
#define DPID_86SW3 3 //插座开关 3-布尔变量 
#define DPID_86SW4 4 //插座开关 4-布尔变量

/* 声光报警器开关 */
#define DPID_SHENGUANG   13   //开关-布尔变量 

/* 门磁0 */
#define DPID_DOORSENSOR  1  //门磁
#define DPID_DOORBATTERY 2  //电量

/* 红外1 */
#define DPID_PIRSTATE    1  //人来=0,人走=1
#define DPID_PIRBATTERY1 4  //电量

/* 水浸3 */
#define DPID_WATERSTATE   1  //0 水浸报警  1 水浸报警解除
#define DPID_WATERBATTERY 4  //电量

/* 烟雾4 */
#define DPID_SMOKESTATE   1    //0 烟雾报警  1 烟雾报警解除
#define DPID_SMOKEBATTERY 15   //电量
#define DPID_CLEARWARNING 101  //清除报警

/* 煤气5 */
#define DPID_GASSTATE     1    //0 煤气报警  1 煤气报警解除
#define DPID_GASBATTERY   15   //电量-百分比
#define DPID_CLEARWARNING 101  //清除报警-布尔变量
#define DPID_SECEN_SW1    1    //情景控制面板 1 键-枚举-0 按下
#define DPID_SECEN_SW2    2    //情景控制面板 2 键-枚举-0 按下
#define DPID_SECEN_SW3    3    //情景控制面板 3 键-枚举-0 按下
#define DPID_SECEN_SW4    4    //情景控制面板 4 键-枚举-0 按下
#define DPID_SECEN_SW5    5    //情景控制面板 5 键-枚举-0 按下
#define DPID_SECEN_SW6    6    //情景控制面板 6 键-枚举-0 按下

/* 商用类-0x82 新温控器 */
#define DPID_BUSINESS1_ONOFF     1   //-布尔
#define DPID_BUSINESS1_TEMPNOW   24  //-数据    16-30 摄氏度-当前温度
#define DPID_BUSINESS1_TEMPSET   16  //-数据    16-30 摄氏度-设置温度
#define DPID_BUSINESS1_TEMPSPEED 28  //-枚举范围：auto, low, middle, high -风速/风向【bit3-bit0】
#define DPID_BUSINESS1_TEMPSDIR  14  //-枚举范围：no_angle, min_angle, mid_angle, remark1, max_angle, remark2, remark3, swing -风速/风向【bit7-bit4】
#define DPID_BUSINESS1_TEMPMODE  2   //-枚举范围：R0, hot1, cool2, R3, wind4, R5, R6, R7, dehumidification8 -工作模式【bit6-bit0】

/* 86 人体存在传感器 */
#define DPID_86PIR_STATE     1    //是否有人-枚举
#define DPID_86PIR_STATEHAS  101  //人存在-枚举
#define DPID_86PIR_SENS      102  //灵敏度-数值百分比
#define DPID_86PIR_TIMES     103  //人体存在维持时间-枚举值: 30S, 60S, 120S, 180S, 300S, 600S, 1200S, 1800S
#define DPID_86PIR_OPENSTATE 104  //本地触发等级  none-监测到人则本地触发   
                                  //active-监测到大幅度动作则本地触发       
                                  //fallow-监测到小幅度动作则本地触发       
                                  //sleep-监测到人呼吸动作则本地触发
#define DPID_86PIR_SENDDATA  105  //数据透传
#define DPID_86PIR_MAC       106  //MAC
#define DPID_86PIR_LED       107  //指示灯-布尔
#define DPID_86PIR_SETBR     109  //设置照度
#define DPID_86PIR_NOWBR     110  //当前照度
#define DPID_86PIR_SW        111  //继电器开关

/* 8601 人体存在传感器-新传感器 */
#define DPID_86PIR01_STATE     1    //是否有人-枚举
#define DPID_86PIR01_STATEHAS  101  //人存在-枚举    无人/运动/静止/运动&静止
#define DPID_86PIR01_MSENS     102  //运动灵敏度-数值百分比
#define DPID_86PIR01_TIMES     103  //人体存在维持时间-枚举值: 30S, 60S, 120S, 180S, 300S, 600S, 1200S, 1800S
#define DPID_86PIR01_SSENS     104  //静止灵敏度-数值百分比
#define DPID_86PIR01_SENDDATA  105  //数据透传-string
#define DPID_86PIR01_PIRNULLT  106  //无人时间-类比
#define DPID_86PIR01_LED       107  //指示灯-布尔
#define DPID_86PIR01_LEIDA     108  //雷达开关-布尔
#define DPID_86PIR01_SETBR     109  //设置照度
#define DPID_86PIR01_NOWBR     110  //当前照度
#define DPID_86PIR01_SW        111  //继电器开关
#define DPID_86PIR01_DISTANCE  112  //感应距离设置
#define DPID_86PIR01_PIRACTION 113  //感应动作配置

/* 温湿度 */
#define DPID_TEMPLATE_V 1  //温度
#define DPID_HUM_V      2  //湿度

/* 遥控器 */
#define DPID_REMOTE_RID  101  //
#define DPID_REMOTE_TYPE 102
#define DPID_GETSTATE    103  //

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
    leshi_mesh_header_t header;
    uint8_t crc;
} leshi_mesh_base_t;


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
    _leshi_dp_t *datapoint;
    uint8_t dp_num;
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

typedef enum {
    LS_PACKAGE_SIZE_256_BYTE = 0,
    LS_PACKAGE_SIZE_512_BYTE,
    LS_PACKAGE_SIZE_1024_BYTE,
    LS_PACKAGE_SIZE_128_BYTE,
} leshi_upgrade_packet_size_e;

typedef enum {
    LS_GROUP_ADD_SUB_DEVICE_SUCCESS = 0,
    LS_GROUP_ADD_SUB_DEVICE_OUT_UPPER_LIMIT,
    LS_GROUP_ADD_SUB_DEVICE_TIMEOUT,
    LS_GROUP_ADD_SUB_DEVICE_SET_OUT_OF_RANGE,
    LS_GROUP_ADD_SUB_DEVICE_WRITE_FILE_ERR,
    LS_GROUP_ADD_SUB_DEVICE_OTHER_ERR,
} leshi_group_add_sub_res_e;

typedef enum {
    LS_GROUP_DEL_SUB_DEVICE_SUCCESS = 0,
    LS_GROUP_DEL_SUB_DEVICE_OUT_UPPER_LIMIT,
    LS_GROUP_DEL_SUB_DEVICE_TIMEOUT,
    LS_GROUP_DEL_SUB_DEVICE_SET_OUT_OF_RANGE,
    LS_GROUP_DEL_SUB_DEVICE_WRITE_FILE_ERR,
    LS_GROUP_DEL_SUB_DEVICE_OTHER_ERR,
} leshi_group_del_sub_res_e;

int ls_process_cmd_back_send(int fd, llq_t *llq_handle);
int ls_bt_mesh_dispatch(llq_t *llq_handle);

#endif  // !_LS_BT_MESH_H
