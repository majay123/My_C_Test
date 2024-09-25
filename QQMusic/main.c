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
 * @Date         : 2024-07-22 16:16:22
 * @LastEditors  : MCD
 * @LastEditTime : 2024-09-25 17:07:49
 * @FilePath     : /My_C_Test/QQMusic/main.c
 * @Description  : 
 * 
 * ******************************************
 */

#include "cJSON.h"
#include "common.h"
#include "qq_music_openapi.h"


#define SCAN_QRCODE_TIMEOUT (10 * 60)  // 10分钟
#include <curl/curl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>



// 定义事件
typedef enum {
    EVENT_QR_CODE_RECEIVED,
    EVENT_QR_CODE_FAILED,
    EVENT_AUTH_SUCCESS,
    EVENT_TOKEN_RECEIVED,
    EVENT_TOKEN_EXPIRED,
    EVENT_REFRESH_TOKEN_EXPIRED,
    EVENT_TIMEOUT,
    EVENT_RESTART_QR_CODE,
    EVENT_IDLE
} Event;

typedef struct {
    Event event;
    char ip[17];
    char authCode[128];
    char code[128];
    char access_token[128];
    char refresh_token[128];
    char open_id[128];
    timer_t general_timer_id;  // 合并处理二维码超时和轮询的定时器
    timer_t token_timer_id;    // 处理令牌过期和刷新令牌过期的定时器
    int qr_code_attempts;
    int is_idle;
    int qr_code_poll_count;
    int token_expired_count;  // 记录 token 过期事件的触发次数
    int total_poll_count;     // 总轮询次数
    time_t token_expiry;      // 刷新令牌过期时间
    time_t refresh_expiry;    // 需要重新扫描二维码登陆qq音乐
} StateMachine;

typedef enum {
    HTTP_REQ_PRIO_NORMAL = 0,
    HTTP_REQ_PRIO_HIGH = 1,
} http_post_prio_t;

typedef struct {
    long timeout;           //http operation timeout
    http_post_prio_t prio;  //message priority
    bool post;
} http_post_option_t;

// 动态内存结构体
// struct MemoryStruct {
//     char *memory;
//     size_t size;
// };

// 获取当前时间
time_t current_time()
{
    return time(NULL);
}

// // curl回调函数
// static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
//     size_t realsize = size * nmemb;
//     struct MemoryStruct *mem = (struct MemoryStruct *)userp;

//     char *ptr = realloc(mem->memory, mem->size + realsize + 1);
//     if(ptr == NULL) {
//         printf("Not enough memory (realloc returned NULL)\n");
//         return 0;
//     }

//     mem->memory = ptr;
//     memcpy(&(mem->memory[mem->size]), contents, realsize);
//     mem->size += realsize;
//     mem->memory[mem->size] = 0;

//     return realsize;
// }

char *my_url_encode(const char *str)
{
    char *encoded = malloc(strlen(str) * 3 + 1);  // 分配最大可能的编码空间
    char *pstr = (char *)str, *pbuf = encoded;

    while (*pstr) {
        if (isalnum(*pstr) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~') {
            *pbuf++ = *pstr;
        }
        else {
            pbuf += sprintf(pbuf, "%%%02X", (unsigned char)*pstr);
        }
        pstr++;
    }
    *pbuf = '\0';
    return encoded;
}

// HTTP请求函数
int http_request(const char *url, const char *data, const char *url_data, struct MemoryStruct *response, bool post)
{
    CURL *curl;
    CURLcode res;
    struct curl_slist *pList = NULL;

    char params[1024 + 128] = {0};
    char md5sum[256] = {0};

    response->memory = malloc(1);
    response->size = 0;
    if (response->memory == NULL) {
        printf("Not enough memory (malloc returned NULL)\n");
        return -1;
    }

    snprintf(params, sizeof(params), "%s_%s", data, QQ_MUSIC_APPKEY);
    md5_string(params, md5sum);
    // printf("params: %s\n", params);
    // printf("md5sum: %s\n", md5sum);

    memset(params, 0, sizeof(params));
    if (url_data != NULL)
        snprintf(params, sizeof(params), "%s&sign=%s", url_data, md5sum);
    else
        snprintf(params, sizeof(params), "%s&sign=%s", data, md5sum);

    // printf("params: %s\n", params);

    curl = curl_easy_init();
    if (!curl) {
        printf("curl_easy_init() failed\n");
        free(response->memory);
        return -1;
    }
    char full_url[1024 + 256] = {0};
    snprintf(full_url, sizeof(full_url), "%s?%s", url, params);
    // printf("full_url:%s\n", full_url);

    curl_easy_setopt(curl, CURLOPT_URL, full_url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)response);

    if (post) {
        pList = curl_slist_append(pList,
                                  "Content-Type:application/x-www-form-urlencoded\r\nConnection:close");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, pList);

        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlens(data));
    }

    // if (data) {
    //     curl_easy_setopt(curl, CURLOPT_POSTFIELDS, params);
    // }

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        free(response->memory);
        curl_easy_cleanup(curl);
        return -1;
    }

    curl_easy_cleanup(curl);
    return 0;
}

