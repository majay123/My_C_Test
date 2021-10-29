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
 * @Date         : 2021-10-13 09:17:27
 * @LastEditors  : MCD
 * @LastEditTime : 2021-10-23 15:48:52
 * @FilePath     : /My_C_Test/simple_test/main.c
 * @Description  : 
 * 
 * ******************************************
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// #include "common.h"

static uint32_t _hex_to_uint(uint8_t *hex, size_t len)
{
    // uint8_t time[4] = {0x00, 0xff,0xe0, 0x19};
    // char *end;
    uint32_t running_time = 0;
    for (size_t i = 0; i < len; i++) {
        running_time = running_time << 8;
        running_time = running_time | hex[i];
        //
    }
    // printf("Running time: %x, %ld\n", running_time, running_time);
    return running_time;
}

static void _uint_to_hex(uint32_t num, uint8_t *data, size_t len)
{
    int i = len - 1;
    for (; i >= 0; i--) {
        data[i] = (uint8_t)((num >> (i * 8)) & 0xFF);
        printf("(%ld)%02x ", i, data[i]);
    }
    printf("\n");
}

void hextodecstring(uint8_t *src_data, size_t len, uint8_t *dest_data)
{
    size_t offset = 0;
    int i = 0;

    for (i = 0; i < len; i++) {
        offset += snprintf(dest_data + offset, sizeof(dest_data), "%02d", src_data[i]); // 格式化的数据写入字符串
    }
}

int my_itoa(uint8_t *buf, uint32_t value, size_t size)
{
    uint8_t *p = buf;
    int i = 1;

    if (buf == NULL || size <= 0)
        return -1;
    for (i = 1; i <= size; i++) {
        *p = value >> (8 * (size - i));
        p++;
    }
    return 0;
}

void cancelchar(char *a, char n)
{
    char *p;
    for (p = a; *p != '\0'; p++)
        if (*p != n)
            *a++ = *p;
    *a = '\0';
}

void del_sp(char *src, char n) // 删除C风格字符串中的空格
{
    char *fp = src;
    while (*src) {
        if (*src != n) { // 如果不是空格就复制
            *fp = *src;
            fp++;
        }
        src++;
    }
    *fp = '\0'; //封闭字符串
}

void split(char *str, char *delims)
{
    char *result = NULL;
    result = strtok(str, delims);
    while (result != NULL) {
        printf("result is \"%s\", %d\n", result, atoi(result));
        result = strtok(NULL, delims);
    }
}




int hexStr2bytes(const uint8_t *hexStr, uint8_t *buf, int bufLen)
{
    int i;
    int len;

    if (NULL == hexStr) {
        len = 0;
    } else {
        len = (int)strlen(hexStr) / 2;

        if (bufLen < len) {
            len = bufLen;
        }
    }

    for (i = 0; i < len; i++) {
        char ch1, ch2;
        int val;

        ch1 = hexStr[i * 2];
        ch2 = hexStr[i * 2 + 1];
        if (ch1 >= '0' && ch1 <= '9') {
            val = (ch1 - '0') * 16;
        } else if (ch1 >= 'a' && ch1 <= 'f') {
            val = ((ch1 - 'a') + 10) * 16;
        } else if (ch1 >= 'A' && ch1 <= 'F') {
            val = ((ch1 - 'A') + 10) * 16;
        } else {
            return -1;
        }

        if (ch2 >= '0' && ch2 <= '9') {
            val += ch2 - '0';
        } else if (ch2 >= 'a' && ch2 <= 'f') {
            val += (ch2 - 'a') + 10;
        } else if (ch2 >= 'A' && ch2 <= 'F') {
            val += (ch2 - 'A') + 10;
        } else {
            return -1;
        }

        buf[i] = val & 0xff;
    }

    return 0;
}

int get_addr(char *src_addr, char *dst_addr, size_t len)
{
    del_sp(src_addr, ':');
    hexStr2bytes(src_addr, dst_addr, len);
}



typedef struct {
    uint8_t addr[2];
    uint8_t len;
    uint8_t data;
    uint8_t key;
} test_def;

int main(int argc, char const *argv[])
{
    uint8_t time1[4] = {0x00, 0x01, 0xe0, 0x19};
    uint8_t data[4] = {0};
    uint8_t data1[4] = {0};
    char test1[20] = "00:E0:99:13:C7:62";
    uint8_t buf[20] = {0};
    _Bool test = 0;
    // char *end;
    uint32_t running_time = 10 * 1000;

    // test_def test_def;
    // memset(&test_def, 0, sizeof(test_def));
    // printf("%d\n",sizeof(test_def));
    // printf("%p, %p %p \n",  &test_def.addr[0], &test_def.addr[1],&test_def.len);
    // // printf("%p\n",  &test_def);

    // printf("%p\n",  (char *)&test_def+2);
    size_t i = 0;
    srand(time(0)); 
    for(; i < 20; i++)
	printf("%d \n", rand() % (6)) ;
    // printf("%d, %p\n", test_def.len, test+ 2);

    // del_sp(test1, ':');
    // printf("%s \n", test1);
    // hexStr2bytes(test1, buf, 8);
    // get_addr(test1, buf, 8);
    // for (size_t i = 0; i < 8; i++)
    // {
    //     printf("%02X ", buf[i]);
    // }
    // printf("\n");
    
    // time_t t;
    // srand((unsigned) time(&t));
    // printf("%d %d\n", rand() % 256, rand() % 256);
    
    // running_time = _hex_to_uint(time, sizeof(time));
    // printf("Running time: %d\n", running_time);
    // _uint_to_hex(running_time, data, sizeof(data));
    // printf("Running time: %x, %ld\n", running_time, running_time);
    // my_itoa(data1, running_time, sizeof(data1));
    // printf("%02x %02x %02x %02x\n", data1[0], data1[1], data1[2], data1[3]);
    // uint8_t mac_addr[8] = {0xF0, 0x0A, 0x4F, 0x04, 0x10, 0x2D, 0x01, 0x01};
    // uint8_t dest_addr[25] = {0};
    // hextodecstring(mac_addr, sizeof(mac_addr), dest_addr);
    // printf("%s\n", dest_addr);
    // test = (test == 0 ?  1 : 0);
    // printf("test = %d\n", test);
    // test = (test == 0 ? 1 : 0);
    // printf("test = %d\n", test);
    // test = (test == 0 ? 1 : 0);
    // printf("test = %d\n", test);
    // printf("%d\n",atoi("200"));
    // test ^= test;
    // printf("test = %d\n", test);
    // test ^= test;
    // printf("test = %d\n", test);
    return 0;
}
