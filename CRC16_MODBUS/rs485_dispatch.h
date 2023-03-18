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
 * @Date         : 2021-08-24 13:33:51
 * @LastEditors  : MCD
 * @LastEditTime : 2022-09-16 16:08:12
 * @FilePath     : /My_C_Test/CRC16_MODBUS/rs485_dispatch.h
 * @Description  :
 *
 * ******************************************
 */

#ifndef __RS485_DISPATCH_H
#define __RS485_DISPATCH_H

// #include "queue.h"
#include "common.h"

typedef struct {
    char  ctrl_dev_count;
    char  device_id;
    char  dev_func_num;
    char  func_id;
    char  data_type;
    char *data;
} eide_rs485_ctrl_t;

int eide_rs485_dispatch_msg(char *data, int datalen);
int eide_rs485_get_module_info(void);
// int eide_rs485_simple_control(llq_t *llq_handle, rs485_data_t *data, int
// datalen);

#endif // !_RS485_DISPATCH_H