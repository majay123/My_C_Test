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
 * @Date         : 2021-08-24 13:24:14
 * @LastEditors  : MCD
 * @LastEditTime : 2022-09-16 16:09:07
 * @FilePath     : /My_C_Test/CRC16_MODBUS/eide_rs485.h
 * @Description  : 
 * 
 * ******************************************
 */

#ifndef __EIDE_RS485_H
#define __EIDE_RS485_H

#include <stdint.h>

// 0            4            7               11              15          19              23              27              31
// +------------+-----------++----------------+--------------++-----------+--------------++---------------+--------------++
// | 引导码 BB BB                                             | 数据长度                   |   版本号      |     预留      |
// +------------+-----------++----------------+--------------++-----------+---------------++--------------+--------------++
// | 数据标志                                                 |    标识    |               数据偏移量                      |
// +------------+-----------++----------------+--------------++-----------+---------------++--------------+--------------++
// |                                                      原模块标识                                                      |
// |                                                        8byte                                                        |
// +------------+-----------++---------------+---------------++-----------+---------------++---------------+-------------++
// |                                                      目的模块标识                                                     |
// |                                                          8byte                                                       |
// +------------+-----------++---------------+---------------++-----------+---------------++----------------+------------++
// |       数据加密类型       |            数据码              |                   具体数据部分
// +-----------+-----------++----------------+---------------++-----------+---------------++----------------+------------++
//                                                            |                        CRC 校验码                         |

#define EIDE_DEV_PATH              ("/dev/ttyS2")
#define EIDE_RS485_MAX_LEN         (256)
#define EIDE_RS485_HEARD_SIZE      (2)
#define EIDE_RS485_CRC_SIZE        (2)
#define EIDE_RS485_MIN_DATA_LEN    (27)
#define EIDE_RS485_MAX_DATA_LEN    (127)
#define EIDE_RS485_SOF             (0xBB)
#define EIDE_MODULE_NAME_SIZE      (128)
#define EIDE_MODULE_MACADDR_SIZE   (8)
#define EIDE_RS485_MAX_DATA_SIZE   (128)
#define EIDE_RS485_HEAD_LEN        (23)
#define EIDE_RS485_FIXED_LEN       EIDE_RS485_HEAD_LEN + EIDE_RS485_CRC_SIZE
#define EIDE_RS485_VERSION         (0x00)
#define EIDE_RS485_FLAGS           (0x00)
#define EIDE_RS485_ENCRYPTION_TYPE (0x00) // 485 总线暂时为 0；
#define EIDE_RS485_MAX_FUNC_SIZE   (0x08)
#define EIDE_RS485_MAX_DN_DEVICES  (0x08)

// 数据码
typedef enum {
    EIDE_DATACODE_CONFIG  = 1, // 配置码
    EIDE_DATACODE_CONTROL = 2, // 控制码
    EIDE_DATACODE_RESPOND,     // 响应码
    EIDE_DATACODE_ERROR,       // 错误码
    EIDE_DATACODE_STATUS,      // 状态码
} eide_rs485_datacode_t;

// 配置码的操作
typedef enum {
    EIDE_CONFIG_READ = 0, // 读取:0
    EIDE_CONFIG_WRITE,    // 写入:1
    EIDE_CONFIG_ADD,      // 添加:2
    EIDE_CONFIG_DELETE,   // 删除:3
} eide_rs485_config_t;

