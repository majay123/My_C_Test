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
 * @Date         : 2022-08-09 09:48:32
 * @LastEditors  : MCD
 * @LastEditTime : 2023-09-21 11:12:53
 * @FilePath     : /My_C_Test/CRC8/crc8func.c
 * @Description  :
 *
 * ******************************************
 */

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static uint8_t CRC8_Table[256] = {
    0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15, 0x38, 0x3F,
    0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D, 0x70, 0x77, 0x7E, 0x79,
    0x6C, 0x6B, 0x62, 0x65, 0x48, 0x4F, 0x46, 0x41, 0x54, 0x53,
    0x5A, 0x5D, 0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5,
    0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD, 0x90, 0x97,
    0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85, 0xA8, 0xAF, 0xA6, 0xA1,
    0xB4, 0xB3, 0xBA, 0xBD, 0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC,
    0xD5, 0xD2, 0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,
    0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2, 0x8F, 0x88,
    0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A, 0x27, 0x20, 0x29, 0x2E,
    0x3B, 0x3C, 0x35, 0x32, 0x1F, 0x18, 0x11, 0x16, 0x03, 0x04,
    0x0D, 0x0A, 0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42,
    0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A, 0x89, 0x8E,
    0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C, 0xB1, 0xB6, 0xBF, 0xB8,
    0xAD, 0xAA, 0xA3, 0xA4, 0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2,
    0xEB, 0xEC, 0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,
    0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C, 0x51, 0x56,
    0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44, 0x19, 0x1E, 0x17, 0x10,
    0x05, 0x02, 0x0B, 0x0C, 0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A,
    0x33, 0x34, 0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B,
    0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63, 0x3E, 0x39,
    0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B, 0x06, 0x01, 0x08, 0x0F,
    0x1A, 0x1D, 0x14, 0x13, 0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5,
    0xBC, 0xBB, 0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
    0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB, 0xE6, 0xE1,
    0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3};

static const unsigned char crc_table[] =
    {
        0x00, 0x31, 0x62, 0x53, 0xc4, 0xf5, 0xa6, 0x97, 0xb9, 0x88, 0xdb, 0xea, 0x7d, 0x4c, 0x1f, 0x2e,

        0x43, 0x72, 0x21, 0x10, 0x87, 0xb6, 0xe5, 0xd4, 0xfa, 0xcb, 0x98, 0xa9, 0x3e, 0x0f, 0x5c, 0x6d,

        0x86, 0xb7, 0xe4, 0xd5, 0x42, 0x73, 0x20, 0x11, 0x3f, 0x0e, 0x5d, 0x6c, 0xfb, 0xca, 0x99, 0xa8,

        0xc5, 0xf4, 0xa7, 0x96, 0x01, 0x30, 0x63, 0x52, 0x7c, 0x4d, 0x1e, 0x2f, 0xb8, 0x89, 0xda, 0xeb,

        0x3d, 0x0c, 0x5f, 0x6e, 0xf9, 0xc8, 0x9b, 0xaa, 0x84, 0xb5, 0xe6, 0xd7, 0x40, 0x71, 0x22, 0x13,

        0x7e, 0x4f, 0x1c, 0x2d, 0xba, 0x8b, 0xd8, 0xe9, 0xc7, 0xf6, 0xa5, 0x94, 0x03, 0x32, 0x61, 0x50,

        0xbb, 0x8a, 0xd9, 0xe8, 0x7f, 0x4e, 0x1d, 0x2c, 0x02, 0x33, 0x60, 0x51, 0xc6, 0xf7, 0xa4, 0x95,

        0xf8, 0xc9, 0x9a, 0xab, 0x3c, 0x0d, 0x5e, 0x6f, 0x41, 0x70, 0x23, 0x12, 0x85, 0xb4, 0xe7, 0xd6,

        0x7a, 0x4b, 0x18, 0x29, 0xbe, 0x8f, 0xdc, 0xed, 0xc3, 0xf2, 0xa1, 0x90, 0x07, 0x36, 0x65, 0x54,

        0x39, 0x08, 0x5b, 0x6a, 0xfd, 0xcc, 0x9f, 0xae, 0x80, 0xb1, 0xe2, 0xd3, 0x44, 0x75, 0x26, 0x17,

        0xfc, 0xcd, 0x9e, 0xaf, 0x38, 0x09, 0x5a, 0x6b, 0x45, 0x74, 0x27, 0x16, 0x81, 0xb0, 0xe3, 0xd2,

        0xbf, 0x8e, 0xdd, 0xec, 0x7b, 0x4a, 0x19, 0x28, 0x06, 0x37, 0x64, 0x55, 0xc2, 0xf3, 0xa0, 0x91,

        0x47, 0x76, 0x25, 0x14, 0x83, 0xb2, 0xe1, 0xd0, 0xfe, 0xcf, 0x9c, 0xad, 0x3a, 0x0b, 0x58, 0x69,

        0x04, 0x35, 0x66, 0x57, 0xc0, 0xf1, 0xa2, 0x93, 0xbd, 0x8c, 0xdf, 0xee, 0x79, 0x48, 0x1b, 0x2a,

        0xc1, 0xf0, 0xa3, 0x92, 0x05, 0x34, 0x67, 0x56, 0x78, 0x49, 0x1a, 0x2b, 0xbc, 0x8d, 0xde, 0xef,

        0x82, 0xb3, 0xe0, 0xd1, 0x46, 0x77, 0x24, 0x15, 0x3b, 0x0a, 0x59, 0x68, 0xff, 0xce, 0x9d, 0xac

};

