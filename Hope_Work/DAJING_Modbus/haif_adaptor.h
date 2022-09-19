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
 * @Date         : 2022-08-24 13:54:46
 * @LastEditors  : MCD
 * @LastEditTime : 2022-08-25 09:21:28
 * @FilePath     : /My_C_Test/DAJING_Modbus/haif_ adaptor.h
 * @Description  : HAI/F(Home Automation Interface) 家庭自动化接口
 * @Description  : 一个HA I/F适配器最多可以连接64台室内和10台室外机 
 *
 * ******************************************
 */

#ifndef _HAIF_ADAPTOR_H
#define _HAIF_ADAPTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#define MAX_INDOOR_MACHINE_SIZE  (64)
#define MAX_OUTDOOR_MACHINE_SIZE (64)

#define ON  (1)
#define OFF (0)

/* Function code */
#define READ_INPUT_REGISTER      (0x04)
#define PRESET_SINGLE_REGISTER   (0x06)
#define PRESET_MULTIPLE_REGISTER (0x16)

/* Exception code */
/* 响应的功能码是在原始功能码的基础上加0x80 */
/* EXCEPTION_CODE = FUNCTION_CODE + 0x80 */


/* 适配器状态 */
typedef enum {
    ADAPTOR_NOT_READY = 0,
    ADAPTOR_BE_READY,
}ADAPTOR_STA_T;

/* 室内机连接状态 */
typedef enum {
    INDOOR_MACH_NOT_CONNECTED = 0,
    INDOOR_MACH_BE_CONNECTED,
}INDOOR_MACH_CONT_STA_T;

/* 室内机通信状态 */
typedef enum {
    INDOOR_MACH_COMM_OK = 0,
    INDOOR_MACH_COMM_ERROR,
}INDOOR_MACH_COMM_STA_T;

typedef struct HAIF_ADAPTOR_Type {
    uint8_t adaptor_slave_address;                              /* 适配器从机地址 */
    uint16_t adaptor_sta;                                       /* 适配器状态 */
    uint8_t inmachine_con[MAX_INDOOR_MACHINE_SIZE];             /* 室内机的连接状态 */
    uint8_t inmachine_comm[MAX_INDOOR_MACHINE_SIZE];            /* 室内机的通信状态 */
} HAIF_ADAPTOR_Type;

#endif // !_HAIF_ADAPTOR_H