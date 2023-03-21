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
 * ************Copyright 2023 MCD************
 * 
 * @version      : 
 * @Company      : HOPE
 * @Author       : MCD
 * @Date         : 2023-03-16 16:36:00
 * @LastEditors  : MCD
 * @LastEditTime : 2023-03-21 15:39:54
 * @FilePath     : /My_C_Test/Hope_Work/leshi/ls_bt_mesh_dev.c
 * @Description  : 
 * 
 * ******************************************
 */
#include <fcntl.h>
#include <getopt.h>  //for getopt_long
#include <libubox/uloop.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/termios.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "app_context.h"
#include "common.h"
#include "debug.h"
#include "ls_bt_mesh_dev.h"
#include "ls_bt_mesh.h"
#include "queue.h"

/**
* @details  	
* @param[in]	device_path
* @param[in]    baud
* @param[in]    bitwidth
* @param[in]    stopbit
* @param[in]    parity
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-21-12:56 V1.0.0				MCD				create
*/
int ls_bt_mesh_dev_open(char *device_path, int baud, char parity, int bitwidth, char stopbit)
{
    struct termios tio;
    int fd = 0;
    speed_t speed;

    /* open the device */
    fd = open(device_path, O_RDWR | O_NOCTTY);  //block read/write
    if (fd < 0) {
        DEBUG_ERROR("%s device open failed\n", device_path);
        return (-1);
    }

    /* configure new values */
    cfmakeraw(&tio); /*see man page */

    switch (baud) {
        case 110:
            speed = B110;
            break;
        case 300:
            speed = B300;
            break;
        case 600:
            speed = B600;
            break;
        case 1200:
            speed = B1200;
            break;
        case 2400:
            speed = B2400;
            break;
        case 4800:
            speed = B4800;
            break;
        case 9600:
            speed = B9600;
            break;
        case 19200:
            speed = B19200;
            break;
        case 38400:
            speed = B38400;
            break;
        case 57600:
            speed = B57600;
            break;
        case 115200:
            speed = B115200;
            break;
        default:
            speed = B2400;
    }

    switch (bitwidth) {
        case 5:
            tio.c_cflag |= CS5;
            break;
        case 6:
            tio.c_cflag |= CS6;
            break;
        case 7:
            tio.c_cflag |= CS7;
            break;
        case 8:
        default:
            tio.c_cflag |= CS8;
            break;
    }

    /* Stop bit (1 or 2) */
    if (stopbit == 1)
        tio.c_cflag &= ~CSTOPB;
    else /* 2 */
        tio.c_cflag |= CSTOPB;

    /* parity */
    if (parity == 'N') {
        /* None */
        tio.c_cflag &= ~(PARENB);
    }
    else if (parity == 'E') {
        /* Even */
        tio.c_cflag |= PARENB;
        tio.c_cflag &= ~PARODD;
    }
    else {
        /* Odd */
        tio.c_cflag |= PARENB;
        tio.c_cflag |= PARODD;
    }

    tio.c_iflag |= IGNPAR; /*ignore parity on input */
    tio.c_iflag &= ~INPCK;
    tio.c_oflag &= ~(OPOST | ONLCR | OLCUC | OCRNL | ONOCR | ONLRET | OFILL);
    tio.c_cflag = CLOCAL | CREAD;
    // tio.c_cflag &= ~(PARENB);
    tio.c_cc[VMIN] = 1;  /* block until 1 char received */
    tio.c_cc[VTIME] = 0; /*no inter-character timer */

    cfsetospeed(&tio, speed);
    cfsetispeed(&tio, speed);

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &tio);

    return fd;
}

/**
* @details  	
* @param[in]	fd
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-21-12:58 V1.0.0				MCD				create
*/
void ls_bt_mesh_dev_close(int fd)
{
    if (fd > 0) {
        close(fd);
        fd = -1;
    }
}

/**
* @details  	
* @param[in]	fd
* @param[in]	rcv_buf
* @param[in]	data_len
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-21-12:58 V1.0.0				MCD				create
*/
int ls_bt_mesh_dev_read_data(int fd, char *rcv_buf, int data_len)
{
    int value = 0;
    int readBytes = 0;
    fd_set fs;
    int timeout = 1000;
    struct timeval tv;
    int notRead = data_len;
    uint8_t bytes_read = 0;

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
            return -1;
        }
        else if (value) {
            if (FD_ISSET(fd, &fs)) {
                readBytes = read(fd, rcv_buf + bytes_read, notRead);
                if (readBytes > 0) {
                    bytes_read += readBytes;
                    notRead -= readBytes;
                }
                else if (readBytes == 0) {
                    return -1;
                }
                else if (readBytes == -1) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
                        // no data, wait for read
                        continue;
                    }
                    else {
                        return -1;
                    }
                }
            }
            else {
                return -1;
            }
        }
        else if (value == 0) {
            return 0;  //select time out
        }
    }

    return bytes_read;
}