unsigned char Crc8CheckSum(unsigned char *ptr, unsigned short length)
{
    unsigned char i;
    unsigned char Crc8 = 0;
    while (length--) {
        for (i = 0x80; i; i >>= 1) {
            if ((Crc8 & 0x80) != 0) {
                Crc8 <<= 1;
                Crc8 ^= 0x31;
            }
            else {
                Crc8 <<= 1;
            }
            if (*ptr & i) {
                Crc8 ^= 0x31;
            }
        }
        ptr++;
    }
    return Crc8;
}

unsigned char crc_high_first(unsigned char *ptr, unsigned char len)
{
    unsigned char i;
    unsigned char crc = 0; /* 计算的初始crc值 */

    while (len--) {
        crc ^= *ptr++;          /* 每次先与需要计算的数据异或,计算完指向下一数据 */
        for (i = 8; i > 0; --i) /* 下面这段计算过程与计算一个字节crc一样 */
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x31;
            else
                crc = (crc << 1);
        }
    }

    return (crc);
}

unsigned char cal_crc_table(unsigned char *ptr, unsigned char len)

{
    unsigned char crc = 0x00;

    while (len--) {
        crc = crc_table[crc ^ *ptr++];
    }
    return (crc);
}

static uint8_t check(const uint8_t *data, int len)
{
    int i;
    uint8_t sum;

    for (i = 0; i < len; i++) {
        sum += data[i];
    }
    return sum;
}

uint8_t check_sum_ls(uint8_t *src, size_t ssize)
{
    uint32_t crc = 0;

    while (ssize--) {
        crc += *src++;
        printf("crc = %x\n", crc);
    }
    crc = crc % 0x100;

    return crc;
}

#if 1
uint8_t crc_8(uint8_t *vptr, uint32_t len)
{
    uint8_t crc = 0;

    for (; len > 0; len--) {
        crc = CRC8_Table[crc ^ *vptr];  //查表得到CRC码
        vptr++;
    }
    return crc;
}
#else
//CRC-8 x8+x2+x+1
uint8_t crc_8(uint8_t *vptr, uint32_t len)
{
    const uint8_t *data = vptr;
    unsigned crc = 0;
    int i, j;
    for (j = len; j; j--, data++) {
        crc ^= (*data << 8);
        for (i = 8; i; i--) {
            if (crc & 0x8000)
                crc ^= (0x1070 << 3);
            crc <<= 1;
        }
    }
    return (uint8_t)(crc >> 8);
}
#endif

