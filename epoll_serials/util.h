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
 * @LastEditTime : 2022-03-01 10:01:59
 * @FilePath     : /My_C_Test/epoll_serials/util.h
 * @Description  : 
 * 
 * ******************************************
 */

#ifndef UTIL_H
#define UTIL_H

#include "es_debug.h"

#define PATH_LEN      (128)
#define MAX_BUF_LEN   (1024 * 8)
#define DELIM         "="
#define ES_CONF_OK    (0)
#define ES_CONF_ERROR (-1)
#define MIN(a, b)     ((a) < (b) ? (a) : (b))

typedef struct es_conf {
    char root[PATH_LEN];
    int baud_rate;
    int thread_num;
    int test_num;
} es_conf_t;

int read_conf(char *filename, es_conf_t *conf);
void handle_for_sigpipe();

#endif // !UTIL_H