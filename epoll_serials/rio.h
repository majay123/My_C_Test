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
 * @Date         : 2022-02-24 10:26:58
 * @LastEditors  : MCD
 * @LastEditTime : 2022-02-25 13:07:44
 * @FilePath     : /My_C_Test/epoll_serials/rio.h
 * @Description  : 
 * 
 * ******************************************
 */

#ifndef RIO_H
#define RIO_H

#include <sys/types.h>
#define RIO_BUFFER_SIZE (8 * 1024)

typedef struct {
    int rio_fd;                    /* descriptor for this internal buf */
    ssize_t rio_cnt;               /* unread bytes in internal buf */
    char *rio_bufptr;              /* next unread byte in internal buf */
    char rio_buf[RIO_BUFFER_SIZE]; /* internal buffer */
} rio_t;

ssize_t rio_readn(int fd, void *usebuf, size_t n);
ssize_t rio_writen(int fd, void *usebuf, size_t n);
void rio_readinitb(rio_t *rp, int fd);
ssize_t rio_readnb(rio_t *rp, void *usebuf, size_t n);
ssize_t rio_readlineb(rio_t *rp, void *usebuf, size_t maxlen);

#endif // !RIO_H