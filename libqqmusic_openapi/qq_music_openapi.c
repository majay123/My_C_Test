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
 * @Date         : 2024-07-25 10:42:18
 * @LastEditors  : MCD
 * @LastEditTime : 2024-07-30 09:56:57
 * @FilePath     : /My_C_Test/libqqmusic_openapi/qq_music_openapi.c
 * @Description  : 
 * 
 * ******************************************
 */

#include "qq_music_openapi.h"
#include "qq_music_openapi_private.h"

/**
 * @brief 获取MD5
 * 
 * @param str 
 * @param outputBuffer 
 */
static void md5_string(const char *str, char *outputBuffer)
{
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5((unsigned char *)str, strlen(str), (unsigned char *)&digest);

    for (int i = 0; i < 16; ++i)
        sprintf(&outputBuffer[i * 2], "%02x", (unsigned int)digest[i]);
}

/**
 * @brief 
 * 需要释放result
 * @param url 
 * @param sign_data 
 * @param result_data 
 * @return char* 
 */
static char *qqmusic_openapi_http_postex(const char *url, const char *sign_data)
{
    CURL *curl;
    CURLcode res;
    char *result = NULL;

    char params[896] = {0};
    char md5sum[256] = {0};

    struct MemoryStruct chunk;
    chunk.memory = malloc(1);  // 初始化
    chunk.size = 0;            // 没有数据

    // char base_data[512] = {0};

    snprintf(params, sizeof(params), "%s_%s", sign_data, QQ_MUSIC_APPKEY);
    md5_string(params, md5sum);
    printf("params: %s\n", params);
    // printf("md5sum: %s\n", md5sum);

    memset(params, 0, sizeof(params));
    snprintf(params, sizeof(params), "%s&sign=%s", sign_data, md5sum);
    printf("params: %s\n", params);

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        // 拼接 URL 和参数
        char full_url[1024];
        snprintf(full_url, sizeof(full_url), "%s?%s", url, params);
        // printf("full_url:%s\n", full_url);
        // 设置 curl 选项
        curl_easy_setopt(curl, CURLOPT_URL, full_url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        // 执行请求
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        else {
            // 打印响应数据
            // printf("%s\n", chunk.memory);
            printf("realsize: %ld, size: %ld\n", chunk.realsize, chunk.size);
            // memcpy(result_data, chunk.memory, chunk.size);
            // printf("result_data:%s\n", result_data);
        }

        // 清理
        curl_easy_cleanup(curl);
        // free(chunk.memory);
    }

    curl_global_cleanup();

    return result = chunk.memory;
}

/**
 * @brief  获取二维码
 * 需要释放result
 * @param ip 
 * @param result_data 
 * @return char* 
 */
char *qqmusic_openapi_get_qrcode(const char *ip)
{
    char base_data[512] = {0};
    char *result = NULL;

    time_t current_time = time(NULL);

    if (current_time == ((time_t)-1)) {
        // 错误处理
        fprintf(stderr, "Failed to obtain the current time.\n");
        return NULL;
    }

    snprintf(base_data, sizeof(base_data), CUSTOM_SDK_GET_QR_CODE_DATA, QQ_MUSIC_APPID, ip, QQ_MUSIC_DEVICE_ID, QQ_MUSIC_OPENAPI_CMD_CUSTOM_SDK_GET_QR_CODE, QQ_MUSIC_DEV_NAME, QQMUSIC_ENCRYPT_AUTH, QQ_MUSIC_APPID, QQ_MUSIC_PACKAGE_NAME, QQ_MUSIC_QRCODE_TYPE, QQ_MUSIC_SIGN_VERSION, current_time);
    // printf("params: %s\n", params);

    result = qqmusic_openapi_http_postex(QQ_MUSIC_DOMAIN_NAME, (const char *)base_data);

    // printf("===result: %p\n", result);

    return result;
}

// 轮训扫描状态
/**
 * @brief 
 * 
 * @param ip 
 * @param auth_code   二维码中间授权码
 * @return char* 
 */
char *qqmusic_openapi_qrcode_auth_poll(const char *ip, const char *auth_code)
{
    char base_data[512] = {0};
    char *result = NULL;

    time_t current_time = time(NULL);

    if (current_time == ((time_t)-1)) {
        // 错误处理
        fprintf(stderr, "Failed to obtain the current time.\n");
        return NULL;
    }

    // CUSTOM_QRCODE_AUTH_POLL_DATA
    snprintf(base_data, sizeof(base_data), CUSTOM_QRCODE_AUTH_POLL_DATA, QQ_MUSIC_APPID, ip, QQ_MUSIC_DEVICE_ID, QQ_MUSIC_OPENAPI_CMD_CUSTOM_QRCODE_AUTH_POLL, QQ_MUSIC_APPID, auth_code, QQ_MUSIC_SIGN_VERSION, current_time);

    result = qqmusic_openapi_http_postex(QQ_MUSIC_DOMAIN_NAME, (const char *)base_data);

    // printf("===result: %s\n", result);

    return result;
}

// 获取token
/**
 * @brief 
 * 
 * @param ip 
 * @param app_key 获取token时必传
 * @param code  授权成功后的code(cmd=getToken时必传)
 * @param cmd   操作指令 0: getToken 1: refreshToken
 * @param refresh_token   cmd=refreshToken时必传
 * @return char* 
 */
char *qqmusic_openapi_get_accesstoken(const char *ip, char *app_key, char *code, char cmd, char *refresh_token)
{
    char base_data[512] = {0};
    char *result = NULL;

    time_t current_time = time(NULL);

    if (current_time == ((time_t)-1)) {
        // 错误处理
        fprintf(stderr, "Failed to obtain the current time.\n");
        return NULL;
    }
  
}