// HTTP请求函数
int http_request_from_hope(const char *url, const char *data, struct MemoryStruct *response, bool post)
{
    CURL *curl;
    CURLcode res;
    struct curl_slist *pList = NULL;

    char params[1024 + 128] = {0};
    char md5sum[256] = {0};

    response->memory = malloc(1);
    response->size = 0;
    if (response->memory == NULL) {
        printf("Not enough memory (malloc returned NULL)\n");
        return -1;
    }

    // printf("params: %s\n", params);
    // printf("md5sum: %s\n", md5sum);

    // printf("params: %s\n", params);

    curl = curl_easy_init();
    if (!curl) {
        printf("curl_easy_init() failed\n");
        free(response->memory);
        return -1;
    }
    // printf("full_url:%s\n", full_url);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)response);

    if (post) {
        pList = curl_slist_append(pList,
                                  "Content-Type:application/x-www-form-urlencoded\r\nConnection:close");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, pList);

        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlens(data));
    }

    // if (data) {
    //     curl_easy_setopt(curl, CURLOPT_POSTFIELDS, params);
    // }

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        free(response->memory);
        curl_easy_cleanup(curl);
        return -1;
    }

    curl_easy_cleanup(curl);
    return 0;
}

// 处理二维码返回的消息
// { "auth_code": "auh-O30dw30wWSQJwuGYz6Wu", "msg": "ok", "ret": 0, "sdk_qr_code": "https:\/\/developer.y.qq.com\/openid\/1\/?a=2000000613&c=auh-O30dw30wWSQJwuGYz6Wu", "sub_ret": 0, "wx_qrcode_image": "" ,"vcheck":0,"traceId":"19990bfe66c91d4a","ts":1722309656852}
static int get_qr_code_response(StateMachine *sm, struct MemoryStruct *response)
{
    cJSON *root = NULL;
    int res = -1;

    if (response == NULL) {
        printf("No response\n");
        return -1;
    }
    if (response->memory) {
        printf("response: %s\n", response->memory);
        root = cJSON_Parse(response->memory);
        if (root == NULL) {
            printf("cJSON_Parse failed\n");
            return -1;
        }
        cJSON *ret = cJSON_GetObjectItem(root, "ret");
        res = ret->valueint;
        if (ret->valueint == 0) {
            cJSON *auth_code = cJSON_GetObjectItem(root, "auth_code");
            memcpy(sm->authCode, auth_code->valuestring, strlen(auth_code->valuestring));
            cJSON *sdk_qr_code = cJSON_GetObjectItem(root, "sdk_qr_code");
            printf("qr code: %s\n", sdk_qr_code->valuestring);
            printf("authCode: %s\n", sm->authCode);
            // return 0;
        }
    }
    if (root)
        cJSON_Delete(root);
    return res;
}

// 设备激活 设备使用之前先激活设备，方可进行后续的接口调用 fcg_music_custom_device_activate.fcg

