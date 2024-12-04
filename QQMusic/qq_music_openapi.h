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
 * @Date         : 2024-07-25 10:46:47
 * @LastEditors  : MCD
 * @LastEditTime : 2024-10-07 14:20:08
 * @FilePath     : /My_C_Test/QQMusic/qq_music_openapi.h
 * @Description  : 
 * 
 * ******************************************
 */


#ifndef __QQ_MUSIC_OPENAPI_H__
#define __QQ_MUSIC_OPENAPI_H__

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define QQ_MUSIC_APPID        "2000002621"
#define QQ_MUSIC_APPKEY       "NdeQijOnksQumgfC"
#define QQ_MUSIC_PACKAGE_NAME "test.szhkeji.com"
// #define QQ_MUSIC_DEVICE_ID    "50140406070131"  // 这个可能是SN号,是会变化的
#define QQ_MUSIC_DEVICE_ID    "1664745890332315648"  // 这个可能是SN号,是会变化的
#define QQ_MUSIC_QRCODE_TYPE  "universal"       //支持QQ音乐/QQ/微信扫码
#define QQ_MUSIC_SIGN_VERSION "v2"
#define QQ_MUSIC_DEV_NAME     "hope"  // 这个可能会变

// 域名
#define QQ_MUSIC_DOMAIN_NAME           "https://qplaycloud.y.qq.com/rpc_proxy/fcgi-bin/music_open_api.fcg"
#define QQ_MUSIC_SHENGZHEN_DOMAIN_NAME "https://opensz.music.qq.com/rpc_proxy/fcgi-bin/music_open_api.fcg"
#define QQ_MUSIC_SHANGHAI_DOMAIN_NAME_ "https://opensh.music.qq.com/rpc_proxy/fcgi-bin/music_open_api.fcg"
#define QQ_MUSIC_TEST_NAME             "https://test.y.qq.com/opentest/rpc_proxy/fcgi-bin/music_open_api.fcg"
#define QQ_MUSIC_NEW_TEST_NAME         "https://test.szhkeji.com/rpc_proxy/music_open_api"

// 登录与授权
#define QQ_MUSIC_OPENAPI_CMD_CUSTOM_SDK_GET_QR_CODE  "fcg_music_custom_sdk_get_qr_code.fcg"
#define QQ_MUSIC_OPENAPI_CMD_CUSTOM_QRCODE_AUTH_POLL "fcg_music_custom_qrcode_auth_poll.fcg"
#define QQ_MUSIC_OPENAPI_CMD_AUTH_GET_ACCESSTOKEN    "fcg_music_oauth_get_accesstoken.fcg"
#define QQ_MUSIC_OPENAPI_CMD_CUSTOM_BIND_ACCOUNT     "fcg_music_custom_bind_account.fcg"
#define QQ_MUSIC_OPENAPI_CMD_CUSTOM_DELETE_ACCOUNT   "fcg_music_custom_delete_account.fcg"
#define QQ_MUSIC_OPENAPI_CMD_CUSTOM_QUERY_ACCOUNT    "fcg_music_custom_query_account.fcg"
#define QQ_MUSIC_OPENAPI_CMD_CUSTOM_GET_ACCOUNT_INFO "fcg_music_custom_get_account_info.fcg"
#define QQ_MUSIC_OPENAPI_CMD_CUSTOM_GET_SONG_INFO_BATCH    "fcg_music_custom_get_song_info_batch.fcg"
#define QQ_MUSIC_OPENAPI_CMD_CUSTOM_SEARCH    "fcg_music_custom_search.fcg"

#define QQMUSIC_ENCRYPT_AUTH     "{\"response_type\":\"code\",\"state\":\"123456\"}"
#define QQMUSIC_URL_ENCRYPT_AUTH "%7B%22response_type%22%3A%22code%22%2C%22state%22%3A%22123456%22%7D"

#define CUSTOM_SDK_GET_QR_CODE_DATA "app_id=%s&client_ip=%s&device_id=%s&opi_cmd=%s&qqmusic_dev_name=%s&qqmusic_encrypt_auth=%s&qqmusic_open_appid=%s&qqmusic_package_name=%s&qqmusic_qrcode_type=%s&sign_version=%s&timestamp=%ld"
// app_id=2000000613&client_ip=103.149.249.27&device_id=88056404100172&opi_cmd=fcg_music_custom_sdk_get_qr_code.fcg&qqmusic_dev_name=hope&qqmusic_encrypt_auth={"response_type":"code","state":"123456"}&qqmusic_open_appid=2000000613&qqmusic_package_name=test.sizheng.com&qqmusic_qrcode_type=app&sign_version=v2&timestamp=1721799849

#define CUSTOM_QRCODE_AUTH_POLL_DATA "app_id=%s&client_ip=%s&device_id=%s&opi_cmd=%s&qqmusic_openid_appId=%s&qqmusic_openid_authCode=%s&sign_version=%s&timestamp=%ld"
// app_id=2000000613&client_ip=103.149.249.27&device_id=88056404100172&opi_cmd=fcg_music_custom_qrcode_auth_poll.fcg&qqmusic_openid_appId=2000000613&qqmusic_openid_authCode=auh-O281jsl0mup2GlTTFRoa&sign_version=v2&timestamp=1721805803

#define CMD_GET_ACCESS_GET_TOKEN_DATA         "app_id=%s&app_key=%s&client_ip=%s&cmd=getToken&code=%s&device_id=%s&opi_cmd=%s&sign_version=%s&timestamp=%ld"
#define CMD_GET_ACCESS_GET_REFRESH_TOKEN_DATA "app_id=%s&app_key=%s&client_ip=%s&cmd=refreshToken&code=%s&device_id=%s&opi_cmd=%s&qqmusic_refresh_token=%s&sign_version=%s&timestamp=%ld"

#define CUSTOM_GET_SONG_INFO_BATCH "app_id=%s&client_ip=%s&device_id=%s&login_type=6&opi_cmd=%s&qqmusic_access_token=%s&qqmusic_open_appid=%s&qqmusic_open_id=%s&sign_version=v2&song_mid=%s&timestamp=%ld"

#define CUSTOM_SEARCH "app_id=%s&client_ip=%s&device_id=%s&login_type=6&num=30&opi_cmd=%s&qqmusic_access_token=%s&qqmusic_open_appid=%s&qqmusic_open_id=%s&sign_version=v2&timestamp=%ld&w=%s"

// char *qqmusic_openapi_get_qrcode(const char *ip);
// char *qqmusic_openapi_qrcode_auth_poll(const char *ip, const char *auth_code);


#endif  // !__QQ_MUSIC_OPENAPI_H__