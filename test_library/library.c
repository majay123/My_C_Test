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
 * @Date         : 2022-02-24 17:13:11
 * @LastEditors  : MCD
 * @LastEditTime : 2022-10-14 10:16:00
 * @FilePath     : /My_C_Test/test_library/library.c
 * @Description  : 
 * 
 * ******************************************
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint8_t check(const uint8_t *data, int len)
{
    int i;
    uint8_t sum;

    for (i = 0; i < len; i++) {
        sum += data[i];
    }
    return sum;
}

int hello_world()
{
    uint8_t chk;
    uint8_t data[] = {0x01, 0x08, 0x00, 0x02, 0x42, 0x01, 0x42, 0x42, 0x02};
    printf("hello world");
	printf("len = %d\n", sizeof(data));
}
