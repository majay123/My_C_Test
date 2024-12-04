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
 * ************Copyright 2024 MCD************
 * 
 * @version      : 
 * @Company      : HOPE
 * @Author       : MCD
 * @Date         : 2024-10-09 11:20:37
 * @LastEditors  : MCD
 * @LastEditTime : 2024-10-25 11:07:52
 * @FilePath     : /My_C_Test/crond/main.c
 * @Description  : 
 * 
 * ******************************************
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
// #include <crond.h>

typedef struct {
    char *timestamp;
    int command;
    int wparam;
    int lparam;
} crondComb_t;

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) ((unsigned)(sizeof(x) / sizeof((x)[0])))
#endif

#define CROND_TIME_STAMP_REBOOT ("0 30 3 * * *")

typedef enum {
    CROND_CMD_BASE = 0x01,
    CROND_CMD_REBOOT,
    CROND_CMD_GET_QQMUSIC_TOPLIST,
    CROND_CMD_MYTEST,
} crondCmd_t;

crondComb_t command[] = {
    {CROND_TIME_STAMP_REBOOT, CROND_CMD_REBOOT, 0, 0},
    {NULL, CROND_CMD_GET_QQMUSIC_TOPLIST, 0, 0},
};

#define ROUND(x)   ((x) >= 0 ? (unsigned char)((x) + 0.5) : (unsigned char)((x) - 0.5))
#define DBVAL(val) (double)((100.0 / 40) * (val))

struct sub_bt_gpio_out_t {
    uint16_t pa_power_en : 1; /* 0---power off, 1---power on */
    uint16_t pa_mute_1 : 1;   /* 分区1 0---unmute, 1---mute */
    uint16_t pa_mute_2 : 1;   /* 分区2 0---unmute, 1---mute */
};

struct sub_bt_gpio_in_t {
    uint16_t sub_bt_status_1 : 1; /* 次蓝牙状态1 0 --- stop/pause, 1 --- play */
    uint16_t sub_bt_status_2 : 1; /* 次蓝牙状态2 0 --- stop/pause, 1 --- play */
};

struct sub_bt_gpio_control_t {

    uint32_t prev_in_data;
    /* system output control */
    union {
        uint32_t out_data;
        struct sub_bt_gpio_out_t gpio_out;
    };

    /* system status flags */
    union {
        uint32_t in_data;
        struct sub_bt_gpio_in_t gpio_in;
    };
};

int main(int argc, char const *argv[])
{
#if 0
    srand(time(NULL));

    // 生成指定范围内的随机数，时
    int hmin = 0, hmax = 3;
    int random_in_range = (rand() % (hmax - hmin + 1)) + hmin;
    printf("Random Number in Range [%d, %d]: %d\n", hmin, hmax, random_in_range);

    // 生成指定范围内的随机数，分
    int mmin = 0, mmax = 59;
    int random_in_range2 = (rand() % (mmax - mmin + 1)) + mmin;
    printf("Random Number in Range [%d, %d]: %d\n", mmin, mmax, random_in_range2);

    printf("command num: %d\n", ARRAY_SIZE(command));


    printf("command num: %d\n", ARRAY_SIZE(command));

    char cron_expression[50] = {0};
    snprintf(cron_expression, sizeof(cron_expression), "%d %d * * *",random_in_range2, random_in_range);

    command[1].timestamp = cron_expression;
    printf("command timestamp: %s\n", command[1].timestamp);

    int a = ROUND(DBVAL(31));
    double b = (double)DBVAL(31);

    printf("a: %d, b: %f\n", a, b);
#endif
    int i = 0;
    uint32_t gpio_in_status = 0;
    struct sub_bt_gpio_control_t gpio_ctl;

    memset(&gpio_ctl, 0, sizeof(gpio_ctl));

    // for(i = 0; i < 2; i++) {
    //     gpio_in_status |= 0 << i; 
    // }

    gpio_in_status |= 0 << 0; 
    gpio_in_status |= 1 << 1; 
    // gpio_ctl.prev_in_data = gpio_in_status; 
    gpio_ctl.in_data = gpio_in_status;
    printf("%d\n", gpio_ctl.in_data);

    printf("%d, %d\n", gpio_ctl.gpio_in.sub_bt_status_2, gpio_ctl.gpio_in.sub_bt_status_1);

    return 0;
}
