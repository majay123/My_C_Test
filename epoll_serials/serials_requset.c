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
 * @Date         : 2022-02-28 16:29:57
 * @LastEditors  : MCD
 * @LastEditTime : 2022-03-01 10:12:48
 * @FilePath     : /My_C_Test/epoll_serials/serials_requset.c
 * @Description  : 
 * 
 * ******************************************
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "serials_requset.h"

/**
* @author  		MCD
* @date  		2022-03-01-09:43
* @details		init es_serial_request_t
*/
int es_init_serial_request_t(es_serial_request_t *request, int fd, int epoll_fd, char *path)
{
    // 初始化es_serial_request_t
    request->fd = fd;
    request->epoll_fd = epoll_fd;
    request->root = path;
    INIT_LIST_HEAD(&(request->list));

    return 0;
}

/**
* @author  		MCD
* @date  		2022-03-01-10:12
* @details		close fd and free request
*/
int es_serial_close_conn(es_serial_request_t *request)
{
    if(request->fd > 0)
        close(request->fd);
    if(request)
        free(request);
    return 0;
}