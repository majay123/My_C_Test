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
 * @Date         : 2023-03-21 14:59:07
 * @LastEditors  : MCD
 * @LastEditTime : 2023-03-21 15:29:49
 * @FilePath     : /My_C_Test/Hope_Work/leshi/include/common.h
 * @Description  : 
 * 
 * ******************************************
 */

#ifndef __COMMON_H_
#define __COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif
// #include "leveldb/c.h"
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
// #include <uci.h>
#include <stdint.h>
#include <unistd.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr)[0])
#define NELEM(x)      ((int)(sizeof(x) / sizeof((x)[0])))
#endif

#ifndef TRUE
#define TRUE (1 == 1)
#endif

#ifndef FALSE
#define FALSE (!TRUE)
#endif

//define the safety strlen
#ifndef strlens
#define strlens(s) (s == NULL ? 0 : strlen((char *)(s)))
#else
#error "strlens conflict build give up"
#endif

//define the safety strdup
#ifndef strdups
#define strdups(s) ((s) == NULL ? NULL : strdup((char *)(s)))
#else
#error "strdups conflict build give up"
#endif

#define BREAK_UINT32(var, ByteNum) \
    (uint8_t)((uint32_t)(((var) >> ((ByteNum)*8)) & 0x00FF))

#define BUILD_UINT32(Byte0, Byte1, Byte2, Byte3) \
    ((uint32_t)((uint32_t)((Byte0)&0x00FF) + ((uint32_t)((Byte1)&0x00FF) << 8) + ((uint32_t)((Byte2)&0x00FF) << 16) + ((uint32_t)((Byte3)&0x00FF) << 24)))

#define BUILD_UINT16(loByte, hiByte) \
    ((uint16_t)(((loByte)&0x00FF) + (((hiByte)&0x00FF) << 8)))

#define HI_UINT16(a) (((a) >> 8) & 0xFF)
#define LO_UINT16(a) ((a)&0xFF)

#define LO_UINT24(a) ((a)&0xFFF)

#define BUILD_UINT8(hiByte, loByte) \
    ((uint8_t)(((loByte)&0x0F) + (((hiByte)&0x0F) << 4)))

#define HI_UINT8(a) (((a) >> 4) & 0x0F)
#define LO_UINT8(a) ((a)&0x0F)

#define REQUIRE(in, tag)                   \
    do {                                   \
        if (in) {                          \
            printf("%s %s\n", #tag, #in); \
            goto tag;                      \
        }                                  \
    } while (0)

#define REQUIRE_NOLOG(in, tag)             \
    do {                                   \
        if (in) {                          \
            printf("%s %s\n", #tag, #in); \
            goto tag;                      \
        }                                  \
    } while (0)

#define REQ_JSON_OBJ(des, item, tag) \
    cJSON_GetObjectItem(des, #item); \
    REQUIRE((item == NULL), tag)
    
#define REQ_JSON_PARSE(str, item, tag) \
    cJSON_Parse(str);                  \
    REQUIRE((item == NULL), tag)

#define MY_RELLOC(data, num, type)            \
    do {                                      \
        if (data) {                           \
            free(data);                       \
            data = NULL;                      \
        }                                     \
        if (num > 0) {                        \
            data = calloc(num, sizeof(type)); \
        }                                     \
    } while (0)

#define MY_RELLOC_REQUIRE(data, num, type, tag)             \
    do {                                                    \
        MY_RELLOC(data, num, type);                         \
        if (!data) {                                        \
            printf("%s %s malloc is null\n", #tag, #data); \
            goto tag;                                       \
        }                                                   \
    } while (0)

// 该位称为预置值，使用人工算法（长除法）时
//需要将除数多项式先与该与职位 异或 ，才能得到最后的除数多项式
#define CRC16_CCITT_SEED (0x0000)
// 该位为简式书写 实际为0x11021
#define CRC16_CCITT_POLY16 (0x1021)

typedef struct
{
    int type;  //command
    size_t length;
    char *value;
    void *callback;
    char *param;
    long serial;
} commonMsg_t;

typedef void (*_function)(sigval_t sig);
timer_t setTimer(size_t timeout_value, unsigned int repeat, _function func, void *param);
int killTimer(timer_t *timer_id);
int resetTimer(timer_t *timer_id, size_t timeout_value);
void select_sleep(int seconds);
void select_usleep(int micro_seconds);

char *
bin2hex_s(char *const hex, const size_t hex_maxlen, const unsigned char *const bin, const size_t bin_len, int upper_case);

int hex2bin_s(unsigned char *const bin, const size_t bin_maxlen, const char *const hex, const size_t hex_len, const char *const ignore, size_t *const bin_len, const char **const hex_end);
#if 0
// support search from user defined path
int uci_config_get(char *path, char *package, char *section, char *option, char *pdata, size_t plen);
int uci_config_set(char *path, char *package, char *section, char *option, char *pdata);
int kv_config_init(const char *dbname);
int kv_config_set(const char *key, const char *v, size_t vlen);
int kv_config_get(const char *key, char *v, size_t vlen);
int kv_config_reset(const char *key);
int kv_config_exit(void);
#endif

int get_devIntAttr_value(char *attrValue, int attrValueLen);
char *get_devStringAttr_value(char *attrValue, int attrValueLen);
int hexStr2bytes(const uint8_t *hexStr, uint8_t *buf, int bufLen);
int bytes2hexStr(const uint8_t *buf, char *hexStr, int bufLen);
unsigned short crc16_ccitt(unsigned char *puchMsg, unsigned int usDataLen);
void dump_rs485_data(uint8_t *data, size_t size);
int sys_get_mac(char *mac, size_t maclen);
int my_itoa(uint8_t *buf, uint32_t value, size_t size);
uint32_t bytetol(uint8_t *buffer, size_t size);
char *strlwr(char *s);
void hextodecstring(uint8_t *src_data, size_t len, uint8_t *dest_data);
uint8_t *dump_rs485_data1(uint8_t *src, size_t ssize);  // need free
uint8_t check_sum_ls(uint8_t *src, size_t ssize);

#ifdef __cplusplus
}
#endif

#endif /*__COMMON_H_*/
