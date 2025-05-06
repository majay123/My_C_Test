/*
 * 
 * ************Copyright 2025 MCD************
 * 
 * @version      : 
 * @Company      : HOPE
 * @Author       : MCD
 * @Date         : 2025-04-22 09:01:02
 * @LastEditors  : MCD
 * @LastEditTime : 2025-04-22 09:45:19
 * @FilePath     : /My_C_Test/srgb_test/main.c
 * @Description  : 
 * 
 * ******************************************
 */
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define DSS_RECOMMENDED_PRELINE_TIME 60

void main(void)
{
    int preline_num;
    int tmp;

    int xres = 480;
    int yres = 480;
#if 1
    int hsw = 2;
    int hbp = 40;
    int hfp = 40;

    int vsw = 2;
    int vbp = 12;
    int vfp = 112;
    int pixclock = 50000;
#else   
    int hsw = 1;
    int hbp = 45;
    int hfp = 2;

    int vsw = 1;
    int vbp = 10;
    int vfp = 1;
    int pixclock = 41667;
#endif
    tmp = (xres + hfp + hbp + hsw) * pixclock;

    // printf("tmp = %d\n", tmp);

    /* caculate preline number */
    preline_num = tmp;
    printf("preline_num1 = %d\n", preline_num);
    if (preline_num != 0)
        preline_num = (DSS_RECOMMENDED_PRELINE_TIME * 1000000 + preline_num / 2) / preline_num; /* round */
    printf("preline_num2 = %d\n", preline_num);
    preline_num -= vfp;
    printf("preline_num3 = %d\n", preline_num);
    preline_num = (preline_num <= 0 ? 1 : preline_num);

    printf("preline_num4 = %d\n", preline_num);

    /* caculate preline time */
    printf("preline_num: %d, vfp: %d, tmp: %d\n", preline_num, vfp, tmp);
    printf(" (preline_num + vfp) * tmp =  %d\n", (preline_num + vfp) * tmp);
    int preline_time = (preline_num + vfp) * (tmp / 1000000);
    if (preline_time > DSS_RECOMMENDED_PRELINE_TIME * 2 ||
        preline_time < DSS_RECOMMENDED_PRELINE_TIME / 2)
        printf("preline time(%dus) is abnormal\n",
               preline_time);
printf("preline time(%dus) \n",
                preline_time);
    /* caculate VB time */
    int vb_time = vsw * tmp / 1000000;

    printf("vb_timer: %dus\n", vb_time);
}