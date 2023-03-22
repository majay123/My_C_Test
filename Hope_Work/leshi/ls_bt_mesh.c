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
 * @Date         : 2023-03-16 16:36:00
 * @LastEditors  : MCD
 * @LastEditTime : 2023-03-22 15:56:44
 * @FilePath     : /My_C_Test/Hope_Work/leshi/ls_bt_mesh.c
 * @Description  : 
 * 
 * ******************************************
 */
#include <fcntl.h>
#include <getopt.h>  //for getopt_long
#include <libubox/uloop.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/termios.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "app_context.h"
#include "common.h"
#include "debug.h"
#include "ls_bt_mesh.h"
#include "ls_bt_mesh_dev.h"
#include "ls_bt_cmd_dispatch.h"
#include "queue.h"


/**
* @details  	
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-22-15:50 V1.0.0				MCD				create
*/
int ls_process_cmd_back_send(int fd, llq_t *llq_handle)
{
    int ret = -1;
    ls_mesh_dev_data_t send_msg;

    memset(&send_msg, 0, sizeof(ls_mesh_dev_data_t));
    ret = llq_receive(llq_handle, (char *)&send_msg, sizeof(ls_mesh_dev_data_t));
    // sleep(1);
    if (ret > 0 && (send_msg.data_len > 0)) {
        if (appContext.verbose) {
            // dump_rs485_data((uint8_t *)send_msg.data, send_msg.data_len);
            uint8_t *dump = dump_rs485_data1((uint8_t *)send_msg.data, send_msg.data_len);
            if (dump) {
                DEBUG_INFO("dump data:%s\n", dump);
                free(dump);
            }
        }

        if (ls_bt_mesh_dev_write_data(fd, send_msg.data, send_msg.data_len) < 0)
            DEBUG_ERROR("eide rs485 send failed\n");
        free(send_msg.data);
        select_usleep(1000 * 200);
    }

    return ret;
}

/**
* @details  	处理bt mesh接收到的队列数据
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-22-09:30 V1.0.0				MCD				create
*/
int ls_bt_mesh_dispatch(llq_t *llq_handle)
{
    int ret = 0;
    ls_mesh_dev_data_t recv_msg;

    memset(&recv_msg, 0, sizeof(ls_mesh_dev_data_t));
    ret = llq_receive(llq_handle, (char *)&recv_msg, sizeof(ls_mesh_dev_data_t));
    if(ret > 0 && (recv_msg.data_len > 0) && (recv_msg.data != NULL))
    {
        if(appContext.verbose) {
            uint8_t *dump = dump_rs485_data1((uint8_t *)recv_msg.data, recv_msg.data_len);
            if (dump) {
                DEBUG_INFO("dump data:%s\n", dump);
                free(dump);
            }
        }
        ls_bt_back_cmd_dispatch((const uint8_t *)recv_msg.data);
        free(recv_msg.data);
    }

    return ret;
}