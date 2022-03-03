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
 * @Date         : 2022-02-24 10:21:51
 * @LastEditors  : MCD
 * @LastEditTime : 2022-03-03 15:18:48
 * @FilePath     : /My_C_Test/serial_test/main.c
 * @Description  : 
 * 
 * ******************************************
 */

#include "serial_common.h"
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define CONF_FILE "../serial.conf"

static void usage()
{
    fprintf(stdout, "\n");
    fprintf(stdout, "serial rs485 service\n");
    fprintf(stdout, "usage:\n");
    fprintf(stdout, "   -v verbose debug info\n");
    fprintf(stdout, "   --help\n");
}

int main(int argc, char *argv[])
{
#if 0
        int fd = STDIN_FILENO;
#else
        int fd = serial_init_open(DEV_PATH, 9600, 8, 1, 'N');
        if(fd < 0) {
            printf("open failed!!\n");
            return -1;
        }
        printf("fd = %d\n", fd);
#endif
    int len = 0;
    char buf[1024] = { 0 };
    int i;
    while(1) {
        // len = read(fd, buf, sizeof(buf));
        len = serial_read_data(fd, buf, 6);
        if(len > 0) {
            printf("read data: %d \n", len);
            for ( i = 0; i < 6; i++)
            {
                printf("%02x ", buf[i]);
            }
            printf("\n");
            printf("%s\n", buf);
        }
    }
    return 0;
}
