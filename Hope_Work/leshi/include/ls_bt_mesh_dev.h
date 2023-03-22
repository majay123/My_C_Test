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
 * @Date         : 2023-03-21 13:07:47
 * @LastEditors  : MCD
 * @LastEditTime : 2023-03-22 15:41:58
 * @FilePath     : /My_C_Test/Hope_Work/leshi/include/ls_bt_mesh_dev.h
 * @Description  : 
 * 
 * ******************************************
 */
#ifndef _LS_BT_MESH_DEV_H
#define _LS_BT_MESH_DEV_H

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

#include "app_context.h"
#include "common.h"
#include "debug.h"
#include "ls_bt_mesh.h"
#include "queue.h"

typedef struct{
    char *data; 
    int data_len;
}ls_mesh_dev_data_t;

void ls_bt_mesh_dev_rev(int fd, llq_t *hndl);
int ls_bt_mesh_dev_write_data(int fd, char *send_buf, int data_len);
void ls_bt_mesh_dev_close(int fd);

#endif  // !_LS_BT_MESH_DEV_H
