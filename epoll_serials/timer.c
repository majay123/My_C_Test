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
 * @LastEditTime : 2022-02-28 16:03:34
 * @FilePath     : /My_C_Test/epoll_serials/timer.c
 * @Description  : 
 * 
 * ******************************************
 */

#include "timer.h"
#include <sys/time.h>

es_pq_t es_timer;
size_t es_current_msec;

/**
* @author  		MCD
* @date  		2022-02-25-15:32
* @details		timer comp
*/
int timer_comp(void *ti, void *tj)
{
    es_timer_t *timeri = (es_timer_t *)ti;
    es_timer_t *timerj = (es_timer_t *)tj;

    return (timeri->key < timerj->key) ? 1 : 0;
}

/**
* @author  		MCD
* @date  		2022-02-25-15:33
* @details		time update
*/
void es_time_update(void)
{
    // 获取当前时间
    struct timeval tv;

    int rc = gettimeofday(&tv, NULL);
    es_current_msec = ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/**
* @author  		MCD
* @date  		2022-02-25-15:34
* @details		timer init
*/
int es_timer_init(void)
{
    // 建立连接后立即初始化
    // 初始化优先队列大小ES_PQ_DEFAULT_SIZE = 10
    int rc = es_pq_init(&es_timer, timer_comp, ES_PQ_DEFAULT_SIZE);

    // 更新当前时间
    es_time_update();

    return 0;
}

/**
* @author  		MCD
* @date  		2022-02-25-15:34
* @details		find timer
*/
int es_find_timer(void)
{
    int time;
    // 返回队列中最早时间和当前时间之差
    while (!es_pq_is_empty(&es_timer)) {
        // 更新当前时间
        es_time_update();
        // timer_node指向最小的时间
        es_timer_t *timer_node = (es_timer_t *)es_pq_min(&es_timer);
        if (timer_node == NULL) {
            ES_DEBUG_ERROR("timer node is NULL");
            return 0;
        }
        // 如果已删，则释放此节点，（es_del_timer只位置不删除）
        if (timer_node->deleted) {
            int rc = es_pq_delmin(&es_timer);
            free(timer_node);
            continue;
        }
        // 此时timer_node为时间最小节点，time为优先队列里最小时间剪去当前时间
        time = (int)(timer_node->key - es_current_msec);
        time = (time > 0) ? time : 0;
        break;
    }
    return time;
}

/**
* @author  		MCD
* @date  		2022-02-25-15:34
* @details		handle expire timers
*/
void es_handle_expire_timers(void)
{
    while (!es_pq_is_empty(&es_timer)) {
        // 更新当前时间
        es_time_update();
        es_timer_t *timer_node = (es_timer_t *)es_pq_min(&es_timer);
        if (timer_node == NULL) {
            ES_DEBUG_ERROR("timer node is NULL");
            return;
        }
        // 如果已删，则释放此节点，（es_del_timer只位置不删除）
        if (timer_node->deleted) {
            int rc = es_pq_delmin(&es_timer);
            free(timer_node);
            continue;
        }
        // 最早如队列节点超时时间大于当前时间（未超时）
        // 结束超时检查，顺带删除了下标记为删除的节点
        if (timer_node->key > es_current_msec)
            return;
        // 出现了没被删除但是超时的情况，调用handler处理
        if (timer_node->handler)
            timer_node->handler(timer_node->arg);

        int rc = es_pq_delmin(&es_timer);
        free(timer_node);
    }
}

/**
* @author  		MCD
* @date  		2022-02-25-15:35
* @details		add tiemr
*/
void es_add_timer(void *arg, size_t timeout, timer_handler_pt handler)
{
    es_time_update();
    // 申请新的es_timer_t节点，并加入到串口的tiemr下
    es_timer_t *timer_node = (es_timer_t *)calloc(1, sizeof(es_timer_t));
    es_serial_request_t *req = (es_serial_request_t *)arg;
    req->timer = timer_node;
    timer_node->key = es_current_msec + timeout;
    timer_node->deleted = 0;
    timer_node->handler = handler;
    // 注意 需要在es_timer_t节点中反向设置指向对应的request的指针
    timer_node->arg = arg;

    // 将节点插入优先队列
    int rc = es_pq_insert(&es_timer, timer_node);
}

/**
* @author  		MCD
* @date  		2022-02-25-15:35
* @details		delete timer
*/
void es_del_timer(void *arg)
{
    es_time_update();
    es_serial_request_t *req = (es_serial_request_t *)arg;
    es_timer_t *timer_node = (es_timer_t *)req->timer;

    // 惰性删除
    // 标记为已删，在find_timer和handle_expire_timers检查队列时会删除
    timer_node->deleted = 1;
}
