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
 * @Date         : 2024-07-25 10:43:00
 * @LastEditors  : MCD
 * @LastEditTime : 2024-07-26 13:10:28
 * @FilePath     : /My_C_Test/libqqmusic_openapi/qq_music_openapi.h
 * @Description  : 
 * 
 * ******************************************
 */

#ifndef __QQ_MUSIC_OPENAPI_H__
#define __QQ_MUSIC_OPENAPI_H__

// 需要释放返回数据的内存
char *qqmusic_openapi_get_qrcode(const char *ip);
char *qqmusic_openapi_qrcode_auth_poll(const char *ip, const char *auth_code);

#endif  // !__QQ_MUSIC_OPENAPI_H__