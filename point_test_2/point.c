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
 * @LastEditTime : 2021-07-09 15:42:48
 * @FilePath     : /My_C_Test/point_test_1/point.c
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

    best_book[0] = &book_table[0];
    best_book[1] = &book_table[2];
    best_book[2] = &book_table[5];

    english_book[0] = &book_table[0];
    english_book[1] = &book_table[1];
    english_book[2] = &book_table[5];
    english_book[3] = &book_table[6];

    print_mcd("%p, %p, %p", &book_table[0], &(*best_book[0]), &(*english_book[0]));


    return 0;
}


