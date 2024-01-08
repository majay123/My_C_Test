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
 * ************Copyright 2023 MCD************
 * 
 * @version      : 
 * @Company      : HOPE
 * @Author       : MCD
 * @Date         : 2023-03-14 16:33:07
 * @LastEditors  : MCD
 * @LastEditTime : 2023-08-22 13:04:16
 * @FilePath     : /My_C_Test/Hope_Work/leshi/main.c
 * @Description  : 
 * 
 * ******************************************
 */
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <libubox/uloop.h>
#include <libubox/usock.h>
#include <libubox/ustream.h>
#include <linux/types.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "app_context.h"
#include "cJSON.h"
#include "common.h"
#include "debug.h"
#include "ls_bt_cmd_dispatch.h"
#include "ls_bt_mesh.h"
#include "ls_bt_mesh_dev.h"
#include "queue.h"

/*********************************************************************
* MACROS
*/
#define COMPILE_TIME (__DATE__ "." __TIME__)

#ifndef APP_VERSION
#define APP_VERSION ("V1.00.00")
#endif

#ifndef APP_SER_NAME
#define APP_SER_NAME ("leshi_service")
#endif

#define CURTAIN_OPEN             "3501ffffffffff09"
#define CURTAIN_CLOSE            "3501ffffffffff08"

/*********************************************************************
* GLOBAL VARIABLES
*/
appContext_t appContext;
llq_t llq_uart_send_stream;
llq_t llq_uart_dispatch_stream;

/*********************************************************************
* LOCAL VARIABLE
*/
static uint8_t thread_exit = 0;
static int uart_fd = -1;

/* Vals for long options */
enum {
    GETOPT_VAL_HELP = 257,
    GETOPT_VAL_BAUDRATE,
    GETOPT_VAL_TTYPORT,
    GETOPT_VAL_PARITY,
    GETOPT_VAL_BITWIDTH,
    GETOPT_VAL_STOPBIT,
    GETOPT_VAL_EXTEND,
};

/*********************************************************************
* LOCAL FUNCTIONS
*/

static void usage()
{
    printf("Usage: uart service [OPTION]\n"
           "\n"
           "  -h, --help\n"
           "  -b, --baud         Baud rate, 9600, etc (9600 is default)\n"
           "  -p, --port         Port (/dev/ttyS0, etc) (must be specified)\n"
           "  -a, --slaveaddr    Slaveaddr (/dev/ttyS0, etc) (must be specified)\n"
           "\n");
}

void *_ls_bt_mesh_read_thread(void *argv)
{
    while (!thread_exit) {
        ls_bt_mesh_dev_rev(uart_fd, &llq_uart_dispatch_stream);
        pthread_testcancel();
    }

    return NULL;
}

static void *_ls_bt_mesh_dispatch_thread(void *argv)
{
    while (!thread_exit) {
        ls_bt_mesh_dispatch(&llq_uart_dispatch_stream);
        pthread_testcancel();
    }

    return NULL;
}

static void *_ls_bt_mesh_send_thread(void *argv)
{
    while (!thread_exit) {
        ls_process_cmd_back_send(uart_fd, &llq_uart_send_stream);
        pthread_testcancel();
    }

    return NULL;
}

static void _ls_bt_mesh_deinit(void)
{
    ls_mesh_dev_data_t data;

    /* send empty message,The goal is to let the thread exit blocking */
    memset(&data, 0, sizeof(ls_mesh_dev_data_t));
    llq_add(&llq_uart_send_stream, (char *)&data, sizeof(ls_mesh_dev_data_t), 0);
    llq_add(&llq_uart_dispatch_stream, (char *)&data, sizeof(ls_mesh_dev_data_t), 0);
    // mqtt_deinit();
    ls_bt_mesh_dev_close(uart_fd);
    llq_close(&llq_uart_send_stream);
    llq_close(&llq_uart_dispatch_stream);
}

