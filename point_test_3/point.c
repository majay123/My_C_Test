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
 * @Date         : 2021-06-29 10:38:35
 * @LastEditors  : MCD
 * @LastEditTime : 2023-10-05 10:55:12
 * @FilePath     : /My_C_Test/point_test_3/point.c
 * @Description  : 
 * 
 * ******************************************
 */

#include "common.h"
#include <fcntl.h>
#include <malloc.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#pragma pack(4)
typedef struct {
    char number[5];
    char type;
} test_T /* __attribute__((aligned(4))) */;
#pragma pack()

void flash_read_word(uint32_t *data, uint32_t len)
{
    int fd = open("test.txt", O_RDONLY);
    int ret = read(fd, (uint32_t *)data, len);
    close(fd);
    if (ret != len) {
        printf("read error\n");
        exit(1);
    }
}

int main(int argc, char const *argv[])
{
    int i = 0;
    // uint8_t *buf = (uint8_t *)malloc(32);
    // memset(buf, 0, 32);
    // for(i = 0; i < 32; i++)
    //     printf("%p\n", buf + i);
    // // flash_read_word((uint32_t *)buf, 32 /4);

    // // printf("%s\n", buf);
    // printf("--------------------------------\n");
    // uint32_t *buf1 = (uint32_t *)malloc(32 / 4);
    // memset(buf1, 0, 32);
    // for(i = 0; i < 32; i++)
    //     printf("%p\n", buf1 + i);

    // printf("size = %d\n", sizeof(test_T));
    // uint16_t a[5] = {0x1234, 0x5678, 0x9abc, 0xdef0, 0x55aa};
    // uint16_t *ptr1 = (uint32_t *)(&a + 1);
    // uint32_t *ptr2 = (uint32_t *)(a + 1);

    // printf("0x%x, 0x%x, 0x%x\n", *(a+1), *(ptr1 + 1), *ptr2);

    for (i = 0; i < 4; i++) {
        if (i == 2) {
            switch (i) {
                case 0:
                    printf("print case 0\n");
                    break;
                case 2:
                    printf("print case 2\n");
                    break;
                default:
                    break;
            }
            break;
        }
        printf("print for %d\n", i);
    }
    printf("out for %d\n", i);

    return 0;
}
