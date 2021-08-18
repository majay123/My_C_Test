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
 * @Date         : 2021-08-18 09:21:28
 * @LastEditors  : MCD
 * @LastEditTime : 2021-08-18 09:57:03
 * @FilePath     : /My_C_Test/globalfifo_test/globalfifo_sig.c
 * @Description  : 
 * 
 * ******************************************
 */

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_LEN (100)

void input_handler(int num)
{
    char data[MAX_LEN] = {0};
    int len;
    printf("receive a signal from globalfifo, signalnum = %d\n", num);

    // len = read(STDIN_FILENO, data, MAX_LEN);
    // data[len] = '\0';
    // printf("input data = %s", data);
}

int main(int argc, char const *argv[])
{
    int oflags;
    int fd;

    fd = open("/dev/globalfifo", O_RDONLY | S_IWUSR);
    if(fd != -1)
    {
        signal(SIGIO, input_handler);
        // 设置本进程拥有STDIN_FILENO文件的拥有者
        fcntl(fd, F_SETOWN, getpid());
        // 启用异步通信
        oflags = fcntl(fd, F_GETFL);
        fcntl(fd, F_SETFL, oflags | FASYNC);

        while (1)   
            usleep(1000 * 100);
    }
    else
        printf("device open failed\n");

    return 0;
}
