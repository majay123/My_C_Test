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
 * @LastEditTime : 2022-02-24 14:04:18
 * @FilePath     : /My_C_Test/epoll_serials/threadpool.h
 * @Description  : 
 * 
 * ******************************************
 */

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct es_task {
    void (*func)(void *);
    void *arg;
    struct es_task *next; // 任务链表 （下一个节点指针）
} es_task_t;

typedef struct threadpool {
    pthread_mutex_t lock; // 互斥锁
    pthread_cond_t cond;  // 条件变量
    pthread_t *threads;   // 线程
    es_task_t *head;      // 任务链表
    int thread_count;     // 线程数量
    int queue_size;       // 任务链表长
    int shutdown;         // 关机模式
    int started;
} es_threadpool_t;

typedef enum {
    es_tp_invalid = -1,
    es_tp_lock_fail = -2,
    es_tp_already_shutdown = -3,
    es_tp_cond_broadcast = -4,
    es_tp_thread_fail = -5,
} es_threadpool_error_t;

typedef enum {
    immediate_shutdown = 1,
    graceful_shutdown = 2,
} es_threadpool_sd_t;

es_threadpool_t *threadpool_init(int thread_num);
int threadpool_add(es_threadpool_t *pool, void (*func)(void *));
int threadpool_destroy(es_threadpool_t *pool, int graceful);

#endif // !THREADPOOL_H