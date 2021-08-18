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
 * @Date         : 2021-08-17 15:45:53
 * @LastEditors  : MCD
 * @LastEditTime : 2021-08-17 15:56:52
 * @FilePath     : /My_C_Test/globalfifo_test/globalfifo_poll.c
 * @Description  : 
 * 
 * ******************************************
 */
#include <fcntl.h>
#include <stdio.h>
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
    int fd, num;
    char rd_ch[BUFFER_LEN];
    fd_set rfds, wfds; /* 读/写文件的描述符 */

    // 以非阻塞方式打开/dev/globalfifo的设备文件
    fd = open("/dev/globalfifo", O_RDONLY | O_NONBLOCK);
    if (fd != -1) {
        if (ioctl(fd, FIFO_CLEAR, 0) < 0)
            printf("ioctl commond failed\n");

        while (1) {
            FD_ZERO(&rfds);
            FD_ZERO(&wfds);

            FD_SET(fd, &rfds);
            FD_SET(fd, &wfds);

            select(fd + 1, &rfds, &wfds, NULL, NULL);
            if (FD_ISSET(fd, &rfds))
                printf("Poll monitor: can be read\n");
            if (FD_ISSET(fd, &wfds))
                printf("Poll monitor: can be written\n");
        }
    } else
        printf("Device open failure\n");

    return 0;
}
