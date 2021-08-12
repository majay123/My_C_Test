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
 * @Date         : 2021-06-21 14:42:34
 * @LastEditors  : MCD
 * @LastEditTime : 2021-08-02 13:45:13
 * @FilePath     : /My_C_Test/memory_test/main.c
 * @Description  : 
 * 
 * ******************************************
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <time.h>


#define PRINT_MCD_ENABLE 1

#if PRINT_MCD_ENABLE == 1
    #define print_mcd(format, arg...)                                                          \
    do                                                                                     			\
    {                                                                                      			\
        char ctime[30] = { 0 };																		\
        char ctime1[30] = { 0 };																		\
        struct tm tm1 = { 0 };																			\
        struct timespec ts; 																		\
        clock_gettime(CLOCK_REALTIME, &ts); 														\
        localtime_r(&ts.tv_sec,&tm1);																\
        strftime(ctime,sizeof(ctime),"%Y-%m-%d %H:%M:%S",&tm1); 									\
        snprintf(ctime1,sizeof(ctime),"%s.%.3ld",ctime,ts.tv_nsec/1000/1000);	                	\
        printf("\033[1;31m[--mcd--][%s]\033[0m:%s,%s,%d--- "format"\n",ctime1, __FILE__,__func__,__LINE__, ##arg);\
    } while (0)
#else
    #define print_mcd(format, arg...)   do {} while (0)
#endif

int bss_end;

static int readFile(char *filepath)
{
    DIR *dir;
    char base[1024] = {0};
    struct dirent *ptr;
    

    dir = opendir(filepath);

    if(dir == NULL)
    {
        print_mcd("open dir failed!");
        exit(1);
    }
    while ((ptr = readdir(dir)) != NULL)
    {
        if(strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
            continue;
        else if(ptr->d_type == DT_DIR)
        {
            memset(base, 0, sizeof(base));
            strcpy(base, filepath);
            strcat(base, "/");
            strcat(base, ptr->d_name);
            readFile(base);
        }
        else
        {
            print_mcd("d name = %s/%s, type = %d", filepath, ptr->d_name, ptr->d_type);
        }
    }
    
    closedir(dir);
    return 1;
}

int main(int argc, const char *argv[])
{
    void *tret;
    char *pmem;
    char file_path[1024] = "/home/mcd/MyWorkSpace/MyGithub_Project/Chatroom-Demo";

    readFile(file_path);
    

#if 0
    print_mcd("bss end: %p", (char *)(&bss_end) + 4);
    tret = sbrk(0);
    if (tret != (void *)-1)
        print_mcd("heap1 start: %p", tret);

    if (brk((char *)tret) == -1)
        print_mcd("brk error");

    tret = sbrk(0);
    if (tret != (void *)-1)
        print_mcd("heap2 start: %p", tret);

    pmem = (char *)malloc(32);
    if (pmem == NULL)
    {
        print_mcd("malloc error!");
        exit(EXIT_FAILURE);
    }
    print_mcd("pmem: %p", pmem);

    tret = sbrk(0);
    if (tret != (void *)-1)
        print_mcd("heap1 end: %p", tret);
        
    if (brk((char *)tret - 10) == -1)
        print_mcd("brk error");

    tret = sbrk(0);
    if (tret != (void *)-1)
        print_mcd("heap2 end: %p", tret);
#endif

    return 0;
}