// 配置功能值
typedef enum {
    EIDE_CONFIG_CODE_MODULE_STA = 1,        // 模块状态
    EIDE_CONFIG_CODE_MODULE_DEVICE_INFO,    // 模块设备信息
    EIDE_CONFIG_CODE_DECICE_INFO,           // 具体设备信息
    EIDE_CONFIG_CODE_DEVICE_FUNC_INFO,      // 具体设备功能信息
    EIDE_CONFIG_CODE_MAC_CONFIG,            // MAC配置
    EIDE_CONFIG_CODE_FACTORY_RESET,         // 恢复出厂设置
    EIDE_CONFIG_CODE_UPGRADE,               // 升级
    EIDE_CONFIG_CODE_MODULE_NAME,           // 模块名称
    EIDE_CONFIG_CODE_DEVICE_NAME,           // 设备名称
    EIDE_CONFIG_CODE_FUNC_NAME,             // 功能名称
    EIDE_CONFIG_CODE_DEVICE_TYPE,           // 设备类型
    EIDE_CONFIG_CODE_BIND_MODULE,           // 模块绑定
    EIDE_CONFIG_CODE_FUNC_DATA_INFO,        // 功能数据信息
    EIDE_CONFIG_CODE_DATE_OF_GATEWAY,       // 网关时间
    EIDE_CONFIG_CODE_TIME,                  // 时间
    EIDE_CONFIG_CODE_VERSION,               // 固件版本号
    EIDE_CONFIG_CODE_SENSITIVITY,           // 灵敏度
    EIDE_CONFIG_CODE_CALIBRATION,           // 校准
    EIDE_CONFIG_CODE_GROUP_CONFIG,          // 组配置
    EIDE_CONFIG_CODE_FUNC_CODE_BIND_DEVICE, // 功能值绑定设备
    EIDE_CONFIG_CODE_FUNC_BIND_DEVICE_FUNC, // 功能绑定设备功能
    EIDE_CONFIG_CODE_TIME_OTHER,            // 时间
    EIDE_CONFIG_CODE_EQUIPMENT_AUTH,        // 设备授权
    // EIDE_CONFIG_CODE_,                          // 朱哥需要调试用，无具体内容
    EIDE_CONFIG_CODE_POWER_DOWN_RECOVERY = 25, // 掉电是否恢复掉电前的输出
    EIDE_CONFIG_CODE_ENABLE,                   // 启用与不启用
    EIDE_CONFIG_CODE_SCENE_SET = 0x1001,       // 场景配置信息
    EIDE_CONFIG_CODE_AUTO_SET_SCENE,           // 自动化配置信息
    EIDE_CONFIG_CODE_FIND_GATEWAY,             // 网关发现
} eide_485_config_func_code_t;

// 设备类型
typedef enum {
    EIDE_DEVICE_GATEWAY = 1,        // 接口
    EIDE_DEVICE_RELAY,              // 继电器
    EIDE_DEVICE_LIGHT,              // 灯
    EIDE_DEVICE_SOCKET,             // 插座
    EIDE_DEVICE_CURTAIN,            // 窗帘
    EIDE_DEVICE_AIRCONDITIONER,     // 空调
    EIDE_DEVICE_MUSICAMPLIFIER,     // 背景音乐--喇叭
    EIDE_DEVICE_MUSIC,              // 背景音乐--喇叭+主机
    EIDE_DEVICE_MUSICHOST,          // 背景音乐主机
    EIDE_DEVICE_CONTACTIN,          // 干接点输入
    EIDE_DEVICE_TEMPERATURESENSOR,  // 温度传感器
    EIDE_DEVICE_HUMIDITYSENSOR,     // 湿度传感器
    EIDE_DEVICE_ILLUMINATIONSENSOR, // 照度传感器
    EIDE_DEVICE_MOVESENSOR,         // 人体移动传感器
    EIDE_DEVICE_DOORCONTACTSENSOR,  // 门磁传感器
    EIDE_DEVICE_PM25SENSOR,         // PM2.5 传感器
    EIDE_DEVICE_VOCSENSOR,          // VOC传感器
    EIDE_DEVICE_CO2SENSOR,          // CO2传感器
    EIDE_DEVICE_COALGASSENSOR,      // 煤气传感器
    EIDE_DEVICE_WATEROUTSENSOR,     // 水浸传感器
    EIDE_DEVICE_DOORLOCK,           // 门锁
    EIDE_DEVICE_INFRAREDEMIT,       // 红外发射
    EIDE_DEVICE_BUTTON,             // 按键
    EIDE_DEVICE_FLOORHEAT,          // 地暖
    EIDE_DEVICE_CONTACTOUT,         // 干结点输出
    EIDE_DEVICE_TV,                 // TV
    EIDE_DEVICE_PROJECTOR,          // 投影
    EIDE_DEVICE_GROUP,              // 虚拟组设备
    EIDE_DEVICE_MACRO,              // 虚拟宏设备
    EIDE_DEVICE_SCENE,              // 虚拟场景设备
    EIDE_DEVICE_DIMMERPWM,          // PWM调光
    EIDE_DEVICE_DIMMER0T10V,        // 0-10V调光
    EIDE_DEVICE_ELECTRICMETER,      // 能源设备
    EIDE_DEVICE_ELECTRICFAN,        // 风扇
    EIDE_DEVICE_REFRIGERATOR,       // 冰箱
    EIDE_DEVICE_SWITCH,             // 开关
    EIDE_DEVICE_AMPLIFIER,          // 功放
    EIDE_DEVICE_PLAYER,             // 播放器
    EIDE_DEVICE_SEQUENCER,          // 时序器
    EIDE_DEVICE_RCU,                // RCU
    EIDE_DEVICE_DOORBELL,           // 门铃
    EIDE_DEVICE_AIRQUALITYSENSOR,   // 空气质量传感器
    EIDE_DEVICE_YSCAMERA,           // 萤石摄像头
    EIDE_DEVICE_COMPUTER,           // 电脑
    EIDE_DEVICE_AUTOMATION,         // 自动化
    EIDE_DEVICE_CUSTOM,             // 自定义设备
    EIDE_DEVICE_VIDEOPLAYER,        // 视频播放器
} eide_rs485_device_type_t;

