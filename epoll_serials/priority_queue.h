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
 * @Date         : 2022-02-24 10:30:00
 * @LastEditors  : MCD
 * @LastEditTime : 2022-02-28 16:02:51
 * @FilePath     : /My_C_Test/epoll_serials/priority_queue.h
 * @Description  : 
 * 
 * ******************************************
 */

#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "es_debug.h"
#include <stdlib.h>

#define ES_PQ_DEFAULT_SIZE (10)

typedef int (*es_pq_comparator_pt)(void *pi, void *pj);

typedef struct priority_queue {
    void **pq;                // 优先队列节点指针
    size_t nalloc;            // 优先队列实际元素个数
    size_t size;              // 优先队列大小
    es_pq_comparator_pt comp; // 堆模式
} es_pq_t;

int es_pq_init(es_pq_t *es_pq, es_pq_comparator_pt comp, size_t size);
int es_pq_is_empty(es_pq_t *es_pq);
size_t es_pq_size(es_pq_t *es_pq);
void *es_pq_min(es_pq_t *es_pq);
int es_pq_delmin(es_pq_t *es_pq);
int es_pq_insert(es_pq_t *es_pq, void *item);
int es_pq_sink(es_pq_t *es_pq, size_t i);

#endif // !PRIORITY_QUEUE_H