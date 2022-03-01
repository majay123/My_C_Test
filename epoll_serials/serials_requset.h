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
 * @Date         : 2022-02-28 14:53:37
 * @LastEditors  : MCD
 * @LastEditTime : 2022-03-01 11:16:10
 * @FilePath     : /My_C_Test/epoll_serials/serials_requset.h
 * @Description  : 
 * 
 * ******************************************
 */

#ifndef SERIALS_REQUEST_H
#define SERIALS_REQUEST_H

#include "list.h"
#include "util.h"
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUF (1024 * 8)

typedef struct es_serial_request {
    char *root;
    void *timer;
    int epoll_fd;
    int fd;
    struct list_head list;  // 存储请求头，list.h中有
} es_serial_request_t;

int es_init_serial_request_t(es_serial_request_t *request, int fd, int epoll_fd, char *path);
int es_serial_close_conn(es_serial_request_t *request);

#endif // !_SERIALS_REQUEST_H