// 获取登录二维码
int get_qr_code(StateMachine *sm)
{
    struct MemoryStruct response;
    char base_data[512] = {0};
    char base_url_data[512] = {0};
    int result = -1;

    time_t current_time = time(NULL);

    if (current_time == ((time_t)-1)) {
        // 错误处理
        fprintf(stderr, "Failed to obtain the current time.\n");
        return -1;
    }

    snprintf(base_data, sizeof(base_data), CUSTOM_SDK_GET_QR_CODE_DATA, QQ_MUSIC_APPID, sm->ip, QQ_MUSIC_DEVICE_ID, QQ_MUSIC_OPENAPI_CMD_CUSTOM_SDK_GET_QR_CODE, QQ_MUSIC_DEV_NAME, QQMUSIC_ENCRYPT_AUTH, QQ_MUSIC_APPID, QQ_MUSIC_PACKAGE_NAME, QQ_MUSIC_QRCODE_TYPE, QQ_MUSIC_SIGN_VERSION, current_time);
    snprintf(base_url_data, sizeof(base_url_data), CUSTOM_SDK_GET_QR_CODE_DATA, QQ_MUSIC_APPID, sm->ip, QQ_MUSIC_DEVICE_ID, QQ_MUSIC_OPENAPI_CMD_CUSTOM_SDK_GET_QR_CODE, QQ_MUSIC_DEV_NAME, QQMUSIC_URL_ENCRYPT_AUTH, QQ_MUSIC_APPID, QQ_MUSIC_PACKAGE_NAME, QQ_MUSIC_QRCODE_TYPE, QQ_MUSIC_SIGN_VERSION, current_time);

    result = http_request(QQ_MUSIC_NEW_TEST_NAME,
                          base_data, base_url_data, &response, false);
    if (result == 0) {
        // printf("===result: %s\n", response.memory);
        result = get_qr_code_response(sm, &response);
        // strcpy(sm->qqmusic_openid_authCode, "simulated_authCode");
        // printf("QR Code received: %s\n", sm->qqmusic_openid_authCode);
    }
    free(response.memory);
    return result;
}

//  "encryptString": "code-O30i-Zp5HTHExYjZDnq",
static int check_auth_status_response(StateMachine *sm, struct MemoryStruct *response)
{
    cJSON *root = NULL;
    int res = -1;

    if (response == NULL) {
        printf("No response\n");
        return -1;
    }

    if (response->memory) {
        root = cJSON_Parse(response->memory);
        if (root == NULL) {
            printf("cJSON_Parse failed\n");
            return -1;
        }
        cJSON *ret = cJSON_GetObjectItem(root, "ret");
        res = ret->valueint;
        if (ret->valueint == 0) {
            cJSON *encryptString = cJSON_GetObjectItem(root, "encryptString");
            memcpy(sm->code, encryptString->valuestring, strlen(encryptString->valuestring));
            printf("code: %s\n", sm->code);
            stop_timer(&sm->general_timer_id);  // 停止定时器
        }
    }
    if (root)
        cJSON_Delete(root);
    return res;
}

// 维码授权结果轮询
int check_auth_status(StateMachine *sm)
{
    struct MemoryStruct response;
    char base_data[512] = {0};
    int result = -1;

    time_t current_time = time(NULL);

    if (current_time == ((time_t)-1)) {
        // 错误处理
        fprintf(stderr, "Failed to obtain the current time.\n");
        return -1;
    }

    snprintf(base_data, sizeof(base_data), CUSTOM_QRCODE_AUTH_POLL_DATA, QQ_MUSIC_APPID, sm->ip, QQ_MUSIC_DEVICE_ID, QQ_MUSIC_OPENAPI_CMD_CUSTOM_QRCODE_AUTH_POLL, QQ_MUSIC_APPID, sm->authCode, QQ_MUSIC_SIGN_VERSION, current_time);

    if (http_request(QQ_MUSIC_NEW_TEST_NAME, base_data, NULL, &response, false) == 0) {
        printf("Auth status checked.\n");

        printf("===result: %s\n", response.memory);
        result = check_auth_status_response(sm, &response);
        printf("get result = %d\n", result);
        if (result == 0) {
            // 取到授权码，成功
            // sm->event = EVENT_AUTH_SUCCESS;
            sm->event = EVENT_IDLE;
            sm->is_idle = 1;
            // stop_timer(&sm->general_timer_id);
            // handle_event(sm, );
        }

        free(response.memory);
        return 0;
    }
    free(response.memory);
    return -1;
}

