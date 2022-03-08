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
 * @LastEditTime : 2022-03-08 11:01:30
 * @FilePath     : /My_C_Test/epoll_serials/threadpool.c
 * @Description  : 
 * 
 * ******************************************
 */

#include "threadpool.h"

static int _threadpool_free(es_threadpool_t *pool);
static void *_threadpool_worker(void *arg);

/**
* @author  		MCD
* @date  		2022-02-25-10:41
* @details		threadpool free
*/
static int _threadpool_free(es_threadpool_t *pool)
{
    if (pool == NULL || pool->started > 0) {
        ES_DEBUG_ERROR("pool is not or pool start = %d", pool->started);
        return -1;
    }

    // 释放线程数组
    if (pool->threads)
        free(pool->threads);

    // 逐节点销毁task链表
    es_task_t *old;
    while (pool->head->next) {
        old = pool->head->next;
        pool->head->next = pool->head->next->next;
        free(old);
    }
    return 0;
}

/**
* @author  		MCD
* @date  		2022-02-25-11:04
* @details		threadpool worker
*/
static void *_threadpool_worker(void *arg)
{
    if (arg == NULL) {
        ES_DEBUG_ERROR("arg is NULL");
        return NULL;
    }

    es_threadpool_t *pool = (es_threadpool_t *)arg;
    es_task_t *task;
    while (1) {
        // 线程上锁
        pthread_mutex_lock(&(pool->lock));

        // 没有task且未停机阻塞
        while ((pool->queue_size == 0) && !(pool->shutdown))
            pthread_cond_wait(&(pool->cond), &(pool->lock));
        if(pool->queue_size > 0)
            // printf("Queue size: %d\n", pool->queue_size);

        // 立即停机模式，平滑停机且没有未完成任务则退出
        if (pool->shutdown == immediate_shutdown)
            break;
        else if ((pool->shutdown == graceful_shutdown) && (pool->queue_size == 0))
            break;

        // 得到第一个task
        task = pool->head->next;
        // 没有task则开锁并进行下一次循环
        if (task == NULL) {
            pthread_mutex_unlock(&(pool->lock));
            continue;
        }

        // 存放task则取走并开锁
        pool->head->next = task->next;
        pool->queue_size--;
        pthread_mutex_unlock(&(pool->lock));

        // 设置task中func参数
        (*(task->func))(task->arg);
        ES_DEBUG_INFO("free task");
        printf("free task\n");
        free(task);
    }
    pool->started--;
    pthread_mutex_unlock(&(pool->lock));
    pthread_exit(NULL);
    return NULL;
}

/**
* @author  		MCD
* @date  		2022-02-24-14:04
* @details		threadpool init
*/
es_threadpool_t *threadpool_init(int thread_num)
{
    es_threadpool_t *pool;
    int i = 0;

    if ((pool = (es_threadpool_t *)calloc(1, sizeof(es_threadpool_t))) == NULL) {
        ES_DEBUG_ERROR("thread pool calloc failed");
        goto err;
    }

    // threads指针指向线程数组（存放tid），数组大小即为线程数量
    pool->thread_count = 0;
    pool->queue_size = 0;
    pool->shutdown = 0;
    pool->started = 0;
    pool->threads = (pthread_t *)calloc(thread_num, sizeof(pthread_t));

    // 分配并初始化task头节点
    pool->head = (es_task_t *)calloc(1, sizeof(es_task_t));
    if ((pool->threads == NULL) || (pool->head == NULL)) {
        ES_DEBUG_ERROR("calloc threads or head failed");
        goto err;
    }
    pool->head->func = NULL;
    pool->head->arg = NULL;
    pool->head->next = NULL;

    // 初始化互斥锁
    if (pthread_mutex_init(&(pool->lock), NULL) != 0) {
        ES_DEBUG_ERROR("pthread mutex init failed");
        goto err;
    }

    // 初始化条件变量
    if (pthread_cond_init(&(pool->cond), NULL) != 0) {
        ES_DEBUG_ERROR("pthread cond init failed");
        goto err;
    }

    // 创建线程
    for (i = 0; i < thread_num; i++) {
        /* code */
        if (pthread_create(&(pool->threads[i]), NULL, _threadpool_worker, (void *)pool) != 0) {
            ES_DEBUG_ERROR("pthread_create failed");
            threadpool_destroy(pool, 0);
            return NULL;
        }
        pool->thread_count++;
        pool->started++;
    }
    return pool;

err:
    if (pool)
        _threadpool_free(pool);
    return NULL;
}

/**
* @author  		MCD
* @date  		2022-02-24-14:04
* @details		threadpoll add
*/
int threadpool_add(es_threadpool_t *pool, void (*func)(void *), void *arg)
{
    int err = 0;

    if ((pool == NULL) || (func == NULL)) {
        ES_DEBUG_ERROR("pool or func is NULL");
        return -1;
    }

    if (pthread_mutex_lock(&(pool->lock)) != 0) {
        ES_DEBUG_ERROR("pthread mutex lock failed");
        return -1;
    }

    if (pool->shutdown) {
        err = es_tp_already_shutdown;
        ES_DEBUG_WARN("threadpool is already shutdown");
        goto out;
    }

    // 新建task并注册消息
    es_task_t *task = (es_task_t *)calloc(1, sizeof(es_task_t));
    if (task == NULL) {
        ES_DEBUG_ERROR("calloc task failed");
        goto out;
    }
    task->func = func;
    task->arg = arg;

    // 新task节点再head插入
    task->next = pool->head->next;
    pool->head->next = task;
    pool->queue_size++;
    // printf("queue size: %d\n", pool->queue_size);
    pthread_cond_signal(&(pool->cond));

out:
    if (pthread_mutex_unlock(&(pool->lock)) != 0) {
        ES_DEBUG_ERROR("pthread mutex unlock failed");
        return -1;
    }
    return err;
}

/**
* @author  		MCD
* @date  		2022-02-24-14:04
* @details		threadpool destroy
*/
int threadpool_destroy(es_threadpool_t *pool, int graceful)
{
    if (pool == NULL) {
        ES_DEBUG_ERROR("pool NULL");
        return es_tp_invalid;
    }
    if (pthread_mutex_lock(&(pool->lock)) != 0) {
        ES_DEBUG_ERROR("pthrad mutex lock failed");
        return es_tp_lock_fail;
    }

    int i, err = 0;
    do {
        if (pool->shutdown) {
            err = es_tp_already_shutdown;
            break;
        }

        pool->shutdown = (graceful) ? graceful_shutdown : immediate_shutdown;

        if (pthread_cond_broadcast(&(pool->cond)) != 0) {
            err = es_tp_cond_broadcast;
            break;
        }

        if (pthread_mutex_unlock(&(pool->lock)) != 0) {
            err = es_tp_lock_fail;
            break;
        }

        // 回收每个线程资源
        for (i = 0; i < pool->thread_count; i++) {
            /* code */
            if (pthread_join(pool->threads[i], NULL) != 0) {
                err = es_tp_thread_fail;
            }
        }
    } while (0);

    if (!err) {
        pthread_mutex_destroy(&(pool->lock));
        pthread_cond_destroy(&(pool->cond));
        _threadpool_free(pool);
    }

    return 0;
}