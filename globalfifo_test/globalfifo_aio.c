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
 * @Date         : 2021-08-18 10:42:12
 * @LastEditors  : MCD
 * @LastEditTime : 2021-08-18 11:05:29
 * @FilePath     : /My_C_Test/globalfifo_test/globalfifo_aio.c
 * @Description  : 
 * 
 * ******************************************
 */

#define _GUN_SOURCE
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <libaio.h>

#define BUF_SIZE (4096)

int main(int argc, char const *argv[])
{
    io_context_t ctx = 0;
    struct iocb cb;
    struct iocb *cbs[1];
    unsigned char *buf;
    struct io_event events[1];
    int ret;
    int fd;

    if (argc < 2) {
        printf("the command format: aior [FILE]\n");
        exit(1);
    }

    fd = open(argv[1], O_RDONLY | O_CREAT | __O_DIRECT);
    if (fd < 0) {
        perror("open error");
        goto error;
    }

    ret = posix_memalign((void **)&buf, 512, (BUF_SIZE + 1));
    if (ret < 0) {
        perror("posix memalign failed");
        goto error1;
    }

    memset(buf, 0, BUF_SIZE + 1);
    ret = io_setup(128, &ctx);
    if (ret < 0) {
        printf("io setup error: %s\n", strerror(-ret));
        goto error2;
    }

    io_prep_pread(&cb, fd, buf, BUF_SIZE, 0);
    cbs[0] = &cb;

    ret = io_submit(ctx, 1, cbs);
    if (ret != 1) {
        if (ret < 0)
            printf("io submit error；%s\n", strerror(-ret));
        else
            fprintf(stderr, "could not sumbit IOs");
        goto error3;
    }

    ret = io_getevents(ctx, 1, 1, events, NULL);
    if (ret != 1) {
        if (ret < 0)
            printf("io getevents error: %s\n", strerror(-ret));
        else
            fprintf(stderr, "could not get Events");
        goto error3;
    }

    if (events[0].res2 == 0)
        printf("%s\n", buf);
    else {
        printf("AIO error: %s", strerror(-events[0].res));
        goto error3;
    }

    if ((ret = io_destroy(ctx)) < 0) {
        printf("io destroy error: %s", strerror(-ret));
        goto error2;
    }
    free(buf);
    close(fd);
    return 0;
error3:
    if ((ret = io_destroy(ctx)) < 0)
        printf("io destroy error: %s", strerror(-ret));
error2:
    if (buf != NULL) {
        free(buf);
        buf = NULL;
    }
error1:
    if (fd != -1)
        close(fd);
error:
    return -1;
}
