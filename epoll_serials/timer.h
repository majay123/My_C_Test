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
 * @Date         : 2022-02-24 10:26:58
 * @LastEditors  : MCD
 * @LastEditTime : 2022-02-28 15:24:20
 * @FilePath     : /My_C_Test/epoll_serials/timer.h
 * @Description  : 
 * 
 * ******************************************
 */

#ifndef TIMER_H
#define TIMER_H

#include "es_debug.h"
#include "priority_queue.h"
#include "serials_requset.h"

#define TIMER_DEFAULT (500) /* ms */

// 函数指针 复制超时处理， es_add_timer时指针处理函数
// 这里的通用指针数据，到时候会改为自定义的数据结构体
typedef int (*timer_handler_pt)(void *arg);

typedef struct es_timer {
    size_t key;               //标记超时时间
    int deleted;              // 标记是否被删除
    timer_handler_pt handler; // 超时处理，add时指定
    void *arg;                // 指向数据(此数据需要之后出现定义)
} es_timer_t;

// es_pq_t定义在"priority_queue.h"中，优先队列中节点
extern es_pq_t es_timer;
extern size_t es_current_msec;

int es_timer_init(void);
int es_find_timer(void);
void es_handle_expire_timers(void);
void es_add_timer(void *arg, size_t timeout, timer_handler_pt handler);
void es_del_timer(void *arg);
int timer_comp(void *ti, void *tj);

#endif // !TIMER_H