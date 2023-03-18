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
 * @LastEditTime : 2022-11-15 15:04:28
 * @FilePath     : /My_C_Test/serial_test/main.c
 * @Description  : 
 * 
 * ******************************************
 */

#include "serial_common.h"
#include <errno.h>
#include <fcntl.h>
#include <fcntl.h>  //文件控制定义
#include <getopt.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>  //POSIX终端控制定义
#include <unistd.h>

#define CONF_FILE "../serial.conf"

static int rs_fd;
pthread_mutex_t mutex;

void select_usleep(int micro_seconds)
{
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = micro_seconds;
    select(0, NULL, NULL, NULL, &timeout);
}

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

int eide_rs485_open(char *device_path)
{
    struct termios tio, oldtio;
    int fd = 0;

    /* open the device */
    fd = open(device_path, O_RDWR | O_NOCTTY);  //block read/write
    if (fd < 0) {
        printf("%s device open failed\n", device_path);
        return (-1);
    }

    cfmakeraw(&tio); /*see man page */

    tio.c_iflag |= IGNPAR; /*ignore parity on input */
    tio.c_iflag &= ~INPCK;
    tio.c_oflag &= ~(OPOST | ONLCR | OLCUC | OCRNL | ONOCR | ONLRET | OFILL);
    tio.c_cflag = CS8 | CLOCAL | CREAD;
    tio.c_cflag &= ~(PARENB | CSTOPB);
    tio.c_cc[VMIN] = 1;  /* block until 1 char received */
    tio.c_cc[VTIME] = 0; /*no inter-character timer */

    cfsetospeed(&tio, B9600);
    cfsetispeed(&tio, B9600);

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &tio);

    return fd;
}

static flag = 0;
static void *send_thread(void *arg)
{
    uint32_t sleep_time = 1000 * 11;
    uint8_t buf[] = {0x01, 0x50, 0xFF, 0xFF, 0xFF, 0xFF, 0x4D};
    uint8_t buf1[] = {0x01, 0x51, 0xFF, 0xFF, 0xFF, 0xFF, 0x4E};
    uint8_t buf2[] = {0x01, 0x52, 0xFF, 0xFF, 0xFF, 0xFF, 0x4F};
    while (1) {
        printf("send data\n");
        flag = 0;
        maisi_rs485_send(rs_fd, buf, sizeof(buf));
        usleep(sleep_time);
        flag = 1;
        sleep(1);
        printf("--------------\n");

        flag = 0;
        maisi_rs485_send(rs_fd, buf1, sizeof(buf));
        usleep(sleep_time);
        flag = 1;
        sleep(1);
        printf("--------------\n");
        flag = 0;
        maisi_rs485_send(rs_fd, buf2, sizeof(buf));
        usleep(sleep_time);
        flag = 1;
        sleep(1);
        printf("--------------\n");

        printf("======================\n\n\n");
        sleep(5);
    }
}

/**
 * @brief need free
 * 
 * @param src 
 * @param ssize 
 * @return uint8_t* 
 */
uint8_t *dump_rs485_data1(uint8_t *src, size_t ssize)
{
    int i = 0;
    int offset = 0;
    uint8_t *dst;
    size_t dsize;

    if (src == NULL || ssize <= 0)
        return NULL;

    dst = (uint8_t *)calloc(ssize * 5 + 1, sizeof(uint8_t));
    if (!dst)
        return NULL;
    dsize = ssize * 5 + 1;

    for (i = 0; i < ssize; i++) {
        offset += snprintf((char *)(dst + offset), dsize, " 0x%02x", src[i]);  // 格式化的数据写入字符串
    }

    return dst;
}

static int _serial_open(void)
{
    int fd;

    if ((fd = open("/dev/ttyS1", O_RDWR | O_NOCTTY | O_NDELAY)) < 0) {
        printf("Error opening!");
        return -1;
    }
    // if (fcntl(fd, F_SETFL, 0) < 0) {
    //     printf("Error setting");
    //     return -1;
    // }

    return fd;
}

#if 1
int main(int argc, char *argv[])
{
#if 1
#if 0
    // int fd = STDIN_FILENO;
    // int rs_fd = _serial_open();
    // if (rs_fd > 0) {
    //     _serial_cfg(rs_fd);
    // }
    // else
    //     return -1;
    rs_fd = eide_rs485_open("/dev/ttyS1");
#else
    ms_rs485_t rs485;
    memset(&rs485, 0, sizeof(rs485));
    rs485.device = calloc(12, sizeof(uint8_t));
    memcpy(rs485.device, "/dev/ttyS1", sizeof("/dev/ttyS1"));
    rs485.baud = 9600;
    rs485.data_bit = 8;
    rs485.stop_bit = 1;
    rs485.parity = 'E';
    rs_fd = maisi_rs485_dev_open(&rs485);
#endif
#else
    rs_fd = serial_init_open(DEV_PATH, 9600, 8, 1, 'N');
    if (rs_fd < 0) {
        printf("open failed!!\n");
        return -1;
    }
    printf("fd = %d\n", rs_fd);
#endif
    int len = 0;
    char buf[1024] = {0};
    int i;
    pthread_t send;
    pthread_create(&send, NULL, send_thread, NULL);
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("mutex init failed");
        return -1;
    }

    while (1) {
#if 0
        {
            len = read(rs_fd, buf, 1);
            if (flag == 0)
                continue;
            // len = serial_read_data(fd, buf, 6);
            // len = serial_read_data(fd, buf, sizeof(buf));
            if (len > 0) {
                printf("read data: %d \n", len);
                for (i = 0; i < len; i++) {
                    printf("%02x ", buf[i]);
                }
                printf("\n");
                // printf("%s\n", buf);
            }
        }
        // else
        //     printf("no data available %d\n", len);
#else
        // len = maisi_rs485_receive_msg(rs_fd, buf, MSG_INDICATION);
        {
            if (flag == 0) {
                len = maisi_rs485_recv(rs_fd, buf, 1);
                memset(buf, 0, sizeof(buf));
                continue;
            }
            len = maisi_rs485_recv(rs_fd, buf, 2);
            if (len > 0) {
                uint8_t *data = dump_rs485_data1(buf, len);
                printf("recv data:%s\n", data);
                if (data)
                    free(data);
            }
            // printf("do something\n");
            memset(buf, 0, sizeof(buf));
        }
#endif
    }

    pthread_join(send, NULL);
    pthread_mutex_destroy(&mutex);
    return 0;
}
#else
int main(int argc, char *argv[])
{
    uint8_t buf[10] = {0x11, 0x12, 0x13, 0x14, 0x15, 0x23, 0x34, 0x45, 0x56, 0x67};

    uint8_t *data = dump_rs485_data1(&buf[3], sizeof(buf) - 3);
    printf("data:%s\n", data);
}
#endif