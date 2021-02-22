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
 * @Date         : 2021-02-20 16:01:53
 * @LastEditors  : MCD
 * @LastEditTime : 2021-02-22 09:00:03
 * @FilePath     : /trunk/semaphore/server/main.c
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

//server
int main(int argc, char *argv[])
{
    char ch;
    int shmid;
    key_t key;
    char *shm, *s;
    sem_t *mutex;
    
    //name the shared memory segmen;
    key = 1000;

    //create & init semaphore
    mutex = sem_open(SEM_NAME, O_CREAT, 0644, 1);
    if(mutex == SEM_FAILED)
    {
        perror("unable to create semaphore");
        sem_unlink(SEM_NAME);
        exit(-1);
    }

    //create the shared memory segment with this key;
    shmid = shmget(key, SHMSZ, IPC_CREAT | 0666);
    if(shmid < 0)
    {
        perror("failure in shmget");
        exit(-1);
    }

    //attach this segment to virtual memory;
    shm = shmat(shmid, NULL, 0);

    //start writing into memory;
    s = shm;
    for(ch = 'A'; ch <= 'Z'; ch++)
    {
        sem_wait(mutex);
        *s++ = ch;
        sem_post(mutex);
    }

    //the below loop could be replaced by binary semaphore
    while(*shm != '*')
    {
        sleep(1);
    }
    sem_close(mutex);
    sem_unlink(SEM_NAME);
    shmctl(shmid, IPC_RMID, 0);

    return EXIT_SUCCESS;
}