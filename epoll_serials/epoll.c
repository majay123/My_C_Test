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
 * @LastEditTime : 2022-03-03 16:33:29
 * @FilePath     : /epoll_serials/home/mcd/MyWorkSpace/MyGithub_Project/My_C_Test/epoll_serials/epoll.c
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
    int epoll_fd = epoll_create1(flags);
    if (epoll_fd < 0) {
        ES_DEBUG_ERROR("epoll create failed: %d", epoll_fd);
        return -1;
    }
    events = (struct epoll_event *)calloc(MAXEVENTS, sizeof(struct epoll_event));
    if (events != NULL) 
        return epoll_fd;
    else {
        ES_DEBUG_ERROR("calloc events failed!");
        return -1;
    }
}

/**
* @author  		MCD
* @date  		2022-02-24-14:00
* @details		epoll add
*/
int es_epoll_add(int epoll_fd, int fd, void *arg, int events)
{
    struct epoll_event event;
    memset(&event, 0, sizeof(struct epoll_event));

    event.data.ptr = arg;
    event.events = events;

    int ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);

    return ret;
}

/**
* @author  		MCD
* @date  		2022-02-24-14:00
* @details		epoll mod
*/
int es_epoll_mod(int epoll_fd, int fd, void *arg, int events)
{
    struct epoll_event event;
    memset(&event, 0, sizeof(struct epoll_event));

    event.data.ptr = arg;
    event.events = events;

    int ret = epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &event);

    return ret;
}

/**
* @author  		MCD
* @date  		2022-02-24-14:00
* @details		epoll del
*/
int es_epoll_del(int epoll_fd, int fd, void *arg, int events)
{
    struct epoll_event event;
    memset(&event, 0, sizeof(struct epoll_event));

    event.data.ptr = arg;
    event.events = events;

    int ret = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &event);

    return ret;
}

/**
* @author  		MCD
* @date  		2022-02-24-14:01
* @details		epoll wait
*/
int es_epoll_wait(int epoll_fd, struct epoll_event *events, int max_events, int timeout)
{
    int ret_count = epoll_wait(epoll_fd, events, max_events, timeout);
    return ret_count;
}


void es_handle_events(int epoll_fd, struct epoll_event *events, int events_num, void (*func)(void *), es_threadpool_t *tp)
{
    int i = 0;

    for (i = 0; i < events_num; i++) {
        // printf("events_num = %d\n", events_num);
        // 获取有事件产生的描述符
        es_serial_request_t *request = (es_serial_request_t *)(events[i].data.ptr);
        // int fd = request->fd;
        // 发生错误或者文件挂断
        if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!events[i].events & EPOLLIN)) {
            // close(fd);
            ES_DEBUG_ERROR("epoll error: %d", events[i].events);
            continue;
        }

        // 将请求任务加入到线程池
        // printf("add thread pool\n");
        threadpool_add(tp, func, events[i].data.ptr);
    }
}
