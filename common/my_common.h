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
 * ************Copyright 2021 MCD************
 * 
 * @version      : 
 * @Company      : HOPE
 * @Author       : MCD
 * @Date         : 2021-09-16 09:00:19
 * @LastEditors  : MCD
 * @LastEditTime : 2021-09-16 09:02:30
 * @FilePath     : /My_C_Test/common/my_common.h
 * @Description  : 
 * 
 * ******************************************
 */

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
 * ************Copyright 2021 MCD************
 * 
 * @version      : 
 * @Company      : HOPE
 * @Author       : MCD
 * @Date         : 2021-08-24 10:15:47
 * @LastEditors  : MCD
 * @LastEditTime : 2021-08-28 12:35:12
 * @FilePath     : /actroot/services/eide_light/app/common/common.h
 * @Description  : 
 * 
 * ******************************************
 */

#ifndef __COMMON_H_
#define __COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
// #include <uci.h>
// #include "leveldb/c.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr)[0])
#endif

#define BREAK_UINT32(var, ByteNum) \
    (uint8_t)((uint32_t)(((var) >> ((ByteNum)*8)) & 0x00FF))

#define BUILD_UINT32(Byte0, Byte1, Byte2, Byte3) \
    ((uint32_t)((uint32_t)((Byte0)&0x00FF) + ((uint32_t)((Byte1)&0x00FF) << 8) + ((uint32_t)((Byte2)&0x00FF) << 16) + ((uint32_t)((Byte3)&0x00FF) << 24)))

#define BUILD_UINT16(loByte, hiByte) \
    ((uint16_t)(((loByte)&0x00FF) + (((hiByte)&0x00FF) << 8)))

#define HI_UINT16(a) (((a) >> 8) & 0xFF)
#define LO_UINT16(a) ((a)&0xFF)

#define BUILD_UINT8(hiByte, loByte) \
    ((uint8_t)(((loByte)&0x0F) + (((hiByte)&0x0F) << 4)))

#define HI_UINT8(a) (((a) >> 4) & 0x0F)
#define LO_UINT8(a) ((a)&0x0F)

// 该位称为预置值，使用人工算法（长除法）时
//需要将除数多项式先与该与职位 异或 ，才能得到最后的除数多项式
#define CRC16_CCITT_SEED            (0x0000)
// 该位为简式书写 实际为0x11021
#define CRC16_CCITT_POLY16          (0x1021)

typedef struct
{
    int type; //command
    size_t length;
    char *value;
    void *callback;
    char *param;
    long serial;
} commonMsg_t;

typedef void (*_function)(sigval_t sig);
timer_t setTimer(size_t timeout_value, unsigned int repeat, _function func, void *param);
int killTimer(timer_t timer_id);
void select_sleep(int seconds);
void select_usleep(int micro_seconds);
char *strlwr(char *s);
char *strupr(char *s);
int hexStr2bytes(const uint8_t *hexStr, uint8_t *buf, int bufLen);
unsigned short crc16_ccitt_tab(unsigned char *pbuf, size_t len);
unsigned short crc16_ccitt(unsigned char *puchMsg, unsigned int usDataLen);
int my_itoa(char *buf, int value, char size);

// support
#ifdef __cplusplus
}
#endif

#endif /*__COMMON_H_*/
