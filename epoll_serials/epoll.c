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
 * @Date         : 2022-02-24 10:26:02
 * @LastEditors  : MCD
 * @LastEditTime : 2022-02-25 09:01:01
 * @FilePath     : /My_C_Test/epoll_serials/epoll.c
 * @Description  : 
 * 
 * ******************************************
 */

#include "epoll.h"

struct epoll_event *events;

/**
* @author  		MCD
* @date  		2022-02-24-12:40
* @details		create epoll
*/
int es_epoll_create(int flags)
{
    int epoll_fd = epoll_create(flags);
    if (epoll_fd < 0) {
        return -1;
    }
    events = (struct epoll_event *)calloc(MAXEVENTS, sizeof(struct epoll_event));
    if (events != NULL)
        return epoll_fd;
    else
        return -1;
}

/**
* @author  		MCD
* @date  		2022-02-24-14:00
* @details		epoll add
*/
int es_epoll_add(int epoll_fd, int fd, void *arg, int events)
{
    return 0;
}

/**
* @author  		MCD
* @date  		2022-02-24-14:00
* @details		epoll mod
*/
int es_epoll_mod(int epoll_fd, int fd, void *arg, int events)
{
    return 0;
}

/**
* @author  		MCD
* @date  		2022-02-24-14:00
* @details		epoll del
*/
int es_epoll_del(int epoll_fd, int fd, void *arg, int events)
{
    return 0;
}

/**
* @author  		MCD
* @date  		2022-02-24-14:01
* @details		epoll wait
*/
int es_epoll_wait(int epoll_fd, struct epoll_event *events, int max_events, int timeout)
{
    return 0;
}

/**
* @author  		MCD
* @date  		2022-02-24-14:01
* @details		handle event
*/
void es_handle_event(int epoll_fd, int fd, struct epoll_event *events, int events_num, char *path, es_threadpool_t *tp)
{
    return;
}
