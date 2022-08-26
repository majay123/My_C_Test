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
 * @Date         : 2022-08-05 09:32:27
 * @LastEditors  : MCD
 * @LastEditTime : 2022-08-15 09:39:03
 * @FilePath     : /My_C_Test/mcuQueue/main.c
 * @Description  :
 *
 * ******************************************
 */

#include "myQueue.h"
#include "pthread.h"

struct uart_rx_data {
    uint8_t uart_payload[128];
    uint8_t rx_len;
    uint8_t rx_ch;
};

QUEUE_HandleTypeDef qUartRx;
QUEUE_DATA_T BufferUartRx[QUEUE_MAX_SIZE * sizeof(struct uart_rx_data)];

static void print_data(QUEUE_DATA_T *data, int size)
{
    int i = 0;

    printf("hex data:");
    for (i = 0; i < size; i++) {
        printf(" %02x", data[i]);
    }
    printf("\n");
}

static void *queue_read(void *arg)
{
    unsigned char ret = 0;
    struct uart_rx_data temp;

    while (1) {
        // printf("queut size: %d\n", qUartRx.queue_size);
        printf("head: %d, queue cnt: %d\n",qUartRx.head, qUartRx.queue_cnt);
        ret = Queue_Pop(&qUartRx, (uint8_t *)&temp);
        if (ret != QUEUE_OK) {
            printf("queue pop error: %d\n", ret);
            // break;
        }
        // if (qUartRx.queue_cnt > 0)
        print_data(temp.uart_payload, temp.rx_len);
        // usleep(1000 * 400);
        sleep(1);
        memset(&temp, 0, sizeof(struct uart_rx_data));
    }
    // for (i = 0; i < qUartRx.queue_cnt; i++) {
    //     Queue_Peek(&qUartRx, (uint8_t *)&temp, i);
    //     printf("temp ch: %d\n", temp.rx_ch);
    //     print_data(temp.uart_payload, temp.rx_len);
    // }
    return NULL;
}

int main(int argc, char const *argv[])
{
    int i = 0;
    // int flag = 0;
    pthread_t r_thread;
    unsigned char ret = 0;
    struct uart_rx_data uartrx;

    Queue_Init(&qUartRx, BufferUartRx, sizeof(struct uart_rx_data));
    ret = pthread_create(&r_thread, NULL, queue_read, NULL);
    if (ret != 0) {
        printf("create thread failed\n");
        exit(1);
    }

    while (1) 
    {
        i++;
        uartrx.rx_len = 128;
        uartrx.rx_ch = i;
        memset(uartrx.uart_payload, i, sizeof(uartrx.uart_payload));
        // printf("fill data to queue\n");
        ret = Queue_Push(&qUartRx, (uint8_t *)&uartrx);
        if (ret != QUEUE_OK) {
            printf("Queue_Push failed ret = %d\n", ret);
            break;
        }
        // printf(" qUartRx queue cnt = %d\n", qUartRx.queue_cnt);
        // printf("Queue push data!\n");
        // sleep(1);
        usleep(1000 * 900);
    }

    // pthread_cancel(r_thread);
    pthread_join(r_thread, NULL);

    return 0;
}
