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
 * @LastEditTime : 2022-03-03 16:05:16
 * @FilePath     : /epoll_serials/priority_queue.c
 * @Description  : 
 * 
 * ******************************************
 */

#include "priority_queue.h"
#include <stdlib.h>
#include <string.h>

/**
* @author  		MCD
* @date  		2022-02-25-13:59
* @details		exch
*/
static void _exch(es_pq_t *es_pq, size_t i, size_t j)
{
    void *tmp = es_pq->pq[i];

    es_pq->pq[i] = es_pq->pq[j];
    es_pq->pq[j] = tmp;
}

/**
* @author  		MCD
* @date  		2022-02-25-13:59
* @details		swim
*/
static void _swim(es_pq_t *es_pq, size_t k)
{
    while (k > 1 && es_pq->comp(es_pq->pq[k], es_pq->pq[k / 2])) {
        _exch(es_pq, k, k / 2);
        k /= 2;
    }
}

/**
* @author  		MCD
* @date  		2022-02-25-14:02
* @details		_sink
*/
static int _sink(es_pq_t *es_pq, size_t k)
{
    size_t j;
    size_t nalloc = es_pq->nalloc;

    while ((k << 1) <= nalloc) {
        j = (k << 1);
        if ((j < nalloc) && (es_pq->comp(es_pq->pq[j + 1], es_pq->pq[j])))
            j++;

        if (!es_pq->comp(es_pq->pq[j], es_pq->pq[k]))
            break;

        _exch(es_pq, j, k);
        k = j;
    }
    return k;
}

/**
* @author  		MCD
* @date  		2022-02-25-14:11
* @details		pq init
*/
int es_pq_init(es_pq_t *es_pq, es_pq_comparator_pt comp, size_t size)
{
    // 为es_pq_t节点的pq分配(void *)指针
    es_pq->pq = (void **)calloc(size + 1, sizeof(void *));
    if (!es_pq->pq)
        return -1;

    es_pq->nalloc = 0;
    es_pq->size = size;
    es_pq->comp = comp;

    return 0;
}

/**
* @author  		MCD
* @date  		2022-02-25-14:12
* @details		is empty
*/
int es_pq_is_empty(es_pq_t *es_pq)
{
    if (!es_pq)
        return -1;
    // 通过nalloc的值 快速判断是否为空
    return (es_pq->nalloc == 0) ? 1 : 0;
}

/**
* @author  		MCD
* @date  		2022-02-25-14:12
* @details		get size
*/
size_t es_pq_size(es_pq_t *es_pq)
{
    // 获取优先队列的大小
    return es_pq->nalloc;
}

/**
* @author  		MCD
* @date  		2022-02-25-14:12
* @details		min
*/
void *es_pq_min(es_pq_t *es_pq)
{
    // 优先队列最小值直接返回第一个元素（指针）
    if (es_pq_is_empty(es_pq))
        return (void *)(-1);

    return es_pq->pq[1];
}

/**
* @author  		MCD
* @date  		2022-02-25-14:51
* @details		resize
*/
static int _resize(es_pq_t *es_pq, size_t new_size)
{
    if (new_size <= es_pq->nalloc) {
        ES_DEBUG_ERROR("new size = %ld, nalloc = %ld", new_size, es_pq->nalloc);
        return -1;
    }

    void **new_ptr = (void **)calloc(new_size, sizeof(void *));
    if (!new_ptr) {
        ES_DEBUG_ERROR("calloc new ptr failed");
        return -1;
    }

    // 将原来的nalloc + 1个元素拷贝到new_ptr指向的位置
    memcpy(new_ptr, es_pq->pq, es_pq->nalloc);
    // 释放原来的
    free(es_pq->pq);

    // 重新改写优先队列元素pq指针为new_ptr
    es_pq->pq = new_ptr;
    es_pq->size = new_size;

    return 0;
}

/**
* @author  		MCD
* @date  		2022-02-25-14:12
* @details		delete min
*/
int es_pq_delmin(es_pq_t *es_pq)
{
    if (es_pq_is_empty(es_pq)) {
        ES_DEBUG_ERROR("es pq is empty");
        return 0;
    }

    _exch(es_pq, 1, es_pq->nalloc);
    --es_pq->nalloc;
    _sink(es_pq, 1);
    if ((es_pq->nalloc > 0) && (es_pq->nalloc <= (es_pq->size - 1) / 4)) {
        if (_resize(es_pq, es_pq->size / 2) < 0)
            return -1;
    }

    return 0;
}

/**
* @author  		MCD
* @date  		2022-02-25-14:13
* @details		insert
*/
int es_pq_insert(es_pq_t *es_pq, void *item)
{
    if (es_pq->nalloc + 1 == es_pq->size) {
        if (_resize(es_pq, es_pq->size * 2) < 0)
            return -1;
    }

    es_pq->pq[++es_pq->nalloc] = item;
    _swim(es_pq, es_pq->nalloc);

    return 0;
}

/**
* @author  		MCD
* @date  		2022-02-25-14:13
* @details		sink
*/
int es_pq_sink(es_pq_t *es_pq, size_t i)
{
    return _sink(es_pq, i);
}