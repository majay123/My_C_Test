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
 * @Date         : 2022-08-04 08:59:31
 * @LastEditors  : MCD
 * @LastEditTime : 2022-08-05 10:50:01
 * @FilePath     : /My_C_Test/mcuQueue/myQueue.h
 * @Description  :
 *
 * ******************************************
 */

#ifndef __MYQUEUE_H__
#define __MYQUEUE_H__

// #ifdef __cplusplus
// extern "C" {
// #endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define QUEUE_MAX_SIZE (10)
#define QUEUE_DATA_T   unsigned char //队列数据类型定义

typedef struct
{
    uint32_t head;
    uint32_t tail;
    // uint32_t length; // total queue length
    int queue_cnt;       //队列当前数量
    uint32_t queue_size; //一个成员的大小

    uint8_t *qbuff;
} QUEUE_HandleTypeDef;

typedef enum {
    QUEUE_OK = 0,    //队列ok
    QUEUE_ERROR = 1, //队列错误
    QUEUE_BUSY,      //队列忙
    QUEUE_TIMEROUT,  //队列超时
    QUEUE_FULL,      //队列满
    QUEUE_EMPTY,     //队列空
    QUEUE_OUTSIZE,   //超出查询大小
} QUEUE_StatusTypeDef;

#if 0
/*********************************************************************
 * @fn      llq_open
 *
 * @brief   Create a queue handle
 *
 * @param    llq_t *hndl - handle to queue to be created
 *
 * @return   none
 */
extern void llq_open(llq_t *hndl);

/*********************************************************************
 * @fn      llq_close
 *
 * @brief   Destroy a queue handle
 *
 * @param   llq_t *hndl - handle to queue to be released
 *
 * @return   none
 */
extern void llq_close(llq_t *hndl);

/*********************************************************************
 * @fn      llq_close
 *
 * @brief   clean a queue all buffer data
 *
 * @param   llq_t *hndl - handle to queue to be released
 *
 * @return   none
 */
extern void llq_flush(llq_t *hndl);

/*********************************************************************
 * @fn      llq_fulled
 *
 * @brief   check a queue if full or not
 *
 * @param   llq_t *hndl - handle to queue
 *
 * @return   zero fulled,other not
 */
int llq_fulled(llq_t *hndl);

/*********************************************************************
 * @fn      llq_add
 *
 * @brief   write message to queue
 *
 * @param   llq_t *hndl - handle to queue to read the message from
 * @Param	char *buffer - Pointer to buffer containing the message
 * @Param	int len - Length of message
 * @Param	int prio - 1 message has priority and should be added to
 * 			head of queue, 0 message assed to tail of queue
 *
 * @return   if return zero successful,other indicate failure
 */
extern int llq_add(llq_t *hndl, char *buffer, int len, int prio);

/*********************************************************************
 * @fn      llq_receive
 *
 * @brief   Block until a message is recieved
 *
 * @param   llq_t *hndl - handle to queue to read the message from
 * @Param	char *buffer - Pointer to buffer to read the message in to
 * @Param	int maxLength - Max length of message to read
 *
 * @return   length of message read from queue if successful,-1 indicate failure
 */
extern int llq_receive(llq_t *hndl, char *buffer, int maxLength);
#endif

void Queue_Init(QUEUE_HandleTypeDef *hqueue, QUEUE_DATA_T *qbuff, uint32_t qsize);
void Queue_Clear(QUEUE_HandleTypeDef *hqueue);
uint32_t Queue_Count(QUEUE_HandleTypeDef *hqueue);
QUEUE_StatusTypeDef Queue_Push(QUEUE_HandleTypeDef *hqueue, QUEUE_DATA_T *data);
// uint32_t Queue_Push_Array(QUEUE_HandleTypeDef *hqueue, QUEUE_DATA_T *data, uint32_t qsize);
QUEUE_StatusTypeDef Queue_Pop(QUEUE_HandleTypeDef *hqueue, QUEUE_DATA_T *pdata);
// uint32_t Queue_Pop_Array(QUEUE_HandleTypeDef *hqueue, QUEUE_DATA_T *pdatas, uint32_t qsize);
QUEUE_StatusTypeDef Queue_Peek(QUEUE_HandleTypeDef *hqueue, QUEUE_DATA_T *pdata, uint32_t num);
// uint32_t Queue_Peek_Array(QUEUE_HandleTypeDef *hqueue, QUEUE_DATA_T *pdatas, uint32_t qsize);

// #ifdef __cplusplus
// }
// #endif
#endif