//  "{\"app_id\":\"2000000613\",\"expireTime\":\"1722397123\",\"login_type\":\"1\",\"qqmusic_access_token\":\"auha4f151522d54e71813e394f3f40a940cd8eed3c1d8f483ec1049b1dea4e11ea7\",\"qqmusic_open_id\":\"2562109544643882420\",\"qqmusic_refresh_token\":\"55750f6d9b35d1dcd2595203b74b8b975d6e5f5d8b023740c2e1757b95401ec9\",\"refresh_token_expireTime\":\"1724902723\"}"
static int get_access_token_response(StateMachine *sm, struct MemoryStruct *response)
{
    cJSON *root = NULL;
    int res = -1;

    if (response == NULL) {
        printf("No response\n");
        return -1;
    }

    if (response->memory) {
        root = cJSON_Parse(response->memory);
        if (root == NULL) {
            printf("cJSON_Parse failed\n");
            return -1;
        }
        cJSON *ret = cJSON_GetObjectItem(root, "ret");
        res = ret->valueint;
        if (ret->valueint == 0) {
            cJSON *encryptString = cJSON_GetObjectItem(root, "encryptString");
            cJSON *token_result = cJSON_Parse(encryptString->valuestring);
            if (token_result) {
                cJSON *access_token = cJSON_GetObjectItem(token_result, "qqmusic_access_token");
                cJSON *refresh_token = cJSON_GetObjectItem(token_result, "qqmusic_refresh_token");
                cJSON *qqmusic_open_id = cJSON_GetObjectItem(token_result, "qqmusic_open_id");
                cJSON *expireTime = cJSON_GetObjectItem(token_result, "expireTime");
                cJSON *refresh_token_expireTime = cJSON_GetObjectItem(token_result, "refresh_token_expireTime");

                memcpy(sm->access_token, access_token->valuestring, strlen(access_token->valuestring));
                memcpy(sm->refresh_token, refresh_token->valuestring, strlen(refresh_token->valuestring));
                memcpy(sm->open_id, qqmusic_open_id->valuestring, strlen(qqmusic_open_id->valuestring));
                printf("access_token: %s\n", sm->access_token);
                printf("refresh_token: %s\n", sm->refresh_token);
                printf("open_id: %s\n", sm->open_id);
                // todo 保存到KV表中

                cJSON_Delete(token_result);
            }
        }
    }
    if (root)
        cJSON_Delete(root);
    return res;
}

// 获取和刷新访问令牌
// 这里获取token需要向我们自己的服务器获取token
// deviceId = 1664745890332315648
// authCode = 889330ACE8E248E6900D3DDAECCD5002
// code = code-O7cLlQZIOU3DAKkL4qX
void get_access_token(StateMachine *sm)
{
    struct MemoryStruct response;
    char base_data[1024] = {0};
    int result = 0;

#if 0
    time_t current_time = time(NULL);

    if (current_time == ((time_t)-1)) {
        // 错误处理
        fprintf(stderr, "Failed to obtain the current time.\n");
        return;
    }
    snprintf(base_data, sizeof(base_data), CMD_GET_ACCESS_GET_TOKEN_DATA, QQ_MUSIC_APPID, QQ_MUSIC_APPKEY, sm->ip, sm->code, QQ_MUSIC_DEVICE_ID, QQ_MUSIC_OPENAPI_CMD_AUTH_GET_ACCESSTOKEN, QQ_MUSIC_SIGN_VERSION, current_time);

    if (http_request(QQ_MUSIC_NEW_TEST_NAME, base_data, NULL, &response, false) == 0) {
        printf("===result: %s\n", response.memory);
        result = get_access_token_response(sm, &response);
        if (result != 0) {
            sm->event = EVENT_RESTART_QR_CODE;
        }
        // strcpy(sm->access_token, "simulated_access_token");
        // strcpy(sm->refresh_token, "simulated_refresh_token");
        // printf("Access token received: %s\n", sm->access_token);
    }
#else
    printf("start get access token\n");
    uint8_t des = 0;
    int message_body_len = 0;
    cJSON *root = NULL;
    char post_url[256] = "http://deve.nbhope.cn:8889/hopeApi/qqmusic/tokenByCode";

    root = cJSON_CreateObject();
    // TEST_ASSERT_NOT_NULL(root);
    if(root == NULL) goto clean;

    cJSON_AddStringToObject(root, "authCode", "889330ACE8E248E6900D3DDAECCD5002");
    cJSON_AddStringToObject(root, "deviceId", "1664745890332315648");
    cJSON_AddStringToObject(root, "code", "code-O7cLlQZIOU3DAKkL4qX");   //
    char *out = cJSON_PrintUnformatted(root);
    // TEST_ASSERT_OBJECT_NOT_NULL(out,  ret, Cleanup);
    if(out == NULL) goto clean;

    // free(out);
    printf("out: %s\n", out);

    message_body_len = get_strlen(out);
    des = calc_des(message_body_len);
    printf("len: %d, des: %x\n", message_body_len, des);
    des = 4;

    snprintf(base_data, MAX_URL_REQUEST_LEN, "ver=%s&des=%02X&cid=%s&sid=%s&key=%s&len=%d&dat=%s", APP_DEFAULT_HTTP_VERSION, des,
             APP_DEFAULT_HTTP_CID, APP_DEFAULT_HTTP_SID, APP_DEFAULT_HTTP_KEY, message_body_len, out);
    // snprintf(post_url, MAX_URL_HOST_LEN, "%s://%s:%d/hopeApi/qqmusic/tokenByCode", APP_DEFAULT_HTTP_PREFIX,
    //          gwContext.httpSrvHost, gwContext.httpSrvPort);
    if (http_request_from_hope(post_url, base_data, &response, true) == 0) {
        printf("===result: %s\n", response.memory);
        // result = get_access_token_response(sm, &response);
        // if (result != 0) {
        //     sm->event = EVENT_RESTART_QR_CODE;
        // }
        // strcpy(sm->access_token, "simulated_access_token");
        // strcpy(sm->refresh_token, "simulated_refresh_token");
        // printf("Access token received: %s\n", sm->access_token);
    }

clean:
    if(out)
        free(out);

    if (root) {
        cJSON_Delete(root);
    }
#endif
    free(response.memory);
}

