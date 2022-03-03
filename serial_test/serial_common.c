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
 * @Date         : 2022-03-03 09:38:03
 * @LastEditors  : MCD
 * @LastEditTime : 2022-03-03 15:18:42
 * @FilePath     : /My_C_Test/serial_test/serial_common.c
 * @Description  : 
 * 
 * ******************************************
 */
#include "serial_common.h"

static int _serial_set_parity(int fd, int speed, int databits, int stopbits, int parity)
{
    struct termios tio, oldtio;
    int i;
    int status;

    int speed_arr[] = {B115200, B19200, B9600, B4800, B2400, B1200, B300};
    int name_arr[] = {115200, 19200, 9600, 4800, 2400, 1200, 300};

    /*保存测试现有串口参数设置，在这里如果串口号等出错，会有相关的出错信息*/
    if (tcgetattr(fd, &oldtio) != 0) {
        printf("SetupSerial 1\n");
        return -1;
    }

    bzero(&tio, sizeof(tio));
    cfmakeraw(&tio); /*see man page */
                     /* 设置字符大小 */
    tio.c_cflag |= CLOCAL | CREAD;

    tio.c_iflag |= IGNPAR; /*ignore parity on input */
    tio.c_oflag &= ~(OPOST | ONLCR | OLCUC | OCRNL | ONOCR | ONLRET | OFILL);

    /* set 波特率 */
    for (i = 0; i < sizeof(speed_arr) / sizeof(int); i++) {
        if (speed == name_arr[i]) {
            cfsetispeed(&tio, speed_arr[i]);
            cfsetospeed(&tio, speed_arr[i]);
        }
    }

    switch (databits) {
    case 7:
        tio.c_cflag |= CS7;
        break;
    case 8:
        tio.c_cflag |= CS8;
        break;
    default:
        printf("unsupported databits: %d\n", databits);
        return -1;
    }

    switch (parity) {
    case 'n':
    case 'N':
        tio.c_cflag &= ~PARENB; /* Clear parity enable */
        tio.c_iflag &= ~INPCK;  /* Enable parity checking */
        break;
    case 'o':
    case 'O':
        tio.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/
        tio.c_iflag |= INPCK;             /* Disnable parity checking */
        break;
    case 'e':
    case 'E':
        tio.c_cflag |= PARENB;  /* Enable parity */
        tio.c_cflag &= ~PARODD; /* 转换为偶效验*/
        tio.c_iflag |= INPCK;   /* Disnable parity checking */
        break;
    case 'S':
    case 's': /*as no parity*/
        tio.c_cflag &= ~PARENB;
        tio.c_cflag &= ~CSTOPB;
        break;
    default:
        printf("Unsupported parity\n");
        return -1;
    }

    /* 设置停止位*/
    switch (stopbits) {
    case 1:
        tio.c_cflag &= ~CSTOPB;
        break;
    case 2:
        tio.c_cflag |= CSTOPB;
        break;
    default:
        printf("Unsupported stop bits\n");
        return -1;
    }

    tio.c_cc[VMIN] = 1;  /* block until 1 char received */
    tio.c_cc[VTIME] = 0; /*no inter-character timer */

    tcflush(fd, TCIFLUSH);
    if (tcsetattr(fd, TCSANOW, &tio) != 0) {
        printf("SetupSerial 3\n");
        return -1;
    }

    return 0;
}

int serial_init_open(char *dev_path, int speed, int databits, int stopbits, int parity)
{
#if 1
    int fd = 0;

    fd = open(dev_path, O_RDWR | O_NOCTTY); // block read/write
    if (fd < 0) {
        printf("%s device open failed", dev_path);
        return -1;
    }

    if (_serial_set_parity(fd, speed, databits, stopbits, parity) < 0) {
        printf("serial set parity failed");
        return -1;
    }
#else
    struct termios tio, oldtio;
    int fd = 0;

    /* open the device */
    fd = open(dev_path, O_RDWR | O_NOCTTY);          //block read/write
    if(fd < 0)
    {
        printf("%s device open failed\n", dev_path);
        return (-1);
    }

    /* c-iflags
     B115200 : set board rate to 115200
     CRTSCTS : HW flow control
     CS8     : 8n1 (8bit,no parity,1 stopbit)
     CLOCAL  : local connection, no modem contol
     CREAD   : enable receiving characters*/

    /* configure new values */
    cfmakeraw(&tio); /*see man page */

    tio.c_iflag |= IGNPAR; /*ignore parity on input */
    tio.c_iflag &= ~INPCK;
    tio.c_oflag &= ~(OPOST | ONLCR | OLCUC | OCRNL | ONOCR | ONLRET | OFILL);
    tio.c_cflag = CS8 | CLOCAL | CREAD;
    tio.c_cflag &= ~(PARENB | CSTOPB);
    tio.c_cc[VMIN] = 1; /* block until 1 char received */
    tio.c_cc[VTIME] = 0; /*no inter-character timer */

    cfsetospeed(&tio, B9600);
    cfsetispeed(&tio, B9600);

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &tio);

#endif
    return fd;
}

int serial_read_data(int fd, char *rcv_buf, int data_len)
{
    int value = 0;
    int readBytes = 0;
    fd_set fs;
    int timeout = 1000;
    struct timeval tv;
    int notRead = data_len;
    int bytes_read = 0;

    if (fd <= 0) {
        return -1;
    }

    while (notRead > 0) {
        FD_ZERO(&fs);
        FD_SET(fd, &fs);

        //reinitialized time
        tv.tv_sec = (timeout / 1000);
        tv.tv_usec = (timeout % 1000) * 1000L;

        value = select(fd + 1, &fs, NULL, NULL, &tv);
        if (-1 == value) {
            printf("value = -1\n");
            return -1;
        } else if (value) {
            if (FD_ISSET(fd, &fs)) {
                readBytes = read(fd, rcv_buf + bytes_read, notRead);
                if (readBytes > 0) {
                    // printf("readBytes = %d\n", readBytes);
                    bytes_read += readBytes;
                    notRead -= readBytes;
                } else if (readBytes == 0) {
                    printf("readBytes = 0\n");
                    return -1;
                } else if (readBytes == -1) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
                        // no data, wait for read
                        continue;
                    } else {
                        printf("read bytes error\n");
                        return -1;
                    }
                }
            } else {
                // printf("FD_ISSET error\n");
                return -1;
            }
        } else if (value == 0) {
            // printf("select time out\n");
            return 0; //select time out
        }
    }
    // printf("bytes read = %d\n",bytes_read);
    return bytes_read;
}

int serial_write_data(int fd, char *send_buf, int data_len)
{
    int ret = 0;
    int remain = data_len;
    int offset = 0;
    int sub = 0, writeBytes = 0;

    if (fd < 0) {
        return -1;
    }
    while (remain > 0) {
        sub = (remain >= 8 ? 8 : remain);
        writeBytes = write(fd, send_buf + offset, sub);
        /*cflush(fd, TCOFLUSH);*/
        if (writeBytes > 0) {
            remain -= writeBytes;
            offset += writeBytes;
        } else if (writeBytes == -1) {
            ret = -1;
            break;
        }
    }

    return ret;
}