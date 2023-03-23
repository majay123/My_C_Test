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
 * @Date         : 2023-03-23 08:40:32
 * @LastEditors  : MCD
 * @LastEditTime : 2023-03-23 13:45:50
 * @FilePath     : /My_C_Test/Hope_Work/leshi/include/ls_bt_mesh_cmd_send.h
 * @Description  : 
 * 
 * ******************************************
 */

#ifndef LS_BT_MESH_CMD_SEND_H
#define LS_BT_MESH_CMD_SEND_H

#include "ls_bt_mesh.h"
#include "ls_bt_mesh_dev.h"

typedef int (*send_cmd_callback)(llq_t *llq_handle, leshi_cmd_e cmd, uint8_t *cdata, size_t cdata_len);

typedef struct
{
    uint8_t cmd_id;
    send_cmd_callback cmd_cbk;
} ls_send_cmd_list_t;

// int ls_bt_mesh_send_cmd_process(llq_t *llq_handle, leshi_cmd_e cmd, uint8_t *cdata, size_t cdata_size);
int ls_bt_mesh_cmd_send(llq_t *llq_handle, leshi_cmd_e cmd, uint8_t *cdata, size_t cdata_size);

#endif // !LS_BT_MESH_CMD_SEND_H