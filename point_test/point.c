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
 * @LastEditTime : 2021-06-29 13:25:12
 * @FilePath     : /My_C_Test/point_test/point.c
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
    device_t *dev_test = (device_t *)calloc(1, sizeof(device_t));
    int i = 0;
    
    if(dev_test == NULL)
        print_mcd("calloc failed!!");
    dev_test->num = 10;

    device_info_t *dev_info_test = (device_info_t *)calloc(dev_test->num,  sizeof(device_info_t));
    print_mcd("calloc size = %ld, %ld", malloc_usable_size(dev_test), malloc_usable_size(dev_info_test));
    if(dev_info_test == NULL)
        print_mcd("calloc failed !!");

    dev_test->device_info = dev_info_test;
    print_mcd("%p, %p", dev_test->device_info, dev_info_test);
    for (i = 0; i < dev_test->num; i++)
        strncpy(dev_info_test[i].name, "test1", DEVICE_NAME_LEN);

    for (size_t i = 0; i < dev_test->num; i++)
        print_mcd("%s, %d", dev_test->device_info[i].name, i);
    
    exit(0);
}


