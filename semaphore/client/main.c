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
 * @Date         : 2021-02-20 16:21:09
 * @LastEditors  : MCD
 * @LastEditTime : 2021-02-22 08:59:50
 * @FilePath     : /trunk/semaphore/client/main.c
 * @Description  : 
 * @
 * ******************************************
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <semaphore.h>
#include <error.h>
#include <pthread.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define SHMSZ   (27)

char SEM_NAME[] = "vik";

//client
int main(int argc, char *argv[])
{
    int shmid;
    key_t key;
    char *shm, *s;
    sem_t *mutex;

    //name the shared memory segmet;
    key = 1000;

    //create & init existing  semaphore
    mutex = sem_open(SEM_NAME, O_RDONLY, 0644, 0);
    if(mutex == SEM_FAILED)
    {
        perror("reader: unable to execute semaphore");
        sem_unlink(SEM_NAME);
        exit(-1);
    }

    //create the shared memory with this key
    shmid = shmget(key, SHMSZ, 0666);
    if(shmid < 0)
    {
        perror("reader:failure in shmget");
        exit(-1);
    }

    //attach this segmet to virtual memory
    shm = shmat(shmid, NULL, 0);

    //start reading
    s = shm;
    for(s = shm; *s != '\0'; s++)
    {
        sem_wait(mutex);
        putchar(*s);
        sem_post(mutex);
    }

    //once done signal exiting of reader: this can be replaced bu another semaphore
    *shm = '*';
    sem_close(mutex);
    sem_unlink(SEM_NAME);
    
    return EXIT_SUCCESS;
}