// 功能权限：包括可读、可写、可主动状态报告、可报警
typedef enum {
    EIDE_FUNC_ACCESS_ONLY_READ = 0, // 可读
    EIDE_FUNC_ACCESS_ONLY_WRITE,    // 可写
    EIDE_FUNC_ACCESS_AUTO_REPORT,   // 可主动状态报告
    EIDE_FUNC_ACCESS_WARNING,       // 可报警
} eide_rs485_func_access_t;

// 功能类型
typedef enum {
    EIDE_FUNC_TYPE_RJ45 = 1,             //RJ45
    EIDE_FUNC_TYPE_RS485,                //RS485
    EIDE_FUNC_TYPE_485EIDE24V,           //485 IEIDE 24V
    EIDE_FUNC_TYPE_RS232,                //RS232
    EIDE_FUNC_TYPE_SWITCH,               //开关
    EIDE_FUNC_TYPE_BRIGHTNESS,           //亮度
    EIDE_FUNC_TYPE_COLORRGB,             //RGB颜色
    EIDE_FUNC_TYPE_OPENCLOSESTOP,        //开关停
    EIDE_FUNC_TYPE_PERCENTAGEHORIZONTAL, //百分比（水平）
    EIDE_FUNC_TYPE_PERCENTAGEVERTICAL,   //百分比（垂直）
    EIDE_FUNC_TYPE_MODE,                 //模式
    EIDE_FUNC_TYPE_TARGETTEMPERATURE,    //目标温度
    EIDE_FUNC_TYPE_CURRENTTEMPERATURE,   //当前温度
    EIDE_FUNC_TYPE_WINDSPEED,            //风速
    EIDE_FUNC_TYPE_STARTSTOP,            //播放控制/启动、暂停
    EIDE_FUNC_TYPE_VOLUME,               //音量调节
    EIDE_FUNC_TYPE_UPDOWNCONTROL,        //上下控制
    EIDE_FUNC_TYPE_PLAYMODE,             //播放模式
    EIDE_FUNC_TYPE_PLAYLIST,             //播放列表
    EIDE_FUNC_TYPE_PLAYTOTALTIME,        //当前歌曲总时长
    EIDE_FUNC_TYPE_PLAYPROGRESS,         //当前播放进度
    EIDE_FUNC_TYPE_PLAYNAME,             //当前播放歌曲
    EIDE_FUNC_TYPE_CURRENTILLUMINATION,  //当前照度
    EIDE_FUNC_TYPE_INFRAREDCODE,         //红外码
    EIDE_FUNC_TYPE_DIRECTIONCONTROL,     //方向控制
    EIDE_FUNC_TYPE_BEFOREMOVEDELAY,      //运行前延时
    EIDE_FUNC_TYPE_MOVEDELAY,            //运行时间
    EIDE_FUNC_TYPE_AFTERMOVEDELAY,       //运行后延时
    EIDE_FUNC_TYPE_HUMANDETECTION,       //有人/无人
    EIDE_FUNC_TYPE_BUTTONSTATUS,         //按键状态
    EIDE_FUNC_TYPE_INDICATORSWITCH,      //指示灯开关
    EIDE_FUNC_TYPE_ADDSUBTRACT,          //音量加减
    EIDE_FUNC_TYPE_CURRENTPM25,          //当前PM2.5值
    EIDE_FUNC_TYPE_CURRENTVOC,           //当前VOC值
    EIDE_FUNC_TYPE_CURRENTCO2,           //当前CO2值
    EIDE_FUNC_TYPE_CURRENTCOALGAS,       //当前煤气值
    EIDE_FUNC_TYPE_CURRENTHUMIDITY,      //当前湿度
    EIDE_FUNC_TYPE_OUTPUTVOLTAGE,        //输出电压
    EIDE_FUNC_TYPE_CURRENTCURRENT,       //当前电流
    EIDE_FUNC_TYPE_CURRENTVOLTAGE,       //当前电压
    EIDE_FUNC_TYPE_KEYENTER,             //确定键
    EIDE_FUNC_TYPE_KEYRETURN,            //返回键
    EIDE_FUNC_TYPE_KEYMENU,              //菜单键
    EIDE_FUNC_TYPE_KEYMUTE,              //静音键
    EIDE_FUNC_TYPE_MACRO,                //宏
    EIDE_FUNC_TYPE_SCENE,                //场景
    EIDE_FUNC_TYPE_VOLTAGEFREQUENCY,     //电压频率
    EIDE_FUNC_TYPE_ACTIVEPOWER,          //视在功率
    EIDE_FUNC_TYPE_ACTIVEENERGY,         //有功能量
    EIDE_FUNC_TYPE_COLORRGBWH,           //RGBWH颜色
    EIDE_FUNC_TYPE_COLORWH,              //WH颜色
    EIDE_FUNC_TYPE_NOTICEAUDIO,          //提示音频
    EIDE_FUNC_TYPE_NOTICETEXT,           //提示文本
    EIDE_FUNC_TYPE_DIMMING,              //调光
    EIDE_FUNC_TYPE_BATTERYLEVEL,         //当前电量
    EIDE_FUNC_TYPE_,                     //指定播放
    EIDE_FUNC_TYPE_HUMANDETECTIONMATRIX, //有人/无人矩阵
    EIDE_FUNC_TYPE_INVALIDATION,         //无效化/锁定
    EIDE_FUNC_TYPE_COLORTEMPERATURE,     //色温
    EIDE_FUNC_TYPE_COLORRGBW,            //RGBW
    EIDE_FUNC_TYPE_TUY           = 61,   //平台对接 61--涂鸦
    EIDE_FUNC_TYPE_ADONGYINGHALL = 62,   //平台对接 62--东营营业厅
    EIDE_FUNC_TYPE_ROOMSTATUS    = 70,   //房态
    EIDE_FUNC_TYPE_CARDSTATUS,           //插卡状态
    EIDE_FUNC_TYPE_DOORDISPLAY,          //门显
    EIDE_FUNC_TYPE_KEYNUMBER,            //数字键
    EIDE_FUNC_TYPE_KEYMAINPAGE,          //主页键
    EIDE_FUNC_TYPE_FORWARDBACKWARD,      //快进快退
    EIDE_FUNC_TYPE_KEYSUBTITLE,          //字幕键
    EIDE_FUNC_TYPE_KEYTRACK,             //音轨键
    EIDE_FUNC_TYPE_KEYSTOP,              //停止键
    EIDE_FUNC_TYPE_KEYDVD,               //DVD
    EIDE_FUNC_TYPE_KEYBLURAY,            //BLU-RAY
    EIDE_FUNC_TYPE_KEYSATCBL,            //SAT/CBL
    EIDE_FUNC_TYPE_KEYNET,               //NET
    EIDE_FUNC_TYPE_KEYAUX1,              //AUX1
    EIDE_FUNC_TYPE_KEYAUX2,              //AUX2
    EIDE_FUNC_TYPE_KEYUSBIPOD,           //USB/IPOD
    EIDE_FUNC_TYPE_KEYGAME,              //GAME
    EIDE_FUNC_TYPE_KEYMOVIE,             //MOVIE
    EIDE_FUNC_TYPE_KEYMUSIC,             //MUSIC
    EIDE_FUNC_TYPE_KEYDIRECT,            //DIRECT
    EIDE_FUNC_TYPE_KEYAUDIOTV,           //AUDIO TV
    EIDE_FUNC_TYPE_KEYANFM,              //ANFM
    EIDE_FUNC_TYPE_KEYOPTION,            //OPTION
    EIDE_FUNC_TYPE_KEYINFO,              //INFO
    EIDE_FUNC_TYPE_SIGNALSOURCE,         //信号源
    EIDE_FUNC_TYPE_KEYSET,               //设置键
    EIDE_FUNC_TYPE_KEYSIGNALSOURCE,      //信号源键
    EIDE_FUNC_TYPE_DOORLOCKSTATUS,       //门锁状态
    EIDE_FUNC_TYPE_DOORLOCKALARM,        //门锁告警
    EIDE_FUNC_TYPE_DOOORLOCKUSER,        //门锁用户
    EIDE_FUNC_TYPE_CUSTOM,               //自定义功能
} eide_rs485_func_type_t;

