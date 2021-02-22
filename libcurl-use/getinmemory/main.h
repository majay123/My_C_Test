#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


#define PRINT_MCD_ENABLE  1

#if PRINT_MCD_ENABLE == 1
    #define print_mcd(format, arg...)   do { printf("\033[31m[----mcd----]\033[0m:%s,%d--- "format"\n",__FILE__,__LINE__, ## arg);}\
                                        while (0)
#else
    #define print_mcd(format, arg...)   do {} while (0)
#endif


#endif // !__MAIN_H__
