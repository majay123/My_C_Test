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
 * @
 * ************Copyright 2021 MCD************
 * 
 * @version      : 
 * @Company      : HOPE
 * @Author       : MCD
 * @Date         : 2021-02-20 10:45:57
 * @LastEditors  : MCD
 * @LastEditTime : 2021-02-20 14:28:23
 * @FilePath     : /trunk/semaphore/test1.c
 * @Description  : 
 * @
 * ******************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

int number;   //need to protect
sem_t sem_id1, sem_id2;


/**
* @details  	thread 1, sem_id1 muxlock, sem_id1 sem_wait(p), and number++, then sem_id2 sem_post(v)
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 202102201058 V1.0.0				MCD				create
*/
void *thread_one_fun(void *arg)
{
    int sval1 = 0;
    int sval2 = 0;
    
    sem_getvalue(&sem_id1, &sval1);
    sem_getvalue(&sem_id1, &sval2);
    printf("therad one : number = %d, sval1 = %d, sval2 = %d\n", number, sval1, sval2);

    sem_wait(&sem_id1);
    printf("thread one have the semphore\n");
    number++;
    
    sem_getvalue(&sem_id1, &sval1);
    sem_getvalue(&sem_id1, &sval2);
    printf("therad one : number = %d, sval1 = %d, sval2 = %d\n", number, sval1, sval2);
    
    sem_post(&sem_id2);

    sem_getvalue(&sem_id1, &sval1);
    sem_getvalue(&sem_id1, &sval2);
    printf("therad one : number = %d, sval1 = %d, sval2 = %d\n", number, sval1, sval2);

    return NULL;
}

/**
* @details  	thread 2, sem_id2 sem_wait(p), and number--, then sem_id1 sem_post(v)
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 202102201100 V1.0.0				MCD				create
*/
void *thread_two_fun(void *arg)
{
    int sval1 = 0;
    int sval2 = 0;

    sem_getvalue(&sem_id1, &sval1);
    sem_getvalue(&sem_id2, &sval2);
    printf("therad two : number = %d, sval1 = %d, sval2 = %d\n", number, sval1, sval2);

    sem_wait(&sem_id2);
    printf("thread two have the semphore\n");
    number--;

    sem_getvalue(&sem_id1, &sval1);
    sem_getvalue(&sem_id2, &sval2);
    printf("therad two : number = %d, sval1 = %d, sval2 = %d\n", number, sval1, sval2);

    sem_post(&sem_id1);

    sem_getvalue(&sem_id1, &sval1);
    sem_getvalue(&sem_id2, &sval2);
    printf("therad two : number = %d, sval1 = %d, sval2 = %d\n", number, sval1, sval2);

    return NULL;
}

int main1(int argc, char **argv)
{
    number = 1;
    pthread_t id1, id2;
    /*
     *  由于程序初始时, sem_id1可进入, sem_id2不可进入
     *  两个线程的动作如下:
     *  thread one P(id1)  number++ V(id2)
     *  thread two P(id2)  number-- V(id1)
     *  而id1可进入, id2不可进入
     *  因此thread one先执行
     *  如果将id1与id2的顺序交换, 则执行顺序相反
     * */
    sem_init(&sem_id1, 0, 1);   //free
    sem_init(&sem_id2, 0, 0);   //busy

    pthread_create(&id1, NULL, thread_one_fun, NULL);
    pthread_create(&id2, NULL, thread_two_fun, NULL);

    pthread_join(id1, NULL);
    pthread_join(id2, NULL);

    printf("main .....\n");

    return 1;
}