int mqtt_client_get_ota_file(const char *ota_file)
{
    // 通过文件路径读取文件的大小，并打印出来
    int ret = 0, i;
    int fd = open(ota_file, O_RDONLY);
    if (fd < 0) {
        printf("open %s failed\n", ota_file);
        return -1;
    }
    struct stat st;
    if (fstat(fd, &st) < 0) {
        printf("fstat %s failed\n", ota_file);
        close(fd);
        return -1;
    }
    printf("ota file size: %ld\n", st.st_size);
    char buffer[10];
    if ((ret = read(fd, buffer, 10)) > 0) {
        // 发送数据包(buffer)的内容（长度为result）
        // printf("Packet %ld sent.\n", ret);
        for (i = 0; i < ret; i++) {
            printf("%02x ", buffer[i]);
        }
    }
    printf("\n");
    close(fd);
    return 0;
}

int test2(const char *ota_file)
{
    FILE *fp;
    int ret = 0;
    int i, j;

    fp = fopen(ota_file, "rb");
    if (fp == NULL) {
        printf("open ota file failed\n");
        return ret;
    }

    // 获取文件大小并计算分包数量
    fseek(fp, 0, SEEK_END);
    int file_size = ftell(fp);
    rewind(fp);
    printf("\nFile size: %d\n", file_size);

    for(i = 0; i < 10; i++) 
    {
        uint8_t buffer[10];
        if ((ret = fread(buffer, sizeof(char), 10, fp)) > 0) {
            // 发送数据包(buffer)的内容（长度为result）
            // printf("Packet %ld sent.\n", ret);
            for (j = 0; j < ret; j++) {
                printf("%02x ", buffer[j]);
            }
        }
        printf("\n");
    }
    fclose(fp);

    return 0;
}

