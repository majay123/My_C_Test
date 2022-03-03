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
 * @LastEditTime : 2022-03-03 16:49:21
 * @FilePath     : /epoll_serials/serials_requset.c
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


/**
* @author  		MCD
* @date  		2022-02-24-14:01
* @details		handle event
*/
// for test
void es_serial_dispatch_rs485(void *arg)
{
    // todo dispatch rs485 data
    ES_DEBUG_INFO("do dispatch rs485 data" );
    es_serial_request_t *req = (es_serial_request_t *)arg;
    char buf[1024] = {0};
#if 0
    size_t len = read(req->fd, buf, sizeof(buf));
    printf("%d, %s", len, buf);
#else
    int i;
    // int len = serial_read_data(req->fd, buf, 6);
    int len = rio_readn(req->fd, buf, 6);
    printf("read data: %d \n", len);
    if(len > 0) {
        for ( i = 0; i < 6; i++)
        {
            printf("%02x ", buf[i]);
        }
        printf("\n");
        printf("%s\n", buf);
    }
#endif
}