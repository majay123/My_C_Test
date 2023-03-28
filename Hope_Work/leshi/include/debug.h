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
 * @Date         : 2023-03-21 10:00:16
 * @LastEditors  : MCD
 * @LastEditTime : 2023-03-27 12:57:25
 * @FilePath     : /My_C_Test/Hope_Work/leshi/include/debug.h
 * @Description  : 
 * 
 * ******************************************
 */

#ifndef __DIAG_DEBUG_H_
#define __DIAG_DEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "app_context.h"
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

extern appContext_t appContext;

#define log2file_fmt(mutex, fmt, ...)                    \
    do {                                                 \
        pthread_mutex_lock(&(mutex));                    \
        FILE *fp = fopen(appContext.app_log_file, "r+"); \
        if (fp == NULL && (errno == ENOENT))             \
            fp = fopen(appContext.app_log_file, "w+");   \
        if (fp) {                                        \
            fseek(fp, 0L, SEEK_END);                     \
            fprintf(fp, fmt, ##__VA_ARGS__);             \
            fclose(fp);                                  \
        }                                                \
        pthread_mutex_unlock(&(mutex));                  \
    } while (0);

#define DEBUG_WARN(param, ...)                                                                                                                            \
    do {                                                                                                                                                  \
        /* if (appContext.verbose) */ {                                                                                                                   \
            char ctime[30] = {0};                                                                                                                         \
            struct tm tm1 = {0};                                                                                                                          \
            struct timespec ts;                                                                                                                           \
            clock_gettime(CLOCK_REALTIME, &ts);                                                                                                           \
            localtime_r(&ts.tv_sec, &tm1);                                                                                                                \
            strftime(ctime, sizeof(ctime), "%y%m%d %H%M%S", &tm1);                                                                                        \
            fprintf(stdout, "\033[;33m[%s.%.3ld][%s]" param "\033[0m", ctime, ts.tv_nsec / 1000 / 1000, __func__, ##__VA_ARGS__);                         \
            fflush(stdout);                                                                                                                               \
            log2file_fmt(appContext.logFileMutex, "%s.%.3ld [W]:%d %s:" param, ctime, ts.tv_nsec / 1000 / 1000, appContext.src, __func__, ##__VA_ARGS__); \
        }                                                                                                                                                 \
    } while (0)

#define DEBUG_INFO(param, ...)                                                                                                                            \
    do {                                                                                                                                                  \
        /* if (appContext.verbose) */ {                                                                                                                         \
            char ctime[30] = {0};                                                                                                                         \
            struct tm tm1 = {0};                                                                                                                          \
            struct timespec ts;                                                                                                                           \
            clock_gettime(CLOCK_REALTIME, &ts);                                                                                                           \
            localtime_r(&ts.tv_sec, &tm1);                                                                                                                \
            strftime(ctime, sizeof(ctime), "%y%m%d %H%M%S", &tm1);                                                                                        \
            fprintf(stdout, "\033[;30m[%s.%.3ld][%s]" param "\033[0m", ctime, ts.tv_nsec / 1000 / 1000, __func__, ##__VA_ARGS__);                         \
            fflush(stdout);                                                                                                                               \
            log2file_fmt(appContext.logFileMutex, "%s.%.3ld [I]:%d %s:" param, ctime, ts.tv_nsec / 1000 / 1000, appContext.src, __func__, ##__VA_ARGS__); \
        }                                                                                                                                                 \
    } while (0)

#define DEBUG_ERROR(param, ...)                                                                                                                           \
    do {                                                                                                                                                  \
        /* if (appContext.verbose) */ {                                                                                                                   \
            char ctime[30] = {0};                                                                                                                         \
            struct tm tm1 = {0};                                                                                                                          \
            struct timespec ts;                                                                                                                           \
            clock_gettime(CLOCK_REALTIME, &ts);                                                                                                           \
            localtime_r(&ts.tv_sec, &tm1);                                                                                                                \
            strftime(ctime, sizeof(ctime), "%y%m%d %H%M%S", &tm1);                                                                                        \
            fprintf(stdout, "\033[;31m[%s.%.3ld][%s]" param "\033[0m", ctime, ts.tv_nsec / 1000 / 1000, __func__, ##__VA_ARGS__);                         \
            fflush(stdout);                                                                                                                               \
            log2file_fmt(appContext.logFileMutex, "%s.%.3ld [E]:%d %s:" param, ctime, ts.tv_nsec / 1000 / 1000, appContext.src, __func__, ##__VA_ARGS__); \
        }                                                                                                                                                 \
    } while (0)
#ifdef __cplusplus
}
#endif

#endif /*__DEBUG_H_*/