#if 0
int main(int argc, char const *argv[])
{
    pthread_t r_thread;
    pthread_t d_thread;
    pthread_t s_thread;
    int c;
    int s_ret = -1;
    int d_ret = -1;
    int r_ret = -1;

    /* required_argument,no_argument,optional_argument */
    struct option long_options[] =
        {
            {"help", no_argument, NULL, GETOPT_VAL_HELP},
            {"baud", required_argument, NULL, GETOPT_VAL_BAUDRATE},
            {"port", required_argument, NULL, GETOPT_VAL_TTYPORT},
            {"parity", required_argument, NULL, GETOPT_VAL_PARITY},
            {"bitwidth", required_argument, NULL, GETOPT_VAL_BITWIDTH},
            {"stopbit", required_argument, NULL, GETOPT_VAL_STOPBIT},
            {"extend", required_argument, NULL, GETOPT_VAL_EXTEND},
            {NULL, 0, NULL, 0}};

    memset(&appContext, 0, sizeof(appContext_t));
    pthread_mutex_init(&appContext.logFileMutex, NULL);

    opterr = 0;
    while ((c = getopt_long(argc, (char *const *)argv, "b:p:hv",
                            long_options, NULL)) != -1) {
        switch (c) {
            case 'v':
                appContext.verbose = 1;
                break;
            case GETOPT_VAL_BAUDRATE:
            case 'b':
                appContext._cl_baud = atoi(optarg);
                break;
            case GETOPT_VAL_TTYPORT:
            case 'p':
                appContext._cl_port = strdups(optarg);
                break;
            case GETOPT_VAL_PARITY:
                /*
				* option of parity type
				* "NONE"(default)   无校验
				* "ODD"             奇校验
				* "EVEN"            偶校验
				* "MARK"
				* "SPACE"           空格
            	*/
                appContext._cl_parity = strdups(optarg);
                break;
            case GETOPT_VAL_BITWIDTH:
                /*
           		* option of bit width
           		* 5,
           		* 6,
           		* 7,
           		* 8(default)
           		*/
                appContext._cl_bitwidth = atoi(optarg);
                break;
            case GETOPT_VAL_STOPBIT:
                /*
           		* option of stop bit
           		* 10, 1 bit stop(default)
           		* 15, 1.5 bit stop
           		* 20, 2 bit stop
           		*/
                appContext._cl_stopbit = atoi(optarg);
                break;
            case GETOPT_VAL_EXTEND:
                /*
           		* extend params,JSON string format,cJSON_Parse
           		*/
                break;
            case GETOPT_VAL_HELP:
            case 'h':
                usage();
                exit(EXIT_SUCCESS);
            case '?':
                // The option character is not recognized.
                fprintf(stderr, "Unrecognized option: %s\n", optarg);
                opterr = 1;
                break;
        }
    }

    if (opterr) {
        usage();
        exit(EXIT_FAILURE);
    }

    /* support maximum 115200 baudrate */
    if (appContext._cl_baud == 0 || appContext._cl_baud > 115200 || !appContext._cl_port) {
        fprintf(stderr, "app tty port or baudrate error\n");
        exit(EXIT_FAILURE);
    }

    /* find the proper serial port */
    char *uart_port = strrchr(appContext._cl_port, '/');
    uart_port++;
    if (!uart_port || !strstr(appContext._cl_port, "tty")) {
        fprintf(stderr, "app tty port format error\n");
        exit(EXIT_FAILURE);
    }

    snprintf(appContext.app_log_file, sizeof(appContext.app_log_file), "/tmp/%s.log", uart_port);
    DEBUG_INFO("** compile time : %s, app name: %s, version:%s\n", COMPILE_TIME, APP_SER_NAME, APP_VERSION);
    uloop_init();

    llq_open(&llq_uart_send_stream);
    llq_open(&llq_uart_dispatch_stream);

    r_ret = pthread_create(&r_thread, NULL, _ls_bt_mesh_read_thread, NULL);
    if (r_ret != 0) {
        DEBUG_ERROR("create r thread failed\n");
        goto ERROUT;
    }
    d_ret = pthread_create(&d_thread, NULL, _ls_bt_mesh_dispatch_thread, NULL);
    if (r_ret != 0) {
        DEBUG_ERROR("create d thread failed\n");
        goto ERROUT;
    }
    s_ret = pthread_create(&s_thread, NULL, _ls_bt_mesh_send_thread, NULL);
    if (r_ret != 0) {
        DEBUG_ERROR("create s thread failed\n");
        goto ERROUT;
    }

    /* wait main thread exit */
    uloop_run();

ERROUT:
    if (appContext._cl_port)
        free(appContext._cl_port);

    if (appContext._cl_parity)
        free(appContext._cl_parity);

    thread_exit = 1;
    _ls_bt_mesh_deinit();

    if (r_ret == 0) {
        pthread_cancel(r_thread);
        pthread_join(r_thread, NULL);
    }
    if (d_ret == 0) {
        pthread_cancel(d_thread);
        pthread_join(d_thread, NULL);
    }
    if (s_ret == 0) {
        pthread_cancel(s_thread);
        pthread_join(s_thread, NULL);
    }

    pthread_mutex_destroy(&appContext.logFileMutex);

    uloop_done();

    DEBUG_INFO("main exit\n");

    return 0;
}
#else
int main(int argc, char const *argv[])
{
    float h, s, v;
    int r = 172;
    int g = 85;
    int b = 43;
    char buf[1024] = {0};
    // leshi_sure_band();
    // leshi_datapoint_parse();
    // leshi_ctrl_switch_dev();
    // // hsv2rgb();
    // rgb2hsv(r, g, b, &h, &s, &v);
    // printf("h = %f, s = %f, v = %f\n", h, s, v);
    // // hsv2rgb();
    // sprintf(buf, "%04x%04x%04x", (int)h, (int)(s * 1000), (int)(v * 1000));
    // printf("buf: %s, len: %d\n", buf, strlen(buf));
    // for (size_t i = 0; i < strlen(buf); i++)
    // {
    //     printf("%02x ", buf[i]);
    // }
    // printf("\n");
    printf("len = %d\n", sizeof(CURTAIN_CLOSE));
    printf("len = %d\n", strlen(CURTAIN_CLOSE));
    leshi_ctrl_switch_dev();
    // leshi_sum_test();
    // leshi_delete_one_dev();
    // mqtt_client_get_ota_file("cJSON.c");
    // char *str = "hello world";
    // char str2[32] = {0};
    // sprintf(str2, "%s", str);
    // printf("%s\n", str2);
    // test2("cJSON.c");
    // leshi_group_ctrl_devs();
}
#endif
