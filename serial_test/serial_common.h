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
 * ************Copyright 2022 MCD************
 * 
 * @version      : 
 * @Company      : HOPE
 * @Author       : MCD
 * @Date         : 2022-03-03 09:38:17
 * @LastEditors  : MCD
 * @LastEditTime : 2022-11-03 10:11:58
 * @FilePath     : /My_C_Test/serial_test/serial_common.h
 * @Description  : 
 * 
 * ******************************************
 */

#ifndef SERIAL_COMMON_H
#define SERIAL_COMMON_H

// #include "es_debug.h"
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>  //for getopt_long
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/termios.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

/* Timeouts in microsecond (0.5 s) */
#define _RESPONSE_TIMEOUT             (500000)
#define _RESPONSE_1S_TIMEOUT          (1000)
#define _BYTE_TIMEOUT                 (500000)
#define MAISI_HEADER_LENGTH           (1)
#define MAISI_CHECKSUM_LENGTH         (1)
#define MAISI_MAX_BINDING_DEVS        (32)
#define MAISI_AC_CTRL_TEMPERATURE_MIN (0x10)
#define MAISI_AC_CTRL_TEMPERATURE_MAX (0x1E)
#define MAISI_FH_CTRL_TEMPERATURE_MIN (0x05)
#define MAISI_FH_CTRL_TEMPERATURE_MAX (0x5A)
#define MAISI_DEV_INFO_DATA_LENGTH    (0x0A)
#define MAISI_QUERY_DEV_STATUS_ONE    (0x01)
#define MAISI_QUERY_DEV_STATUS_MANY   (0x0F)
#define MAISI_QUERY_DEV_STATUS_ALL    (0xFF)
#define MAISI_CTRL_CODE_INDEX         (0x02)
#define MAISI_DEV_NUMBER_INDEX        (0x03)
#define MAISI_MAX_ADU_LENGTH          (MAISI_MAX_BINDING_DEVS * MAISI_DEV_INFO_DATA_LENGTH + 5)

/* air conditioner control cmd code */
#define MAISI_CMD_CTRL_AIR_CONDITIONER_ONOFF          (0x31) /* 0: off, 1: on */
#define MAISI_CMD_CTRL_AIR_CONDITIONER_TEMPERATURE    (0x32) /* 设定温度 16~30℃（十进制） */
#define MAISI_CMD_CTRL_AIR_CONDITIONER_MODE           (0x33) /*  */
#define MAISI_CMD_CTRL_AIR_CONDITIONER_WIND_SPEED     (0x34)
#define MAISI_CMD_CTRL_AIR_CONDITIONER_WIND_DIRECTION (0x35)
#define MAISI_CMD_CTRL_AIR_CONDITIONER_HUMIDITY       (0x36)
#define MAISI_CMD_CTRL_AIR_CONDITIONER_CLEANLINESS    (0x37)

/* central ventilation system control cmd code */
#define MAISI_CMD_CTRL_VENTILATION_SYSTEM_ONOFF       (0x71)
#define MAISI_CMD_CTRL_VENTILATION_SYSTEM_TEMPERATURE (0x72)
#define MAISI_CMD_CTRL_VENTILATION_SYSTEM_MODE        (0x73)
#define MAISI_CMD_CTRL_VENTILATION_SYSTEM_WIND_SPEED  (0x74)

/* Floor heating control cmd code*/
#define MAISI_CMD_CTRL_FLOOR_HEATING_ONOFF                 (0x81)
#define MAISI_CMD_CTRL_FLOOR_HEATING_TEMPERATURE           (0x82)
#define MAISI_CMD_CTRL_FLOOR_HEATING_MODE                  (0x83)
#define MAISI_CMD_CTRL_FLOOR_HEATING_ANTIFREEZE_PROTECTION (0x84)

/* all device Monitoring instruction */
#define MAISI_CMD_QUERY_AIR_CONDITIONING_STATUS  (0x50)
#define MAISI_CMD_QUERY_VENTILATION_DEV_STATUS   (0x51)
#define MAISI_CMD_QUERY_FLOOR_HEATING_DEV_STATUS (0x52)

typedef enum ms_dev_type_e {
    MAISI_DEVICE_TYPE_AIR_CONDITIONS = 1,
    MAISI_DEVICE_TYPE_VENTILATION_SYSTEM,
    MAISI_DEVICE_TYPE_FLOOR_HEATING,
} ms_dev_type_e;

typedef enum ms_dev_onoff_e {
    MAISI_DEV_OFF = 0x00,
    MAISI_DEV_ON = 0x01,
} ms_dev_onoff_e;