// 数据类型定义
typedef enum {
    EIDE_DATA_TYPE_NO = 0,             // 无类型 0
    EIDE_DATA_TYPE_BOOLEAN,            // 布尔类型 1
    EIDE_DATA_TYPE_CHAR,               // 单字节类型 2
    EIDE_DATA_TYPE_UNSIGNED_CHAR,      // 无符号单字节类型 3
    EIDE_DATA_TYPE_SHORE_INT,          // 双字节类型 4
    EIDE_DATA_TYPE_UNSIGNED_SHORT_INT, // 无符号双字节类型 5
    EIDE_DATA_TYPE_INT,                // 整数类型 6
    EIDE_DATA_TYPE_UNSIGNED_INT,       // 无符号整数类型 7
    EIDE_DATA_TYPE_FLOAT,              // 浮点类型 8
    EIDE_DATA_TYPE_STRING,             // 字符串类型 9
    EIDE_DATA_TYPE_ENUM,               // 枚举类型 10
    EIDE_DATA_TYPE_STRUCT,             // 结构体类型 11
    EIDE_DATA_TYPE_BUFF,               // 字节数组 12
} eide_re485_data_type_t;

//  数据单位
typedef enum {
    EIDE_DATA_UNIT_ON = 0,  // 没有单位 0
    EIDE_DATA_UNIT_PERCENT, // 百分比（%） 1
    EIDE_DATA_UNIT_OC,      // 摄氏度（℃） 2
    EIDE_DATA_UNIT_OF,      // 华氏度（℉ ） 3
    EIDE_DATA_UNIT_PPM,     // PPM（PPM） 4
    EIDE_DATA_UNIT_MGM3,    // 毫克每立方米（mg/m³） 5
    EIDE_DATA_UNIT_S,       // 秒（s） 6
    EIDE_DATA_UNIT_MS,      // 毫秒（ms） 7
    EIDE_DATA_UNIT_V,       // 伏特（V） 8
    EIDE_DATA_UNIT_A,       // 安培（A） 9
    EIDE_DATA_UNIT_MA,      // 毫安（mA） 10
    EIDE_DATA_UNIT_LUX,     // 勒克司（Lux） 11
    EIDE_DATA_UNIT_HZ,      // 赫兹（Hz） 12
    EIDE_DATA_UNIT_W,       // 瓦（W） 13
    EIDE_DATA_UNIT_WH,      // 瓦每小时（W/h） 14
    EIDE_DATA_UNIT_MV,      // 毫伏（mV） 15
    EIDE_DATA_UNIT_K,       // 开尔文(K)   16
    EIDE_DATA_UNIT_M3,      // 微克每立方米（ug/m³） 17
} eide_rs485_data_unit_t;

