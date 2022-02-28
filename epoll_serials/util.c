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
 * ************Copyright 2022 MCD************
 * 
 * @version      : 
 * @Company      : HOPE
 * @Author       : MCD
 * @Date         : 2022-02-24 10:26:58
 * @LastEditors  : MCD
 * @LastEditTime : 2022-02-28 16:03:40
 * @FilePath     : /My_C_Test/epoll_serials/util.c
 * @Description  : 
 * 
 * ******************************************
 */

#include "util.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
* @author  		MCD
* @date  		2022-02-28-15:43
* @details		read config
*/
int read_conf(char *filename, es_conf_t *conf)
{
    // 以只读方式打开
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        ES_DEBUG_ERROR("fopen %s", filename);
        return ES_CONF_ERROR;
    }

    char buff[MAX_BUF_LEN] = {0};
    int buff_len = MAX_BUF_LEN;
    char *curr_pos = buff;
    char *delim_pos = NULL;
    int i = 0;
    int pos = 0;
    int line_len = 0;

    while (fgets(curr_pos, MAX_BUF_LEN, fp)) {
        // 定位每一行第一个界定符位置
        delim_pos = strstr(curr_pos, DELIM);
        if (!delim_pos) {
            ES_DEBUG_ERROR("get = error");
            return ES_CONF_ERROR;
        }
        if (curr_pos[strlen(curr_pos) - 1] == '\n') {
            curr_pos[strlen(curr_pos) - 1] = '\0';
        }

        // 得到root路径
        if (strncmp("root", curr_pos, 4) == 0) {
            delim_pos = delim_pos + 1;
            while (*delim_pos != '#') {
                conf->root[i++] = *delim_pos;
                ++delim_pos;
            }
        }

        // 得到baud rate 波特率
        if (strncmp("baud", curr_pos, 4) == 0) {
            conf->baud_rate = atoi(delim_pos + 1);
        }

        // 得到thread数量
        if (strncmp("thread_num", curr_pos, 9) == 0)
            conf->thread_num = atoi(delim_pos + 1);

        // line_len得到当前行行长
        line_len = strlen(curr_pos);

        // 当前位置跳转至下一行首部
        curr_pos += line_len;
    }
    fclose(fp);
    return ES_CONF_OK;
}

/**
* @author  		MCD
* @date  		2022-02-28-15:43
* @details		
*/
void handle_for_sigpipe()
{
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;

    if (sigaction(SIGPIPE, &sa, NULL))
        return;
}