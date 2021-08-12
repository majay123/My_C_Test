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
 * @Date         : 2021-08-09 14:06:44
 * @LastEditors  : MCD
 * @LastEditTime : 2021-08-10 15:15:20
 * @FilePath     : /My_C_Test/getopt_test/main.c
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
#include <getopt.h>

#include "common.h"


#define my_min(x,y) ({ \
const typeof(x) _x = (x); \
const typeof(y) _y = (y); \
(void) (&_x == &_y); \
_x < _y _x : _y; })

struct ex_struct{
    char a;
    char b;
    char c;
};
struct ex_struct1{
    char a;
    char b;
    char c;
}__attribute__((aligned(4)));

struct ex_struct2{
    char a;
    int b;
    long c;
};

int main(int argc, char *argv[])
{
    int c;
    int option_index = 0;
    int n = argc;
    double x[n];

    print_mcd("%d", sizeof(x) / sizeof(double));
    print_mcd("%d, %d, %d", sizeof(struct ex_struct), sizeof(struct ex_struct1),sizeof(struct ex_struct2));
    return 0;
    
    static struct option long_options[] = {
        {"add",     required_argument, NULL,  259 },
        {"append",  no_argument,       NULL,  260 },
        {"delete",  required_argument, NULL,  261 },
        {"verbose", no_argument,       NULL,  262 },
        {"create",  required_argument, NULL,  263},
        {"file",    required_argument, NULL,  264 },
        {NULL,      0,                 NULL,  0 }
    };

    while((c = getopt_long(argc, argv, "a:b:c:d:", long_options, &option_index)) != -1)
    {
        print_mcd("opt = %c, %d", c, c);
        print_mcd("optarg = %s", optarg);
        print_mcd("optind = %d", optind);
        print_mcd("argv[optind - 1] = %s\n", argv[optind - 1]);
    }
    return 0;
}
