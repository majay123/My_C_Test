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
 * @LastEditTime : 2021-12-29 10:37:50
 * @FilePath     : /My_C_Test/point_test_2/point.c
 * @Description  : 
 * 
 * ******************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <malloc.h>
#include "common.h"

int main(int argc, char const *argv[])
{
    char *book_table[] = {
        "old man", "new man", "test book", "tunshi book",
        "xiaogou", "dadao book", "hundun"
    };

    char **english_book[4];
    char **best_book[3];
#if 0
    best_book[0] = &book_table[0];
    best_book[1] = &book_table[2];
    best_book[2] = &book_table[5];

    english_book[0] = &book_table[0];
    english_book[1] = &book_table[1];
    english_book[2] = &book_table[5];
    english_book[3] = &book_table[6];

    print_mcd("%p, %p, %p", &book_table[0], &(*best_book[0]), &(*english_book[0]));

    int i = 0;
    uint8_t a = 1;
    for (i = 0; i < 10; i++) {
        a = ~a & 0x01;
        printf("%d\n", a);
    }
#endif
    char tmp[] = "DUI.SmartHome.Device.TurnOff";
    char tmp2[] = "DUI.SmartHome.";

    printf("%d\n", strncmp(tmp, tmp2, strlen("DUI.SmartHome.")));

    int a = atoi("0");
    print_mcd("a = %d", a);

    return 0;
}


