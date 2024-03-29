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
 * @Date         : 2023-03-18 10:24:11
 * @LastEditors  : MCD
 * @LastEditTime : 2023-03-22 15:53:33
 * @FilePath     : /My_C_Test/Hope_Work/leshi/include/ls_bt_cmd_dispatch.h
 * @Description  : 
 * 
 * ******************************************
 */

#ifndef _LS_BT_CMD_DISPATCH
#define _LS_BT_CMD_DISPATCH

#include "ls_bt_mesh.h"
#include "queue.h"
#include "ls_bt_mesh_dev.h"

typedef int (*command_callback)(const uint8_t *data, uint16_t cdata_len);

typedef struct
{
    uint8_t cmd_id;
    command_callback cmd_cbk;
} ls_command_list_t;

int ls_bt_back_cmd_dispatch(const uint8_t *data);

#endif  // !_LS_BT_CMD_DISPATCH