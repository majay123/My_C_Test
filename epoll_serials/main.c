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
 * @LastEditTime : 2022-02-24 14:57:21
 * @FilePath     : /My_C_Test/epoll_serials/main.c
 * @Description  : 
 * 
 * ******************************************
 */

#include "es_debug.h"
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

int main(int argc, char const *argv[])
{
    /* code */
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
    ES_DEBUG_ERROR("error");
    ES_DEBUG_INFO("info");
    ES_DEBUG_WARN("warn");
    return 0;
}
