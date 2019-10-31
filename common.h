#ifndef __COMMON_H__
#define __COMMON_H__


#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

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

#endif