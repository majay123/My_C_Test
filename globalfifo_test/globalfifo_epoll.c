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
 * @Date         : 2021-08-17 15:46:05
 * @LastEditors  : MCD
 * @LastEditTime : 2021-08-17 17:10:49
 * @FilePath     : /My_C_Test/globalfifo_test/globalfifo_epoll.c
 * @Description  : 
 * 
 * ******************************************
 */

#include <fcntl.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define FIFO_CLEAR (0x01)
#define BUFFER_LEN (20)

int main(int argc, char const *argv[])
{
    int fd;
    char buf[2048] = {0};

    fd = open("/dev/globalfifo", O_RDONLY | O_NONBLOCK);
    printf("fd = %d\n", fd);
    if (fd != -1) {
        struct epoll_event ev_globalfifo, ev;
        int err;
        int epfd;

        if (ioctl(fd, FIFO_CLEAR, 0) < 0)
            printf("ioctl command failed\n");

        epfd = epoll_create(1);
        if (epfd < 0)
            printf("epoll create failed\n");
        
        bzero(&ev_globalfifo, sizeof(struct epoll_event));
        bzero(&ev, sizeof(struct epoll_event));
        ev_globalfifo.events = EPOLLIN | EPOLLPRI;
        ev_globalfifo.data.fd = fd;

        err = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev_globalfifo);
        if (err < 0) {
            perror("epoll ctrl error");
            return -1;
        }
        err = epoll_wait(epfd, &ev, 1, 15000);
        if (err < 0)
            perror("epoll wait error");
        else if (err == 0)
            printf("No data input in FIFO within 5s\n");
        else
        {
            // read(fd, );
            printf("FIFO is not empty: %d, %d\n", ev.events, ev.data.fd);
            int len = 0;
            char data;
            printf("get data = ");
            while(len >= 0){
                len = read(ev.data.fd, &data, 1);
                printf("%c", data);
            }
            printf("\n");
        }
        err = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev_globalfifo);
        if (err < 0)
            perror("epoll ctrl error");

    } else
        printf("Device open failure\n");
    return 0;
}
