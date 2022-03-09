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
 * @LastEditTime : 2022-03-08 14:52:27
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
#include <fcntl.h> //文件控制定义
#include <termios.h> //POSIX终端控制定义

#define CONF_FILE "../serial.conf"

static void usage()
{
    fprintf(stdout, "\n");
    fprintf(stdout, "serial rs485 service\n");
    fprintf(stdout, "usage:\n");
    fprintf(stdout, "   -v verbose debug info\n");
    fprintf(stdout, "   --help\n");
}


static int _serial_cfg(int fd)
{
    struct termios opt, oldtio;
    int i;
    int status;

    int speed_arr[] = {B115200, B19200, B9600, B4800, B2400, B1200, B300};
    int name_arr[] = {115200, 19200, 9600, 4800, 2400, 1200, 300};

    /*保存测试现有串口参数设置，在这里如果串口号等出错，会有相关的出错信息*/
    if (tcgetattr(fd, &oldtio) != 0) {
        printf("SetupSerial 1\n");
        return -1;
    }
    bzero(&opt, sizeof(opt));

    cfsetispeed(&opt, B9600);
    cfsetospeed(&opt, B9600);
    opt.c_cflag |= CLOCAL | CREAD;
    opt.c_cflag &= ~(CRTSCTS | CSIZE);

    opt.c_cflag |= CS8;
    opt.c_cflag &= ~PARENB;
    opt.c_iflag &= ~INPCK;

    opt.c_cflag &= ~CSTOPB;

    opt.c_oflag &= ~OPOST;
    opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    opt.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

    opt.c_cc[VTIME] = 0;
    opt.c_cc[VMIN] = 0;

    tcflush(fd, TCIFLUSH);
    
    if (tcsetattr(fd, TCSANOW, &opt) != 0) {
        printf("SetupSerial 3\n");
        return -1;
    }
    
    return 0;
}

static int _serial_open(void)
{
    int fd;

    if((fd = open("/dev/ttyS0", O_RDWR|O_NOCTTY|O_NDELAY)) < 0) {
        printf("Error opening!");
        return -1;
    }
    if(fcntl(fd, F_SETFL, 0) < 0) {
        printf("Error setting");
        return -1;
    }

    return fd;
}


#if 1
int main(int argc, char *argv[])
{
#if 1
        // int fd = STDIN_FILENO;
        int fd = _serial_open();
        if (fd > 0) {
            _serial_cfg(fd);
        }
        else
            return -1;
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
        len = read(fd, buf, 100);
        // len = serial_read_data(fd, buf, 6);
        // len = serial_read_data(fd, buf, sizeof(buf));
        if(len > 0) {
            printf("read data: %d \n", len);
            for ( i = 0; i < 6; i++)
            {
                printf("%02x ", buf[i]);
            }
            printf("\n");
            printf("%s\n", buf);
        }
        // else
        //     printf("no data available %d\n", len);
    }
    return 0;
}
#endif