// 刷新令牌
int refresh_token(StateMachine *sm)
{
    struct MemoryStruct response;
    char base_data[512] = {0};
    int result = 0;

#if 0 
    time_t current_time = time(NULL);

    if (current_time == ((time_t)-1)) {
        // 错误处理
        fprintf(stderr, "Failed to obtain the current time.\n");
        return;
    }

    snprintf(base_data, sizeof(base_data), CMD_GET_ACCESS_GET_REFRESH_TOKEN_DATA, QQ_MUSIC_APPID, QQ_MUSIC_APPKEY, sm->ip, sm->code, QQ_MUSIC_DEVICE_ID, QQ_MUSIC_OPENAPI_CMD_AUTH_GET_ACCESSTOKEN, sm->refresh_token, QQ_MUSIC_SIGN_VERSION, current_time);

    if (http_request(QQ_MUSIC_NEW_TEST_NAME, base_data, NULL, &response, false) == 0) {
        printf("===result: %s\n", response.memory);
        result = get_access_token_response(sm, &response);
        if (result != 0) {
            sm->event = EVENT_RESTART_QR_CODE;
        }
        // strcpy(sm->access_token, "new_simulated_access_token");
        // printf("Access token refreshed: %s\n", sm->access_token);
    }
#else //
     printf("start get access token\n");
    uint8_t des = 0;
    int message_body_len = 0;
    cJSON *root = NULL;
    char post_url[256] = "http://deve.nbhope.cn:8889/hopeApi/musicAccount/info";

    root = cJSON_CreateObject();
    // TEST_ASSERT_NOT_NULL(root);
    if(root == NULL) goto clean;

    cJSON_AddStringToObject(root, "authCode", "889330ACE8E248E6900D3DDAECCD5002");
    cJSON_AddStringToObject(root, "deviceId", "1664745890332315648");
    cJSON_AddStringToObject(root, "musicType", "qqmusic");   //
    char *out = cJSON_PrintUnformatted(root);
    // TEST_ASSERT_OBJECT_NOT_NULL(out,  ret, Cleanup);
    if(out == NULL) goto clean;

    // free(out);
    printf("out: %s\n", out);

    message_body_len = get_strlen(out);
    des = calc_des2(message_body_len);
    printf("len: %d, des: %x\n", message_body_len, des);
    // des = 0x7B;

    snprintf(base_data, MAX_URL_REQUEST_LEN, "ver=%s&des=%02X&cid=%s&sid=%s&key=%s&len=%d&dat=%s", APP_DEFAULT_HTTP_VERSION, des,
             APP_DEFAULT_HTTP_CID, APP_DEFAULT_HTTP_SID, APP_DEFAULT_HTTP_KEY, message_body_len, out);
    // snprintf(post_url, MAX_URL_HOST_LEN, "%s://%s:%d/hopeApi/qqmusic/tokenByCode", APP_DEFAULT_HTTP_PREFIX,
    //          gwContext.httpSrvHost, gwContext.httpSrvPort);
    if (http_request_from_hope(post_url, base_data, &response, true) == 0) {
        printf("===result: %s\n", response.memory);
        // result = get_access_token_response(sm, &response);
        // if (result != 0) {
        //     sm->event = EVENT_RESTART_QR_CODE;
        // }
        // strcpy(sm->access_token, "simulated_access_token");
        // strcpy(sm->refresh_token, "simulated_refresh_token");
        // printf("Access token received: %s\n", sm->access_token);
    }
clean:
    if(out)
        free(out);

    if (root) {
        cJSON_Delete(root);
    }
#endif
    free(response.memory);

    return result;
}

