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
 * @LastEditTime : 2021-06-21 15:13:01
 * @FilePath     : /My_C_Test/memory_test/main.c
 * @Description  : 
 * 
 * ******************************************
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define PRINT_MCD_ENABLE 1

#if PRINT_MCD_ENABLE == 1
#define print_mcd(format, arg...)                                                          \
    do                                                                                     \
    {                                                                                      \
        printf("\033[31m[-mcd-]\033[0m:%s,%d--- " format "\n", __FILE__, __LINE__, ##arg); \
    } while (0)
#else
#define print_zizy(format, arg...) \
    do                             \
    {                              \
    } while (0)
#endif

int bss_end;

int main(int argc, const char *argv[])
{
    void *tret;
    char *pmem;

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

    return 0;
}
