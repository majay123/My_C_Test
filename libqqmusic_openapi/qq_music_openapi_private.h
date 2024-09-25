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
 * @Date         : 2024-07-25 11:14:27
 * @LastEditors  : MCD
 * @LastEditTime : 2024-07-30 10:10:11
 * @FilePath     : /My_C_Test/libqqmusic_openapi/qq_music_openapi_private.h
 * @Description  : 
 * 
 * ******************************************
 */
#ifndef __QQ_MUSIC_OPENAPI_PRIVATE_H__
#define __QQ_MUSIC_OPENAPI_PRIVATE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <curl/curl.h>
#include <openssl/md5.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define QQ_MUSIC_APPID        "2000000613"
#define QQ_MUSIC_APPKEY       "MDBcwRNlvchdGoNr"
#define QQ_MUSIC_PACKAGE_NAME "test.sizheng.com"
#define QQ_MUSIC_DEVICE_ID    "88056404100172"  // 这个可能是SN号,是会变化的
#define QQ_MUSIC_QRCODE_TYPE  "universal"       //支持QQ音乐/QQ/微信扫码
#define QQ_MUSIC_SIGN_VERSION "v2"
#define QQ_MUSIC_DEV_NAME     "hope"  // 这个可能会变

// 域名
#define QQ_MUSIC_DOMAIN_NAME           "https://qplaycloud.y.qq.com/rpc_proxy/fcgi-bin/music_open_api.fcg"
#define QQ_MUSIC_SHENGZHEN_DOMAIN_NAME "https://opensz.music.qq.com/rpc_proxy/fcgi-bin/music_open_api.fcg"
#define QQ_MUSIC_SHANGHAI_DOMAIN_NAME_ "https://opensh.music.qq.com/rpc_proxy/fcgi-bin/music_open_api.fcg"
#define QQ_MUSIC_TEST_NAME             "https://test.y.qq.com/opentest/rpc_proxy/fcgi-bin/music_open_api.fcg"

// 登录与授权
#define QQ_MUSIC_OPENAPI_CMD_CUSTOM_SDK_GET_QR_CODE  "fcg_music_custom_sdk_get_qr_code.fcg"
#define QQ_MUSIC_OPENAPI_CMD_CUSTOM_QRCODE_AUTH_POLL "fcg_music_custom_qrcode_auth_poll.fcg"
#define QQ_MUSIC_OPENAPI_CMD_AUTH_GET_ACCESSTOKEN    "fcg_music_oauth_get_accesstoken.fcg"
#define QQ_MUSIC_OPENAPI_CMD_CUSTOM_BIND_ACCOUNT     "fcg_music_custom_bind_account.fcg"
#define QQ_MUSIC_OPENAPI_CMD_CUSTOM_DELETE_ACCOUNT   "fcg_music_custom_delete_account.fcg"
#define QQ_MUSIC_OPENAPI_CMD_CUSTOM_QUERY_ACCOUNT    "fcg_music_custom_query_account.fcg"
#define QQ_MUSIC_OPENAPI_CMD_CUSTOM_GET_ACCOUNT_INFO "fcg_music_custom_get_account_info.fcg"

#define QQMUSIC_ENCRYPT_AUTH        "{\"response_type\":\"code\",\"state\":\"123456\"}"
#define CUSTOM_SDK_GET_QR_CODE_DATA "app_id=%s&client_ip=%s&device_id=%s&opi_cmd=%s&qqmusic_dev_name=%s&qqmusic_encrypt_auth=%s&qqmusic_open_appid=%s&qqmusic_package_name=%s&qqmusic_qrcode_type=%s&sign_version=%s&timestamp=%ld"
// app_id=2000000613&client_ip=103.149.249.27&device_id=88056404100172&opi_cmd=fcg_music_custom_sdk_get_qr_code.fcg&qqmusic_dev_name=hope&qqmusic_encrypt_auth={"response_type":"code","state":"123456"}&qqmusic_open_appid=2000000613&qqmusic_package_name=test.sizheng.com&qqmusic_qrcode_type=app&sign_version=v2&timestamp=1721799849

#define CUSTOM_QRCODE_AUTH_POLL_DATA "app_id=%s&client_ip=%s&device_id=%s&opi_cmd=%s&qqmusic_openid_appId=%s&qqmusic_openid_authCode=%s&sign_version=%s&timestamp=%ld"
// app_id=2000000613&client_ip=103.149.249.27&device_id=88056404100172&opi_cmd=fcg_music_custom_qrcode_auth_poll.fcg&qqmusic_openid_appId=2000000613&qqmusic_openid_authCode=auh-O281jsl0mup2GlTTFRoa&sign_version=v2&timestamp=1721805803

#define CMD_GET_ACCESS_GET_TOKEN_DATA "app_id=%d&app_key=%s&client_ip=%s&cmd=getToken&code=%s&device_id=%s&opi_cmd=%s&sign_version=%s&timestamp=%ld"
#define CMD_GET_ACCESS_GET_REFRESH_TOKEN_DATA "app_id=%d&app_key=%s&client_ip=%s&cmd=refreshToken&device_id=%s&opi_cmd=%s&sign_version=%s&timestamp=%ld"

struct MemoryStruct {
    char *memory;
    size_t size;
    size_t realsize;
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

    mem->realsize = realsize;

    return realsize;
}

#ifdef __cplusplus
}
#endif

#endif /* __QQ_MUSIC_OPENAPI_PRIVATE_H__ */