// 定时器回调函数：处理二维码超时和轮询
void general_timer_timeout_handler(union sigval arg)
{
    StateMachine *sm = (StateMachine *)arg.sival_ptr;

    if (sm->total_poll_count >= 300) {  // 超过300次轮询
        sm->event = EVENT_TIMEOUT;
        sm->is_idle = 0;
        // handle_event(sm, );
    }
    else {
        sm->total_poll_count++;
        if (check_auth_status(sm) != 0) {
            printf("QR Code polling failed. Retrying...\n");
            // set_timer(&sm->general_timer_id, 5, general_timer_timeout_handler, sm);  // 5秒轮询一次
        }
    }
}

// 定时器回调函数：处理令牌过期
void token_timer_timeout_handler(union sigval arg)
{
    StateMachine *sm = (StateMachine *)arg.sival_ptr;
    sm->token_expired_count++;

    if (sm->token_expired_count % 30 == 0) {
        sm->event = EVENT_REFRESH_TOKEN_EXPIRED;
        sm->is_idle = 0;
        // handle_event(sm, );
    }
    else {
        sm->event = EVENT_TOKEN_EXPIRED;
        sm->is_idle = 0;
        // handle_event(sm, );
    }
}

// 停止定时器
void stop_timer(timer_t *timer_id)
{
    struct itimerspec its;
    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 0;
    timer_settime(*timer_id, 0, &its, NULL);
}

// 设置定时器
void set_timer(timer_t *timer_id, int seconds, void (*handler)(union sigval), StateMachine *sm)
{
    struct sigevent sev;
    struct itimerspec its;

    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_value.sival_ptr = sm;
    sev.sigev_notify_function = handler;
    sev.sigev_notify_attributes = NULL;
    timer_create(CLOCK_REALTIME, &sev, timer_id);

    its.it_value.tv_sec = seconds;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = seconds;
    its.it_interval.tv_nsec = 0;
    timer_settime(*timer_id, 0, &its, NULL);
}

// 事件处理
void handle_event(StateMachine *sm)
{
    Event event = sm->event;
    if (sm->is_idle && event != EVENT_RESTART_QR_CODE) {
        return;  // 在待机状态时，仅响应重启二维码事件
    }
    // printf("event = %d\n", sm->event);
    switch (event) {
        case EVENT_QR_CODE_RECEIVED:
            sm->total_poll_count = 0;                                                // 重置轮询计数器
            set_timer(&sm->general_timer_id, 5, general_timer_timeout_handler, sm);  // 5秒轮询一次
            sm->event = EVENT_IDLE;
            break;
        case EVENT_QR_CODE_FAILED:
            if (sm->qr_code_attempts < 3) {
                printf("QR Code attempt %d failed. Trying again.\n", sm->qr_code_attempts + 1);
                sm->qr_code_attempts++;
                sm->event = EVENT_RESTART_QR_CODE;
                // handle_event(sm, EVENT_RESTART_QR_CODE);
            }
            else {
                printf("QR Code failed 3 times. Stopping attempts and entering idle state.\n");
                sm->event = EVENT_IDLE;
            }
            break;
        case EVENT_TIMEOUT:
            printf("QR Code timeout. Restarting QR Code process.\n");
            sm->qr_code_attempts = 0;
            sm->event = EVENT_RESTART_QR_CODE;
            // handle_event(sm, );
            break;
        case EVENT_AUTH_SUCCESS:
            // 停止处理二维码超时和轮询的定时器
            printf("start get token\n");
            stop_timer(&sm->general_timer_id);
            get_access_token(sm);
            sm->event = EVENT_TOKEN_RECEIVED;
            // handle_event(sm, );
            break;
        case EVENT_TOKEN_RECEIVED:
            printf("stop get token\n");
            // 设置定时器以处理访问令牌过期
            // 还是需要通过时间戳判断是否过期
            set_timer(&sm->token_timer_id, 24 * 3600, token_timer_timeout_handler, sm);  // 24小时
            sm->token_expired_count = 0;                                                 // 重置 token 过期事件计数器
            sm->event = EVENT_IDLE;
            // sm->is_idle = 1;  // 进入待机状态
            break;
        case EVENT_TOKEN_EXPIRED:
            printf("Access token expired. Refreshing token.\n");
            refresh_token(sm);
            break;
        case EVENT_REFRESH_TOKEN_EXPIRED:
            printf("Refresh token expired. Restarting QR Code process\n");
            sm->qr_code_attempts = 0;
            sm->event = EVENT_RESTART_QR_CODE;
            // handle_event(sm, );
            break;
        case EVENT_RESTART_QR_CODE:
            sm->is_idle = 0;
            if (get_qr_code(sm) == 0) {
                sm->event = EVENT_QR_CODE_RECEIVED;
                // handle_event(sm, );
            }
            else {
                sm->event = EVENT_QR_CODE_FAILED;
                // handle_event(sm, );
            }
            break;
        case EVENT_IDLE:
            sm->is_idle = 1;
            printf("System is now idle.\n");
            break;
    }
}

