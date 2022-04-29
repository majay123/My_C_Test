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
 * @Date         : 2022-04-29 10:28:37
 * @LastEditors  : MCD
 * @LastEditTime : 2022-04-29 10:45:34
 * @FilePath     : /My_C_Test/socketpair/main.c
 * @Description  :
 *
 * ******************************************
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
    int pipe_fd[2] = {0};
    pid_t pid;
    int ret = socketpair(AF_UNIX, SOCK_STREAM, 0, pipe_fd);
    char buf[1024] = {0};

    if (ret < 0) {
        perror("socketpair failed");
        exit(-1);
    }

    pid = fork();
    if (pid < 0) {
        printf("Fork failed\n");
        exit(-1);
    } else if (pid == 0) {
        // child process
        printf("do child process\n");
        close(pipe_fd[0]);
        while (1) {
            memset(buf, 0, sizeof(buf));
            strcpy(buf, "I am your child");
            write(pipe_fd[1], buf, sizeof(buf) - 1);    // 子进程向管道写入数据
            memset(buf, 0, sizeof(buf));
            ssize_t n = read(pipe_fd[1], buf, sizeof(buf) - 1);     // 子进程从管道读取父进程数据
            buf[n] = 0;
            printf("father->child: %s\n", buf);
            sleep(1);
        }
        close(pipe_fd[1]);
    } else {
        // parent process
        printf("do parent process\n");
        close(pipe_fd[1]);
        while (1) {
            memset(buf, 0, sizeof(buf));
            ssize_t n = read(pipe_fd[0], buf, sizeof(buf)-1);
            buf[n] = 0;
            if (n > 0) {
                printf("child->fahter: %s\n", buf);
            } else {
                printf("read failed\n");
                break;
            }
            memset(buf, 0, sizeof(buf));
            strcpy(buf, "I am your father");
            write(pipe_fd[0], buf, sizeof(buf) - 1);     // 父进程向管道写入数据
            sleep(1);
        }
        close(pipe_fd[0]);
    }
}