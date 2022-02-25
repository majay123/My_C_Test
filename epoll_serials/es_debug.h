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
 * @Date         : 2022-02-24 12:50:39
 * @LastEditors  : MCD
 * @LastEditTime : 2022-02-25 09:14:07
 * @FilePath     : /My_C_Test/epoll_serials/es_debug.h
 * @Description  : 
 * 
 * ******************************************
 */

#ifndef ES_DEBUG_H_
#define ES_DEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

extern pthread_mutex_t logFileMutex;

typedef struct
{
    char verbose; // verbose debug info output control
} appContext_t;


extern appContext_t esContext;

#define LOG_TO_FILE  (0)
#define APP_LOG_FILE ("/tmp/gwservice.log")

#if LOG_TO_FILE
#define log2file_fmt(mutex, fmt, ...)         \
    do {                                      \
        pthread_mutex_lock(&(mutex));         \
        FILE *fp = fopen(APP_LOG_FILE, "r+"); \
        if (fp == NULL && (errno == ENOENT))  \
            fp = fopen(APP_LOG_FILE, "w+");   \
        if (fp) {                             \
            fseek(fp, 0L, SEEK_END);          \
            fprintf(fp, fmt, ##__VA_ARGS__);  \
            fclose(fp);                       \
        }                                     \
        pthread_mutex_unlock(&(mutex));       \
    } while (0);
#endif

#if LOG_TO_FILE
#define ES_DEBUG_WARN(param, ...)                                                                                                                          \
    do {                                                                                                                                                   \
        char ctime[30] = {0};                                                                                                                              \
        struct tm tm1 = {0};                                                                                                                               \
        struct timespec ts;                                                                                                                                \
        clock_gettime(CLOCK_REALTIME, &ts);                                                                                                                \
        localtime_r(&ts.tv_sec, &tm1);                                                                                                                     \
        strftime(ctime, sizeof(ctime), "%Y-%m-%d %H:%M:%S", &tm1);                                                                                         \
        snprintf(ctime, sizeof(ctime), "%s.%.3ld", ctime, ts.tv_nsec / 1000 / 1000);                                                                       \
        fprintf(stdout, "\033[1;31m[ES_DEBUG_WARN][%s]\033[0m:(%s),%s,%d--- " param "\n", ctime1, __FILE__, __func__, __LINE__, ##__VA_ARGS__);            \
        fflush(stdout);                                                                                                                                    \
        log2file_fmt(logFileMutex, "\033[1;31m[ES_DEBUG_WARN][%s]\033[0m:(%s),%s,%d--- " param "\n", ctime1, __FILE__, __func__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#else
#define ES_DEBUG_WARN(param, ...)                                                                                                      \
    do {                                                                                                                               \
        char ctime[30] = {0};                                                                                                          \
        char ctime1[30] = {0};                                                                                                          \
        struct tm tm1 = {0};                                                                                                           \
        struct timespec ts;                                                                                                            \
        clock_gettime(CLOCK_REALTIME, &ts);                                                                                            \
        localtime_r(&ts.tv_sec, &tm1);                                                                                                 \
        strftime(ctime, sizeof(ctime), "%Y-%m-%d %H:%M:%S", &tm1);                                                                     \
        snprintf(ctime1, sizeof(ctime), "%s.%.3ld", ctime, ts.tv_nsec / 1000 / 1000);                                                   \
        printf("\033[1;31m[ES_DEBUG_WARN][%s]\033[0m:(%s),%s,%d--- " param "\n", ctime1, __FILE__, __func__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#endif

#if LOG_TO_FILE
#define ES_DEBUG_INFO(param, ...)                                                                                                                          \
    do {                                                                                                                                                   \
        char ctime[30] = {0};                                                                                                                              \
        struct tm tm1 = {0};                                                                                                                               \
        struct timespec ts;                                                                                                                                \
        clock_gettime(CLOCK_REALTIME, &ts);                                                                                                                \
        localtime_r(&ts.tv_sec, &tm1);                                                                                                                     \
        strftime(ctime, sizeof(ctime), "%Y-%m-%d %H:%M:%S", &tm1);                                                                                         \
        snprintf(ctime, sizeof(ctime), "%s.%.3ld", ctime, ts.tv_nsec / 1000 / 1000);                                                                       \
        fprintf(stdout, "\033[1;31m[ES_DEBUG_INFO][%s]\033[0m:(%s),%s,%d--- " param "\n", ctime1, __FILE__, __func__, __LINE__, ##__VA_ARGS__);            \
        fflush(stdout);                                                                                                                                    \
        log2file_fmt(logFileMutex, "\033[1;31m[ES_DEBUG_INFO][%s]\033[0m:(%s),%s,%d--- " param "\n", ctime1, __FILE__, __func__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#else
#define ES_DEBUG_INFO(param, ...)                                                                                                          \
    do {                                                                                                                                   \
        if (esContext.verbose == 1) {                                                                                                      \
            char ctime[30] = {0};                                                                                                          \
            char ctime1[30] = {0};                                                                                                          \
            struct tm tm1 = {0};                                                                                                           \
            struct timespec ts;                                                                                                            \
            clock_gettime(CLOCK_REALTIME, &ts);                                                                                            \
            localtime_r(&ts.tv_sec, &tm1);                                                                                                 \
            strftime(ctime, sizeof(ctime), "%Y-%m-%d %H:%M:%S", &tm1);                                                                     \
            snprintf(ctime1, sizeof(ctime1), "%s.%.3ld", ctime, ts.tv_nsec / 1000 / 1000);                                                   \
            printf("\033[1;31m[ES_DEBUG_INFO][%s]\033[0m:(%s),%s,%d--- " param "\n", ctime1, __FILE__, __func__, __LINE__, ##__VA_ARGS__); \
        }                                                                                                                                  \
    } while (0)
#endif

#if LOG_TO_FILE
#define ES_DEBUG_ERROR(param, ...)                                                                                                                          \
    do {                                                                                                                                                    \
        char ctime[30] = {0};                                                                                                                               \
        struct tm tm1 = {0};                                                                                                                                \
        struct timespec ts;                                                                                                                                 \
        clock_gettime(CLOCK_REALTIME, &ts);                                                                                                                 \
        localtime_r(&ts.tv_sec, &tm1);                                                                                                                      \
        strftime(ctime, sizeof(ctime), "%Y-%m-%d %H:%M:%S", &tm1);                                                                                          \
        snprintf(ctime, sizeof(ctime), "%s.%.3ld", ctime, ts.tv_nsec / 1000 / 1000);                                                                        \
        fprintf(stdout, "\033[1;31m[ES_DEBUG_ERROR][%s]\033[0m:(%s),%s,%d--- " param "\n", ctime1, __FILE__, __func__, __LINE__, ##__VA_ARGS__);            \
        fflush(stdout);                                                                                                                                     \
        log2file_fmt(logFileMutex, "\033[1;31m[ES_DEBUG_ERROR][%s]\033[0m:(%s),%s,%d--- " param "\n", ctime1, __FILE__, __func__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#else
#define ES_DEBUG_ERROR(param, ...)                                                                                                      \
    do {                                                                                                                                \
        char ctime[30] = {0};                                                                                                           \
        char ctime1[30] = {0};                                                                                                           \
        struct tm tm1 = {0};                                                                                                            \
        struct timespec ts;                                                                                                             \
        clock_gettime(CLOCK_REALTIME, &ts);                                                                                             \
        localtime_r(&ts.tv_sec, &tm1);                                                                                                  \
        strftime(ctime, sizeof(ctime), "%Y-%m-%d %H:%M:%S", &tm1);                                                                      \
        snprintf(ctime1, sizeof(ctime), "%s.%.3ld", ctime, ts.tv_nsec / 1000 / 1000);                                                    \
        printf("\033[1;31m[ES_DEBUG_ERROR][%s]\033[0m:(%s),%s,%d--- " param "\n", ctime1, __FILE__, __func__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#endif


enum {
    GETOPT_VAL_HELP = 257,
};

#ifdef __cplusplus
}
#endif

#endif /*ES_DEBUG_H_*/