static int get_config(const char *key, char *value)
{
    return -1;
}

// https://test.szhkeji.com/rpc_proxy/music_open_api?app_id=2000002621&client_ip=192.168.110.211&device_id=1662620792859168768&opi_cmd=fcg_music_custom_sdk_get_qr_code.fcg&qqmusic_dev_name=hope&qqmusic_encrypt_auth=%7B%22response_type%22%3A%22code%22%2C%22state%22%3A%2210086%22%7D&qqmusic_open_appid=2000002621&qqmusic_package_name=test.sizheng.com&qqmusic_qrcode_type=universal&sign_version=v2&timestamp=1727230958&sign=5f85ea8fcfa8adbdc3a2aab8346b2bc0

int main()
{
    StateMachine sm = {0};
    sm.qr_code_attempts = 0;
    sm.token_expired_count = 0;
    sm.is_idle = 0;
    sm.total_poll_count = 0;

    char *ip = get_external_ip();
    printf("ip: %s\n", ip);
    // return 1;

    if (ip) {
        memcpy(sm.ip, ip, strlen(ip));
    }
    strcpy(sm.ip, "60.179.179.6");
    printf("sm ip: %s\n", sm.ip);

    curl_global_init(CURL_GLOBAL_ALL);
#if 0
    // 先判断refresh_token是否过期
    // todo 从KV表中获取保存的refresh token
    if (get_config("refresh_token", sm.refresh_token) == 0) {
        printf("refresh_token: %s\n", sm.refresh_token);
        sm.event = EVENT_TOKEN_EXPIRED;
    }
    else {
        sm.event = EVENT_RESTART_QR_CODE;
    }

    // 主循环，保持程序运行
    while (1) {
        handle_event(&sm);

        // 系统的消息处理
        // todo 收到退出二维码显示界面的消息,就将任务设置为空闲状态
        // todo 收到进入二维码界面需要显示二维码的时候,获取二维码并显示
        // todo 如果已经授权登陆,那就将任务设置为空闲状态

        // todo 每天定期去获取歌单,获取歌曲信息
    }
#else 
    refresh_token(NULL);
#endif
    // // 退出主循环，释放资源
    // stop_timer(&sm.general_timer_id);
    // stop_timer(&sm.token_timer_id);

    curl_global_cleanup();
    free(ip);

    // 清理动态内存
    printf("Cleaning up and exiting.\n");

    return 0;
}

#if 0
int main()
{
    char *result = NULL;

    char *ip = get_external_ip();
    printf("ip: %s\n", ip);

    result = qqmusic_openapi_get_qrcode(ip);
    if (result) {
        printf("result: %s, len = %d\n", result, strlen(result));
        cJSON *root = cJSON_Parse(result);
        free(result);
        result = NULL;
        if (root) {
            cJSON *auth_code = cJSON_GetObjectItem(root, "auth_code");
            if (auth_code) {
                result = qqmusic_openapi_qrcode_auth_poll(ip, auth_code->valuestring);
                printf("result: %s\n", result);
            }
        }
        if (root) {
            cJSON_Delete(root);
        }
        free(result);
        result = NULL;
    }

    // free(result_data);
    free(result);

    free(ip);

    return 0;
}
#endif