bool crc8_check(uint8_t *in_data, uint32_t in_len, uint8_t in_crc)
{
    uint8_t crc = 0;
    crc = crc_8(in_data, in_len);
    if (crc != in_crc) {
        printf("crc_8 cal=0x%02X input=0x%02X\r\n", crc, in_crc);
        return false;
    }
    else {
        return true;
    }
}

enum {
    SLIP_START = 0xAA,
    SLIP_END = 0xBB,
    SLIP_ESCAPE = 0xCA,
    SLIP_ESCAPE_START = 0xCB,
    SLIP_ESCAPE_ES = 0xCC,
    SLIP_ESCAPE_END = 0xCD,
} miio_escape_e;

static void test_send(uint8_t *data, int len)
{
    for (int i = 0; i < len; i++) {
        printf("%02X ", data[i]);
    }
    printf("\r\n");
}

static void array_test(void)
{
    int a[5] = {1, 2, 3, 4, 5};
    int b[5];  //用来存放倒置后的数据
    int i, j;
    for (i=0, j=4; i<5, j>=0; ++i, --j)
    {
        b[i] = a[j];
        printf("%d\n", b[i]);
    }
}

int main(int argc, char const *argv[])
{
    // unsigned char test[] = {0xA5, 0x13, 0x01, 0x01, 0x01, 0x01, 0x10, 0x00, 0xC0, 0x07, 0x01, 0x01, 0x3C, 0x01, 0x3C, 0x01, 0x3C, 0x6A, 0x55};
    // uint8_t data_on[] = {0x55, 0xaa, 0x00, 0x0c, 0x00, 0x13,
    //                      0x0A, 0x62, 0x30, 0x39, 0x34, 0x35, 0x31, 0x62, 0x62, 0x36, 0x64,
    //                      0x02, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x32};
    // unsigned char crc = 0;
    // uint8_t chk;
    // uint8_t data[] = {0x01, 0x81, 0x00, 0x02, 0x42, 0x01, 0x42, 0x02};
    // uint8_t data1[] = {0xAA, 0x15, 0x00, 0xA1, 0xB2, 0x0F, 0x03, 0xF1, 0x01, 0x01, 0x01, 0x00, 0x18};
    // printf("hello world\n");
    // printf("len = %d\n", sizeof(data));
    // chk = check(data, sizeof(data));
    // printf("chk = 0x%02x\n", chk);

    // crc = Crc8CheckSum(&test[10], 7);
    // crc = cal_crc_table(&test[10], 7);
    // crc = crc_high_first(&test[10], 7);
    // crc = check_sum_ls(data_on, sizeof(data_on) - 1);
    // printf("crc = %x\n", crc);
    // crc = Crc8CheckSum(data1, sizeof(data1));

    uint8_t data[] = {0x08, 0x03};
    // uint8_t data[] = {0x0A, 0x00, 0xE5, 0xB7, 0xA6, 0xE9, 0x94, 0xAE, 0xE5, 0x8F, 0xB3, 0xE8,  0xBD, 0xAC, 0x19};
    // uint8_t buff[16] = {0};
    // crc_8(data, 3);
    printf("crc = 0x%02x\n", crc_8(data, 2));

    // char* str = "你";
    // printf("%s, %d\n", str, strlen(str));
    uint8_t start = SLIP_START;
    test_send((uint8_t *)&start, 1);

    // uint8_t key = 1;
    // snprintf(buff, sizeof(buff), "%d", key);
    // printf("buff = %s\n", buff);

    // array_test();
    char buf_tp[16] = {0};

    buf_tp[0] = 0x31;
    buf_tp[1] = 0x31;
    atoi(buf_tp);
    printf("buf_tp = %d\n", atoi(buf_tp));

    char *bugf = "左键右转";
    printf("bugf = %s, len = %d\n", bugf, strlen(bugf));

    return 0;
}
