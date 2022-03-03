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
 * @LastEditTime : 2022-03-03 16:49:12
 * @FilePath     : /epoll_serials/rio.c
 * @Description  : 
 * 
 * ******************************************
 */

#include "rio.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/**
* @author  		MCD
* @date  		2022-02-25-13:14
* @details		rio readn
*/
ssize_t rio_readn(int fd, void *usebuf, size_t n)
{
    size_t nleft = n;
    ssize_t nread = 0;

    char *bufp = (char *)usebuf;
    while (nleft > 0) {
        // printf("nleft = %d\n", nleft);
        if ((nread = read(fd, bufp, nleft)) < 0) {
            if (errno == EINTR)
                nread = 0;
            else
                return -1;
        } else if (nread == 0)
            break;
        // printf("nread = %d\n");
        nleft -= nread;
        bufp += nread;
    }
    return (n - nleft);
}

/**
* @author  		MCD
* @date  		2022-02-25-13:14
* @details		rio writen
*/
ssize_t rio_writen(int fd, void *usebuf, size_t n)
{
    size_t nleft = n;
    ssize_t nwritten = 0;
    char *bufp = (char *)usebuf;

    while (nleft > 0) {
        if ((nwritten = write(fd, bufp, nleft)) <= 0) {
            if (errno == EINTR)
                nwritten = 0;
            else
                return -1;
        }
        nleft -= nwritten;
        bufp += nwritten;
    }
    return n;
}

/**
* @author  		MCD
* @date  		2022-02-25-13:23
* @details		rio read
*/
static ssize_t _rio_read(rio_t *rp, char *usebuf, size_t n)
{
    size_t cnt;

    while (rp->rio_cnt <= 0) {
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
        if (rp->rio_cnt < 0) {
            if (errno == EAGAIN)
                return -EAGAIN;
            if (errno != EINTR)
                return -1;
        } else if (rp->rio_cnt == 0)
            return 0;
        else
            rp->rio_bufptr = rp->rio_buf;
    }
    cnt = n;
    if (rp->rio_cnt < (ssize_t)n)
        cnt = rp->rio_cnt;
    memcpy(usebuf, rp->rio_bufptr, cnt);
    rp->rio_bufptr += cnt;
    rp->rio_cnt -= cnt;

    return cnt;
}

/**
* @author  		MCD
* @date  		2022-02-25-13:15
* @details		rio readinitb
*/
void rio_readinitb(rio_t *rp, int fd)
{
    rp->rio_fd = fd;
    rp->rio_cnt = 0;
    rp->rio_bufptr = rp->rio_buf;
}

/**
* @author  		MCD
* @date  		2022-02-25-13:15
* @details		rio readnb
*/
ssize_t rio_readnb(rio_t *rp, void *usebuf, size_t n)
{
    size_t nleft = n;
    ssize_t nread = 0;
    char *bufp = (char *)usebuf;

    while (nleft > 0) {
        if ((nread = _rio_read(rp, bufp, nleft)) < 0) {
            if (errno == EINTR)
                return 0;
            else
                return -1;
        } else if (nread == 0)
            break;
        nleft -= nread;
        bufp += nread;
    }
    return (n - nleft);
}

/**
* @author  		MCD
* @date  		2022-02-25-13:15
* @details		rio readlineb
*/
ssize_t rio_readlineb(rio_t *rp, void *usebuf, size_t maxlen)
{
    size_t n = 0;
    ssize_t rc = 0;
    char c, *bufp = (char *)usebuf;

    for (n = 1; n < maxlen; n++) {
        if ((rc = _rio_read(rp, &c, 1)) == 1) {
            *bufp++ = c;
            if (c == '\n')
                break;
        } else if (rc == 0) {
            if (n == 1)
                return 0;
            else
                break;
        } else if (rc == -EAGAIN)
            return rc;
        else
            return -1;
    }
    *bufp = 0;
    return n;
}