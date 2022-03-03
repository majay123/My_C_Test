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
 * @LastEditTime : 2022-03-03 16:33:17
 * @FilePath     : /epoll_serials/main.c
 * @Description  : 
 * 
 * ******************************************
 */

#include "epoll.h"
#include "es_debug.h"
#include "serial_common.h"
#include "serials_requset.h"
#include "util.h"
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

extern struct epoll_event *events;
pthread_mutex_t logFileMutex;
appContext_t esContext;

static void usage()
{
    fprintf(stdout, "\n");
    fprintf(stdout, "serial rs485 service\n");
    fprintf(stdout, "usage:\n");
    fprintf(stdout, "   -v verbose debug info\n");
    fprintf(stdout, "   --help\n");
}

/**
* @author  		MCD
* @date  		2022-03-01-09:29
* @details		set start param
*/
static void _set_start_param(int argc, char *argv[])
{
    int c;
    struct option long_options[] =
        {
            {"help", no_argument, NULL, GETOPT_VAL_HELP},
            {NULL, 0, NULL, 0}};

    opterr = 0;
    memset(&esContext, 0, sizeof(appContext_t));
    while ((c = getopt_long(argc, argv, "c:hvx",
                            long_options, NULL)) != -1) {
        switch (c) {
        case 'v':
            esContext.verbose = 1;
            break;
        case 'x':
            esContext.verbose = 0;
            break;
        case GETOPT_VAL_HELP:
        case 'h':
            usage();
            exit(EXIT_SUCCESS);
        case '?':
            // The option character is not recognized.
            fprintf(stderr, "Unrecognized option: %s\n", optarg);
            opterr = 1;
            break;
        }
    }

    if (opterr) {
        usage();
        exit(EXIT_FAILURE);
    }
}

#if 0
static int _test_open_fd(char *root, int i)
{
    char buf[256] = {0};
    int fd = -1;

    snprintf(buf, sizeof(buf), "%stest_files/test%d.txt", root, i + 1);
    ES_DEBUG_INFO("get file path: %s", buf);
    fd = open(buf, O_RDWR, 0665);

    return fd;
}
#endif

int main(int argc, char *argv[])
{
    es_conf_t conf;
    int i = 0;

    /* code */
    memset(&conf, 0, sizeof(es_conf_t));
    _set_start_param(argc, argv);
    // 读取配置
    if (read_conf(CONF_FILE, &conf) < 0) {
        exit(EXIT_FAILURE);
    }

    // 处理SIGPIPE
    handle_for_sigpipe();

    // 初始化测试的文件描述符

    // 创建epoll并注册监听描述符
    int epoll_fd = es_epoll_create(0);
    if (epoll_fd < 0) {
        ES_DEBUG_ERROR("es epoll create failed");
        if (events) {
            free(events);
            events = NULL;
        }
        exit(EXIT_FAILURE);
    }
    ES_DEBUG_INFO("get conf testnum = %d", conf.test_num);
    es_serial_request_t *request = (es_serial_request_t *)calloc(conf.test_num, sizeof(es_serial_request_t));
    for (i = 0; i < conf.test_num; i++) {
        // int fd = _test_open_fd(conf.root, i);
        // if(fd < 0) {
        //     ES_DEBUG_ERROR("open failed!!");
        //     continue;
        // }
#if 0
        int fd = STDIN_FILENO;
#else
        int fd = serial_init_open(DEV_PATH, 9600, 8, 1, 'N');
        if(fd < 0) {
            ES_DEBUG_ERROR("open failed!!");
            continue;
        }
#endif
        es_init_serial_request_t(&request[i], fd, epoll_fd, conf.root);
        int ret = es_epoll_add(epoll_fd, fd, &request[i], (EPOLLIN | EPOLLET));      
        // int ret = es_epoll_add(epoll_fd, fd, &request[i], (EPOLLIN));       
        if (ret < 0) {
            ES_DEBUG_ERROR("es epoll add failed %d !!", ret);
            exit(EXIT_FAILURE);
        }
    }

    // 初始化线程池
    es_threadpool_t *tp = threadpool_init(conf.thread_num);

    while (1) {
        // 调用epoll_wait, 返回接收到事件的数量
        int event_num = es_epoll_wait(epoll_fd, events, MAXEVENTS, -1);
        // 遍历events数据，根据监听种类以及描述符类型分发操作
        if (event_num > 0) {
            // printf("num events: %d\n", event_num);
            ES_DEBUG_INFO("event num = %d", event_num);
            es_handle_events(epoll_fd, events, event_num, es_serial_dispatch_rs485, tp);
        }
    }

    // es_init_serial_request_t(request, )
    return 0;
}
