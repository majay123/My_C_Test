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
 * @Date         : 2022-02-24 10:25:31
 * @LastEditors  : MCD
 * @LastEditTime : 2022-02-24 12:37:56
 * @FilePath     : /My_C_Test/epoll_serials/epoll.h
 * @Description  : 
 * 
 * ******************************************
 */

#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>
#include "threadpool.h"

#define MAXEVENTS           (100)

int es_epoll_create(int flags);
int es_epoll_add(int epoll_fd, int fd, void *arg, int events);
int es_epoll_mod(int epoll_fd, int fd, void *arg, int events);
int es_epoll_del(int epoll_fd, int fd, void *arg, int events);
int es_epoll_wait(int epoll_fd, struct epoll_event *events, int max_events, int timeout);
void es_handle_event(int epoll_fd, int fd, struct epoll_event *events, int events_num, char *path, es_threadpool_t* tp);

#endif // !EPOLL_H