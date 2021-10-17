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
 * @LastEditTime : 2021-10-16 13:29:42
 * @FilePath     : /My_C_Test/simple_test/main.c
 * @Description  : 
 * 
 * ******************************************
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
        printf("(%ld)%02x ",i,  data[i]);
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

int main(int argc, char const *argv[])
{
    uint8_t time[4] = {0x00, 0x01, 0xe0, 0x19};
    uint8_t data[4] = {0};
    uint8_t data1[4] = {0};
    _Bool test = 0;
    // char *end;
    uint32_t running_time = 10 * 1000;
    // running_time = _hex_to_uint(time, sizeof(time));
    // printf("Running time: %d\n", running_time);
    // _uint_to_hex(running_time, data, sizeof(data));
    printf("Running time: %x, %ld\n", running_time, running_time);
    my_itoa(data1, running_time, sizeof(data1));
    printf("%02x %02x %02x %02x\n", data1[0], data1[1], data1[2], data1[3]);
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