/**
* @details  	
* @param[in]	fd
* @param[in]	hndl
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-21-13:03 V1.0.0				MCD				create
*/
void ls_bt_mesh_dev_rev(int fd, llq_t *hndl)
{
    unsigned short fcs, tmp_fcs = 0;
    uint8_t retryAttempts, bytesRead = 0, len = 0, rcpBuffIdx = 0, rcpTempLen;
    uint8_t sofByte;
    uint8_t rcpBuff[LESHI_MAX_CMD_LEN] = {0};
    ls_mesh_dev_data_t llq_data;

    /* read first byte and check it is a SOF */
    bytesRead = ls_bt_mesh_dev_read_data(fd, (char *)&(rcpBuff[rcpBuffIdx]), 1);
    if ((bytesRead == 1) && (rcpBuff[rcpBuffIdx] == LESHI_DATA_HEADER1)) {
        rcpBuffIdx++;
        bytesRead = ls_bt_mesh_dev_read_data(fd, (char *)&(rcpBuff[rcpBuffIdx]), 1);
        if ((bytesRead == 1) && (rcpBuff[rcpBuffIdx] == LESHI_DATA_HEADER2)) {
            rcpBuffIdx++;
            /* read version, cmd, length byte */
            bytesRead = ls_bt_mesh_dev_read_data(fd, (char *)&(rcpBuff[rcpBuffIdx]), 4);
            if (bytesRead == 4) {
                rcpBuffIdx += 4;
                rcpTempLen = BUILD_UINT16(rcpBuff[5], rcpBuff[4]) + 1;
                while (rcpTempLen > 0) {
                    /* clear retry counter */
                    retryAttempts = 0;

                    /* read RCP frame */
                    bytesRead = ls_bt_mesh_dev_read_data(fd, (char *)&(rcpBuff[rcpBuffIdx]), rcpTempLen);
                    if (appContext.verbose)
                        DEBUG_INFO("bytesRead = %d, rcpTempLen = %d, retryAttempts = %d\n", bytesRead, rcpTempLen, retryAttempts);
                    /*  check for error */
                    if (bytesRead > rcpTempLen) {
                        /* check whether retry limits has been reached */
                        if (retryAttempts++ < 5) {
                            /* sleep for 10ms */
                            usleep(10000);
                            /* try again */
                            bytesRead = 0;
                        }
                        else {
                            /* something went wrong, abort */
                            DEBUG_ERROR("read failed too many times\n");
                            return;
                        }
                    }

                    /* update counters */
                    if (rcpTempLen > bytesRead) {
                        rcpTempLen -= bytesRead;
                    }
                    else {
                        rcpTempLen = 0;
                    }
                    rcpBuffIdx += bytesRead;
                }
                /* dump uart buffer */
                if (appContext.verbose) {
                    uint8_t *dump = dump_rs485_data1(rcpBuff, len);
                    if (dump) {
                        DEBUG_INFO("dump data:%s\n", dump);
                        free(dump);
                    }
                }
                /* Verify FCS of incoming RCP frames */
                fcs = check_sum_ls(&rcpBuff[0], rcpBuffIdx - 1);
                tmp_fcs = rcpBuff[rcpBuffIdx - 1];

                if (appContext.verbose)
                    DEBUG_INFO("crc = %x, check crc = %x\n", tmp_fcs, fcs);
                if (fcs != tmp_fcs) {
                    DEBUG_ERROR("crc check failed: get crc = %x, check crc = %x\n", tmp_fcs, fcs);
                    return;
                }
                /* llq add llq_uart_dispatch_stream */
                llq_data.data = (char *)calloc(rcpBuffIdx, sizeof(uint8_t));
                if (llq_data.data) {
                    memcpy(llq_data.data, rcpBuff, rcpBuffIdx);
                    llq_data.data_len = rcpBuffIdx;
                    if (appContext.verbose)
                        DEBUG_INFO("llq dat len = %d\n", llq_data.data_len);
                    if (llq_add(hndl, (char *)&llq_data, len, 0) < 0) {
                        DEBUG_ERROR("llq_handler add error\n");
                        free(llq_data.data);
                    }
                }
                else {
                    DEBUG_ERROR("llq data calloc failed!\n");
                }
            }
        }
    }
}

/**
* @details  	
* @param[in]	fd
* @param[in]	send_buf
* @param[in]	data_len
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 2023-03-21-13:02 V1.0.0				MCD				create
*/
int ls_bt_mesh_dev_write_data(int fd, char *send_buf, int data_len)
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
        }
        else if (writeBytes == -1) {
            ret = -1;
            break;
        }
    }

    return ret;
}