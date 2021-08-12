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
 * @Date         : 2021-08-12 09:36:52
 * @LastEditors  : MCD
 * @LastEditTime : 2021-08-12 10:37:46
 * @FilePath     : /My_C_Test/file_test/file_test.c
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
#include "common.h"

#define MAX_BUF_SIZE        (1024)

int main(int argc, char const *argv[])
{
    int fd;
    size_t len = 0;
    char buff[MAX_BUF_SIZE] = "Hello linux, this is file test!";
    char read_buff[MAX_BUF_SIZE] = {0};

    fd = open("hello.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if(fd){
        len = write(fd, buff, strlen(buff));
        close(fd);
        
        if(len <= 0){
            return -1;
        }
    }
    
    fd = open("hello.txt", O_RDONLY);
    len = read(fd, read_buff, MAX_BUF_SIZE);
    close(fd);
    if(len <= 0)
        return -1;
    read_buff[len] = '\0';
    print_mcd("%s", read_buff);

    return 0;
}

