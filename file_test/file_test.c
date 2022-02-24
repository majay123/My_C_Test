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
 * @LastEditTime : 2022-02-24 14:37:27
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
#include <errno.h>
#include <time.h>

#define PRINT_MCD_ENABLE 1


#if PRINT_MCD_ENABLE == 1
    #define print_mcd(format, arg...)                                                          \
    do                                                                                     			\
    {                                                                                      			\
        char ctime[30] = { 0 };																		\
        char ctime1[30] = { 0 };																	\
        struct tm tm1 = { 0 };																		\
        struct timespec ts; 																		\
        clock_gettime(CLOCK_REALTIME, &ts); 														\
        localtime_r(&ts.tv_sec,&tm1);																\
        strftime(ctime,sizeof(ctime),"%Y-%m-%d %H:%M:%S",&tm1); 									\
        snprintf(ctime1,sizeof(ctime),"%s.%.3ld",ctime,ts.tv_nsec/1000/1000);	                	\
        printf("\033[31m[--mcd--][%s]\033[0m:%s,%s,%d--- " format "\n", ctime1,__FILE__,__func__,__LINE__, ##arg); 	\
    } while (0)
#else
    #define print_mcd(format, arg...)   do {} while (0)
#endif  

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