// 错误码
typedef enum {
    EIDE_CODE_SUCCESS = 0X0000, // 未发生错误
    EIDE_CODE_BOOT_CODE_ERR,    // 引导码错误
    EIDE_CODE_DATA_LENGTH_ERR, // 数据长度字段过长 OR 数据长度字段过短
    EIDE_CODE_VERSION_CANNOT_BE_PARSED_ERR, // 版本号不能解析
    EIDE_CODE_ENCRYPTION_TYPE_ERR,          // 数据加密类型不能解析
    EIDE_CODE_DATA_CODE_ERR,                // 数据码错误
    EIDE_CODE_CRC_ERR,                      // CRC校验码错误
    EIDE_CODE_CONFIG_CODE_ERR,              // 不是配置码错误
    EIDE_CODE_CONFIG_OPERATION_CODE_ERR,    // 不是对应的配置码操作
    EIDE_CODE_CONFIG_FUNCTION_CODE_ERR,     // 不是对应的配置功能值
    EIDE_CODE_EMPTY_DATA_RETURNED_ERR,      // 返回数据为空
    EIDE_CODE_CANNOT_TO_ADD_EQUIPMENT_ERR  = 0x8000, // 不能继续增加设备
    EIDE_CODE_DEVICE_CANNOT_BE_DELETED_ERR = 0x8001, // 此设备不能删除
    EIDE_CODE_GROUP_DEVICE_TYPE_ERR = 0xC007, // 组中设备不是同一类型
    EIDE_CODE_NOT_ENOUGH_STORAGE_ERR,         // 存储不够
    EIDE_CODE_AUTH_MAC_IS_INCORRECT_ERR,      // 被授权MAC不对
    EIDE_CODE_AUTH_VERIFICATION_FAILURE_ERR,  // 认证校验失败
    EIDE_CODE_OPEN_CIRCUIT_PROTECTION_ERR,    // 断路保护
    EIDE_CODE_MODULE_FIRMWARE_ERR,            // 该模块的固件不对
    EIDE_CODE_MD5_CHECK_ERR,                  // MD5校验不对
} eide_rs485_err_code_t;

