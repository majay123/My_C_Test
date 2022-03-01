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
 * @Date         : 2022-03-01 13:10:41
 * @LastEditors  : MCD
 * @LastEditTime : 2022-03-01 13:36:43
 * @FilePath     : /My_C_Test/epoll_test/main.c
 * @Description  : 
 * 
 * ******************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/epoll.h>

struct epoll_event events[10];

int main(int argc, char const *argv[])
{
    struct epoll_event ev;
    int fd = -1;
    
    memset(&events, 0, sizeof(struct epoll_event) * 10);
    memset(&ev, 0, sizeof(struct epoll_event));

    fd = open("test.txt", O_RDWR);
    if(fd < 0) {
        printf("Couldn't open test.txt\n");
        exit(EXIT_FAILURE);
    }

    int epoll_fd = epoll_create1(0);
    if(epoll_fd < 0) {
        printf("epoll_create1 failed :%d!!\n", epoll_fd);
        exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN | EPOLLET;  //输入 边缘触发
    ev.data.fd = STDIN_FILENO;
    int ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &ev);
    
    if(ret < 0) {
        printf("epoll_ctl_add failed %d\n", ret);
        exit(EXIT_FAILURE);
    }

    while(1) {
        int nfds = epoll_wait(epoll_fd, events, 10, -1);
        if ( nfds == -1 )
        {
            printf( "epoll_wait\n" );;
            exit( EXIT_FAILURE );
        }
        char buf[1024] = {0};
        for ( int i = 0; i < nfds; i++ ) {
            // read(events[i].data.fd, buf, sizeof(buf));
            // printf( "read: %s\n", buf );
            if(events[i].data.fd == STDIN_FILENO)
                printf("hello world\n");
        }
    }

    return 0;
}
