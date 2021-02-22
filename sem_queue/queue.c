/*
 * queue.c
 *
 * This module contains the POSIX wrapper for Semaphore.
 *
 * Copyright (C) 2013 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <errno.h>
#include "queue.h"

static int addToHead(llq_t *hndl, char *data, int length)
{
    if(hndl->head == NULL)
    {
        hndl->head = (node_t *) malloc(1 * sizeof(node_t));
        if(!hndl->head)
        {
            return -1;
        }
        hndl->head->ptr = NULL;
        hndl->head->data = (char *) malloc(length * (sizeof(char)));
        if(!hndl->head->data)
        {
            free(hndl->head);
            return -1;
        }
        hndl->head->length = length;
        memcpy(hndl->head->data, data, length);
        hndl->tail = hndl->head;
    }
    else
    {
        hndl->temp = (node_t *) malloc(1 * sizeof(node_t));
        if(!hndl->temp)
        {
            return -1;
        }
        hndl->temp->ptr = hndl->head;
        hndl->temp->data = (char *) malloc(length * (sizeof(char)));
        if(!hndl->temp->data)
        {
            free(hndl->temp);
            return -1;
        }
        hndl->temp->length = length;
        memcpy(hndl->temp->data, data, length);

        hndl->head = hndl->temp;
    }
    return 0;
}

static int addToTail(llq_t *hndl, char *data, int length)
{
    if(hndl->tail == NULL)
    {
        hndl->tail = (node_t *) malloc(1 * sizeof(node_t));
        if(!hndl->tail)
        {
            return -1;
        }
        hndl->tail->ptr = NULL;
        hndl->tail->data = (char *) malloc(length * (sizeof(char)));
        if(!hndl->tail->data)
        {
            free(hndl->tail);
            return -1;
        }
        hndl->tail->length = length;
        memcpy(hndl->tail->data, data, length);
        hndl->head = hndl->tail;
    }
    else
    {
        hndl->temp = (node_t *) malloc(1 * sizeof(node_t));
        if(!hndl->temp)
        {
            return -1;
        }
        hndl->tail->ptr = hndl->temp;
        hndl->temp->data = (char *) malloc(length * (sizeof(char)));
        if(!hndl->temp->data)
        {
            free(hndl->temp);
            return -1;
        }
        hndl->temp->length = length;
        memcpy(hndl->temp->data, data, length);
        hndl->temp->ptr = NULL;

        hndl->tail = hndl->temp;
    }
    return 0;
}

void llq_open(llq_t *hndl)
{
    hndl->head = hndl->tail = NULL;
    hndl->count = 0;
    sem_init(&(hndl->llqAccessSem), 0, 1);
    sem_init(&(hndl->llqCountSem), 0, 0);
}

void llq_close(llq_t *hndl)
{
    //fix blocked when app exit
    sem_destroy(&(hndl->llqAccessSem));
    sem_destroy(&(hndl->llqCountSem));
    while(hndl->head)
    {
        hndl->temp = hndl->head->ptr;
        free(hndl->head->data);
        free(hndl->head);
        hndl->head = hndl->temp;
    }
}

void llq_flush(llq_t *hndl)
{
    //wait to get access to the que
    sem_wait(&(hndl->llqAccessSem));

    while(hndl->head)
    {
        hndl->temp = hndl->head->ptr;
        free(hndl->head->data);
        free(hndl->head);
        hndl->head = hndl->temp;
    }

    //release access sem
    sem_post(&(hndl->llqAccessSem));
}

int llq_timedreceive(llq_t *hndl, char *buffer, int maxLength,
                     const struct timespec *timeout)
{
    int rLength = 0, sepmRnt;

    if(timeout != NULL)
    {
        //wait for a message or timeout
        struct timespec tv;
        clock_gettime(CLOCK_REALTIME, &tv);
        tv.tv_sec += timeout->tv_sec;
        tv.tv_nsec += timeout->tv_nsec;
        while((sepmRnt = sem_timedwait(&(hndl->llqCountSem), &tv)) == -1 && errno == EINTR)
            continue;          /* Restart if interrupted by handler */
    }
    else
    {
        //wait for a message
        while((sepmRnt = sem_wait(&(hndl->llqCountSem))) == -1 && errno == EINTR)
            continue;          /* Restart if interrupted by handler */
    }

    if(sepmRnt != -1)
    {
        if(hndl->head != NULL)
        {
            //wait to get access to the que
            sem_wait(&(hndl->llqAccessSem));

            hndl->temp = hndl->head->ptr;
            memcpy(buffer, hndl->head->data, hndl->head->length);
            rLength = (int) hndl->head->length;

            if(hndl->count > 0)
            {
                hndl->count--;
            }

            //did head point to another element
            if(hndl->temp != NULL)
            {
                //free current head element and point head to next
                free(hndl->head->data);
                free(hndl->head);
                hndl->head = hndl->temp;
            }
            else
            {
                //no elements left in queue
                free(hndl->head->data);
                free(hndl->head);
                hndl->head = NULL;
                hndl->tail = NULL;
            }

            //release access sem
            sem_post(&(hndl->llqAccessSem));
        }
    }
    else
    {
        rLength = -1;
    }
    return rLength;
}

int llq_receive(llq_t *hndl, char *buffer, int maxLength)
{
    return llq_timedreceive(hndl, buffer, maxLength, NULL);
}

int llq_add(llq_t *hndl, char *buffer, int len, int prio)
{
    int ret = -1;
    if(!hndl || len <= 0)
    {
        return -1;
    }

    //wait to get access to the que
    sem_wait(&(hndl->llqAccessSem));

    if(hndl->count > QUEUE_MAX_SIZE)          //if queue very large,prevent system crash, we discard msg
    {
        goto End;
    }

    if(prio == 1)
    {
        ret = addToHead(hndl, buffer, len);
    }
    else
    {
        ret = addToTail(hndl, buffer, len);
    }

    //if enqueue successful
    if(!ret)
    {
        hndl->count++;
    }

End:
    //release access sem
    sem_post(&(hndl->llqAccessSem));

    if(!ret)      //if enqueue successful
    {
        //increase counting sem representing que length
        sem_post(&(hndl->llqCountSem));
    }

    return ret;
}