typedef struct {
    uint8_t device_id;

} eide_device_info_t;

typedef struct {
    uint8_t        module_name[EIDE_MODULE_NAME_SIZE];
    uint8_t        module_macaddr[EIDE_MODULE_MACADDR_SIZE];
    unsigned short module_type;
    uint8_t        device_type_num;
    uint8_t        device_num;
} eide_module_info_t;

typedef struct {
    uint8_t guid[EIDE_RS485_HEARD_SIZE];
    uint8_t data_length;
    uint8_t version;
    uint8_t serial_number[EIDE_RS485_HEARD_SIZE];
    uint8_t flag_offset[EIDE_RS485_HEARD_SIZE];
    uint8_t src_address[EIDE_MODULE_MACADDR_SIZE];
    uint8_t dst_address[EIDE_MODULE_MACADDR_SIZE];
    uint8_t encryption_type;
} eide_rs485_head_t;

typedef struct {
    uint8_t ctrl_dev_count;
    uint8_t device_id;
    uint8_t dev_func_num;
    uint8_t func_id;
    uint8_t data_type;
    uint8_t data[4];
} eide_rs485_one_dev_ctrl_t;

typedef struct {
    uint8_t func_id;
    uint8_t data_type;
    uint8_t data[4];
} eide_rs485_func_ctrl_t;

typedef struct {
    uint8_t                dev_id;
    uint8_t                dev_func_num;
    eide_rs485_func_ctrl_t funcs[EIDE_RS485_MAX_FUNC_SIZE];
} eide_rs485_dev_ctrl_t;

typedef struct {
    uint8_t               ctrl_dev_count;
    eide_rs485_dev_ctrl_t ctrl[EIDE_RS485_MAX_DN_DEVICES];
} eide_rs485_devs_ctrl_t;

typedef struct {
    uint8_t data_code;
    uint8_t data[EIDE_RS485_MAX_DATA_SIZE];
} eide_rs485_payload_t;

typedef struct {
    eide_rs485_head_t    header;
    eide_rs485_payload_t payload;
    uint8_t              crc16_check[2];
} eide_rs485_msg_t;

int   eide_rs485_open(char *device_path);
void  eide_rs485_close(int fd);
int   eide_read_rs485_data(int fd, char *rcv_buf, int data_len);
void *eide_rs485_rev(int fd);
int   eide_rs485_write_data(int fd, char *send_buf, int data_len);

#endif // !__EIDE_RS485_H