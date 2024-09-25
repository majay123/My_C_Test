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
 * ************Copyright 2024 MCD************
 * 
 * @version      : 
 * @Company      : HOPE
 * @Author       : MCD
 * @Date         : 2024-07-24 13:01:38
 * @LastEditors  : MCD
 * @LastEditTime : 2024-09-25 17:07:42
 * @FilePath     : /My_C_Test/QQMusic/common.h
 * @Description  : 
 * 
 * ******************************************
 */
#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef __cplusplus
extern "C"
{
#endif


#include <curl/curl.h>
#include <openssl/md5.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include "md5.h"

#define strlens(s) (s == NULL ? 0 : strlen((char *)s))
#define MAX_URL_HOST_LEN                    (128)
#define MAX_URL_REQUEST_LEN                 (1024)

//hardcode parameter
#define APP_DEFAULT_HTTP_CID								("750064224428658688")
#define APP_DEFAULT_HTTP_KEY								("A716A953593940D2BD78E1A02CD3C070")
#define APP_DEFAULT_HTTP_SID								("750837261197414400")
#define APP_DEFAULT_HTTP_VERSION							("1.0")

#define MD5_SUM_STR_LEN 32
#define MD5_SUM_HEX_LEN 16

struct MemoryStruct {
    char *memory;
    size_t size;
};

/**
 * @brief curl的回调函数
 * 
 * @param contents 
 * @param size 
 * @param nmemb 
 * @param userp 
 * @return size_t 
 */
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL) {
        printf("Not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    

    return realsize;
}


char *get_external_ip();
void md5_string(const char *str, char *outputBuffer);
char *url_encode(const char *str);
int get_strlen(const char *str);
uint8_t calc_des(size_t len);
uint8_t calc_des2(size_t len);

#ifdef __cplusplus
}
#endif

#endif /* __COMMON_H__ */