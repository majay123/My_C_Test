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
 * @Date         : 2021-02-20 14:16:30
 * @LastEditors  : MCD
 * @LastEditTime : 2021-02-20 16:01:49
 * @FilePath     : /trunk/semaphore/test2.c
 * @Description  : 
 * @
 * ******************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <error.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

// int number;   //need to protect
// sem_t sem_id1, sem_id2;

int main2(int argc, char *argv[])
{
    int fd, i;
    int nloop = 10, zero = 0;
    int *ptr;
    sem_t mutex;

    //open a file and map it into memory
    fd = open("log.txt", O_RDWR | O_CREAT, S_IRWXU);
    // if(fd)
    write(fd, &zero, sizeof(int));
    ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    //create, initialize semaphore
    if(sem_init(&mutex, 1, 1) < 0)
    {
        printf("semaphore init failed\n");
        exit(0);
    }

    //fork child process
    if(fork() == 0)
    {
        for(i = 0; i < nloop; i++)
        {
            sem_wait(&mutex);
            printf("child: %d\n", (*ptr)++);
            sem_post(&mutex);
        }
        exit(0);
    }

    //back to parent process
    for(i = 0; i < nloop; i++)
    {
        sem_wait(&mutex);
        printf("parent: %d\n", (*ptr)++);
        sem_post(&mutex);
    }
    exit(0);
    
    // return 0;
}