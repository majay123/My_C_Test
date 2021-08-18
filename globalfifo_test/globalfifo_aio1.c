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
 * ************Copyright 2021 MCD************
 * 
 * @version      : 
 * @Company      : HOPE
 * @Author       : MCD
 * @Date         : 2021-08-18 11:13:48
 * @LastEditors  : MCD
 * @LastEditTime : 2021-08-18 15:01:55
 * @FilePath     : /My_C_Test/globalfifo_test/globalfifo_aio1.c
 * @Description  : 
 * 
 * ******************************************
 */

#define _GUN_SOURCE
#define __STDC_FORMAT_MACROS

#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <libaio.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define FILE_TEST   "globalfifo_test copy.c" 
#define FILE_SIZE  (65536 * 10)
#define NUM_EVENTS (128)
#define ALIGN_SIZE (512)
#define RD_WR_SIZE (1024)
#define MAX_EVENTS (8192)

struct custom_iocb {
    struct iocb iocb;
    int nth_request;
};

void aio_callback(io_context_t ctx, struct iocb *iocb, long res, long res2)
{
    struct custom_iocb *aiocbp = (struct custom_iocb *)iocb;
    printf("nth_request: %d, request_type: %s, offset: %lld, length: %lu, res: %ld, res2: %ld\n",
           aiocbp->nth_request, (iocb->aio_lio_opcode == IO_CMD_PREAD) ? "READ" : "WRITE",
           iocb->u.c.offset, iocb->u.c.nbytes, res, res2);
}

int main(int argc, char const *argv[])
{
    int efd = -1, fd = -1, epfd = -1;
    io_context_t ctx = 0;
    struct timespec tms;
    struct io_event events[NUM_EVENTS];
    struct custom_iocb iocbs[NUM_EVENTS];
    struct iocb *iocbps[NUM_EVENTS];
    struct custom_iocb *iocbp;
    struct epoll_event epevent;
    int i, j, r;
    void *buf = NULL;

    efd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (efd == -1) {
        perror("eventfd failed!");
        return -1;
    }

    fd = open(FILE_TEST, O_CREAT | O_RDONLY | __O_DIRECT, 0644);
    if (fd == -1) {
        perror("open file failed!");
        return -2;
    }

    // 截断到指定大小
    ftruncate(fd, FILE_SIZE);

    // ctx = 0;
    if (io_setup(MAX_EVENTS, &ctx)) {
        perror("io setup failed!");
        return -3;
    }

    if (posix_memalign(&buf, ALIGN_SIZE, RD_WR_SIZE)) {
        perror("posix memalign failed!");
        goto err;
    }
    memset(buf, 0, RD_WR_SIZE);

    for (i = 0, iocbp = iocbs; i < NUM_EVENTS; ++i, ++iocbp) {
        iocbps[i] = &iocbp->iocb;
        io_prep_pread(&iocbp->iocb, fd, buf, RD_WR_SIZE, i * RD_WR_SIZE);
        io_set_eventfd(&iocbp->iocb, efd);
        io_set_callback(&iocbp->iocb, aio_callback);
        iocbp->nth_request = i + 1;
    }

    if (io_submit(ctx, NUM_EVENTS, iocbps) != NUM_EVENTS) {
        perror("io submit");
        goto err;
    }

    epfd = epoll_create(1);
    if (epfd < 0) {
        perror("epoll creat failed");
        goto err;
    }

    epevent.events = EPOLLIN | EPOLLET;
    epevent.data.ptr = NULL;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, efd, &epevent)) {
        perror("epoll ctl failed");
        goto err;
    }

    i = 0;
    while (i < NUM_EVENTS) {
        uint64_t finished_aio;
        if (epoll_wait(epfd, &epevent, 1, -1) != 1) {
            perror("epoll wait failed");
            goto err;
        }

        if (read(efd, &finished_aio, sizeof(finished_aio)) != sizeof(finished_aio)) {
            perror("read failed!");
            goto err;
        }
        printf("finished io number: %" PRIu64 "\n", finished_aio);

        while (finished_aio > 0) {
            tms.tv_sec = 0;
            tms.tv_nsec = 0;
            r = io_getevents(ctx, 1, NUM_EVENTS, events, &tms);
            printf("io get events = %d\n", r);
            if (r > 0) {
                for (j = 0; j < r; j++) {
                    ((io_callback_t)(events[j].data))(ctx, events[j].obj, events[j].res, events[j].res2);
                }
                i += r;
                finished_aio -= r;
            }
        }
    }

err:
    if (epfd != -1)
        close(epfd);
    if (buf != NULL) {
        free(buf);
        buf = NULL;
    }
    if (ctx > 0)
        io_destroy(ctx);
    if (fd != -1)
        close(fd);
    if (efd != -1)
        close(efd);
    // remove("auto.sh");

    return 0;
}