typedef enum {
    MAISI_AC_MODE_COOL = 0x01,           /* 制冷模式 */
    MAISI_AC_MODE_DEHUMIDIFICATION,      /* 除湿模式 */
    MAISI_AC_MODE_REFRESHING,            /* 清爽模式 */
    MAISI_AC_MODE_AIR_SUPPLY,            /* 送风模式 */
    MAISI_AC_MODE_AUTO_DEHUMIDIFICATION, /* 自动除湿模式 */
    MAISI_AC_MODE_INTIMATE_SLEEP,        /* 贴心睡眠 */
    MAISI_AC_MODE_HEATING = 0x08,        /* 制热模式 */
    MAISI_AC_MODE_FLOOR_HEATING,         /* 地暖模式 */
    MAISI_AC_MODE_HOT_HEAT,              /* 强热模式（地暖和制热同时开启） */
} ms_ac_mode_e;

typedef enum {
    MAISI_WIND_SPEED_AUTO = 0x00,
    MAISI_WIND_SPEED_HIGH = 0x01,
    MAISI_WIND_SPEED_MID,
    MAISI_WIND_SPEED_MID_HIGH,
    MAISI_WIND_SPEED_LOW,
    MAISI_WIND_SPEED_MID_LOW,
    MAISI_WIND_SPEED_CLOSED,
} ms_wind_speed_e;

typedef enum {
    MAISI_VEN_MODE_AUTO = 0x00,          /* 自动模式 */
    MAISI_VEN_MODE_VENTILATE = 0x01,     /* 换气模式 */
    MAISI_VEN_MODE_EXHAUST,              /* 排风模式 */
    MAISI_VEN_MODE_AI,                   /* 智能模式 */
    MAISI_VEN_MODE_FORCEFUL,             /* 强劲模式 */
    MAISI_VEN_MODE_SLEEP,                /* 省电（睡眠）模式 */
    MAISI_VEN_MODE_AIR_SUPPLY,           /* 送风模式 */
    MAISI_VEN_MODE_BYPASS,               /* 旁通模式 */
    MAISI_VEN_MODE_QUICK_CLEAR,          /* 速净模式 */
    MAISI_VEN_MODE_COMFORTABLE,          /* 舒适模式 */
    MAISI_VEN_MODE_COOL_WIND,            /* 凉风模式 */
    MAISI_VEN_MODE_MANUAL,               /* 手动模式 */
    MAISI_VEN_MODE_MUTE,                 /* 静音模式 */
    MAISI_VEN_MODE_FRESH_WIND,           /* 新风模式 */
    MAISI_VEN_MODE_COOL,                 /* 制冷模式 */
    MAISI_VEN_MODE_HEATING,              /* 制热模式 */
    MAISI_VEN_MODE_DEHUMIDIFICATION,     /* 除湿模式 */
    MAISI_VEN_MODE_HOT_EXCHANGE,         /* 热交换模式 */
    MAISI_VEN_MODE_INTERNAL_CIRCULATION, /* 内循环 */
    MAISI_VEN_MODE_EXTERNAL_CIRCULATION, /* 外循环 */
    MAISI_VEN_MODE_MIX,                  /* 混风模式（内外循环同时打开） */
    MAISI_VEN_MODE_CLOSED,               /* 关闭模式 */
} ms_ven_mode_e;

typedef enum {
    /* Request message on the server side */
    MSG_INDICATION,
    /* Request message on the client side */
    MSG_CONFIRMATION
} msg_type_t;

typedef struct ms_rs485_t {
    /* Device: "/dev/ttyS0", "/dev/ttyUSB0" or "/dev/tty.USA19*" on Mac OS X. */
    char *device;
    /* Bauds: 9600, 19200, 57600, 115200, etc */
    int baud;
    /* Data bit */
    uint8_t data_bit;
    /* Stop bit */
    uint8_t stop_bit;
    /* Parity: 'N', 'O', 'E' */
    char parity;
    /* Save old termios settings */
    struct termios old_tios;
    /* To handle many slaves on the same link */
    int confirmation_to_ignore;
} ms_rs485_t;

#define DEV_PATH ("/dev/ttyS0")

int serial_init_open(char *dev_path, int speed, int databits, int stopbits, int parity);
int serial_read_data(int fd, char *rcv_buf, int data_len);
int serial_write_data(int fd, char *send_buf, int data_len);
int maisi_rs485_dev_open(ms_rs485_t *rs485);
void maisi_rs485_dev_close(int fd);
int maisi_rs485_receive_msg(int fd, uint8_t *msg, msg_type_t msg_type);
ssize_t maisi_rs485_send(int fd, const uint8_t *req, int req_length);

#endif  // !