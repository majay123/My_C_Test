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
 * @Date         : 2023-03-21 09:59:50
 * @LastEditors  : MCD
 * @LastEditTime : 2023-03-21 10:12:42
 * @FilePath     : /My_C_Test/Hope_Work/leshi/include/app_context.h
 * @Description  : 
 * 
 * ******************************************
 */

#ifndef __APP_CONTEXT_H_
#define __APP_CONTEXT_H_

#ifdef __cplusplus
extern "C" {
#endif

// #include "dmessage.pb-c.h"
#include <linux/limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int _cl_baud;
    char *_cl_port;
    char *_cl_parity;
    int _cl_bitwidth;
    int _cl_stopbit;
    int src;  //message source direction
    pthread_mutex_t logFileMutex;
    char app_log_file[PATH_MAX];
    char verbose;  // verbose debug info output control
} appContext_t;

#ifdef __cplusplus
}
#endif

#endif /*__APP_CONTEXT_H_*/
