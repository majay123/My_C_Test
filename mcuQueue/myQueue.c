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
 * @Date         : 2022-08-04 08:58:31
 * @LastEditors  : MCD
 * @LastEditTime : 2022-08-15 15:48:47
 * @FilePath     : /My_C_Test/mcuQueue/myQueue.c
 * @Description  : 
 * 
 * ******************************************
 */
#include "myQueue.h"

/**
 * @brief 初始化（创建）队列，每个队列必须先执行该函数才能使用。
 * 
 * @param hqueue            队列变量指针
 * @param qbuff             队列缓存区地址 
 * @param qsize             队列缓存区长度
 */
void Queue_Init(QUEUE_HandleTypeDef *hqueue, QUEUE_DATA_T *qbuff, uint32_t qsize)
{
    memset(hqueue, 0, sizeof(QUEUE_HandleTypeDef));
    hqueue->qbuff = qbuff;
    hqueue->queue_size = qsize;
    hqueue->queue_cnt = 0;
}

/**
 * @brief 清空队列
 * 
 * @param hqueue            队列变量指针
 */
void Queue_Clear(QUEUE_HandleTypeDef *hqueue)
{
    if(hqueue->qbuff) 
        memset(hqueue->qbuff, 0, hqueue->queue_size);
    hqueue->head = 0;
    hqueue->tail = 0;
    hqueue->queue_cnt = 0;
}

/**
 * @brief 获取队列内数据的个数
 * 
 * @param hqueue            队列变量指针
 * @return uint32_t         队列内数据的个数 
 */
uint32_t Queue_Count(QUEUE_HandleTypeDef *hqueue)
{
    return hqueue->queue_cnt;
}

/**
 * @brief 压入数据到队列中
 * 
 * @param hqueue            队列变量指针
 * @param data              待压入队列的数据
 * @return QUEUE_StatusTypeDef 队列状态
 */
QUEUE_StatusTypeDef Queue_Push(QUEUE_HandleTypeDef *hqueue, QUEUE_DATA_T *data)
{
    // uint32_t offset = 0;

    if(data == NULL)
        return QUEUE_ERROR;
    if(hqueue->queue_cnt >= QUEUE_MAX_SIZE)
        return QUEUE_FULL;
    if(((hqueue->tail + 1) % QUEUE_MAX_SIZE) == hqueue->head)
        return QUEUE_FULL;
    // if((hqueue->tail + 1) % hqueue->queue_cnt ==  hqueue->head)
    //     return QUEUE_FULL;
        
    memcpy(hqueue->qbuff + (hqueue->tail * hqueue->queue_size), data, hqueue->queue_size);
    hqueue->tail = (hqueue->tail + 1) % QUEUE_MAX_SIZE;
    hqueue->queue_cnt++;

    return QUEUE_OK;
}

#if 0
/**
 * @brief 
 * 
 * @param hqueue 
 * @param data 
 * @param qsize 
 * @return uint32_t 
 */
uint32_t Queue_Push_Array(QUEUE_HandleTypeDef *hqueue, QUEUE_DATA_T *data, uint32_t qsize)
{

}
#endif

/**
 * @brief 从队列中弹出数据
 * 
 * @param hqueue            队列变量指针
 * @param pdata             待弹出队列的数据缓存地址
 * @return QUEUE_StatusTypeDef 队列状态
 */
QUEUE_StatusTypeDef Queue_Pop(QUEUE_HandleTypeDef *hqueue, QUEUE_DATA_T *pdata)
{
    if (pdata == NULL)
        return QUEUE_ERROR;
    
    if(hqueue->queue_cnt <= 0)
        return QUEUE_EMPTY;

    
    memcpy(pdata, hqueue->qbuff + (hqueue->head * hqueue->queue_size), hqueue->queue_size);
    memset(hqueue->qbuff + (hqueue->head * hqueue->queue_size), 0, hqueue->queue_size);
    hqueue->head = (hqueue->head + 1) % QUEUE_MAX_SIZE;
    hqueue->queue_cnt--;

    return QUEUE_OK;
}

#if 0
/**
 * @brief 
 * 
 * @param hqueue 
 * @param pdatas 
 * @param qsize 
 * @return uint32_t 
 */
uint32_t Queue_Pop_Array(QUEUE_HandleTypeDef *hqueue, QUEUE_DATA_T *pdatas, uint32_t qsize)
{

}
#endif

/**
 * @brief 查询现有数据（不删除队列中的数据）
 * 
 * @param hqueue            队列变量指针
 * @param pdata             待返回队列的数据缓存地址
 * @return QUEUE_StatusTypeDef 队列状态
 */
QUEUE_StatusTypeDef Queue_Peek(QUEUE_HandleTypeDef *hqueue, QUEUE_DATA_T *pdata, uint32_t num)
{
    if(hqueue->queue_cnt <= 0)
        return QUEUE_EMPTY;
    
    if(hqueue->queue_cnt < num)
        return QUEUE_OUTSIZE;
    
    memcpy(pdata, hqueue->qbuff + ((hqueue->head + num) * hqueue->queue_size), hqueue->queue_size);

    return QUEUE_OK;
    
}

#if 0
/**
 * @brief 
 * 
 * @param hqueue 
 * @param pdatas 
 * @param qsize 
 * @return uint32_t 
 */
uint32_t Queue_Peek_Array(QUEUE_HandleTypeDef *hqueue, QUEUE_DATA_T *pdatas, uint32_t qsize)
{

}
#endif