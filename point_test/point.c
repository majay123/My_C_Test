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
 * @Date         : 2021-06-29 10:38:35
 * @LastEditors  : MCD
 * @LastEditTime : 2021-06-29 17:37:41
 * @FilePath     : /My_C_Test/point_test/point.c
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
#include <malloc.h>
#include "common.h"

device_t g_dev_test;

static device_t * _test_point(void)
{
    device_t *dev_test = (device_t *)calloc(1, sizeof(device_t));
    int i = 0;

    if(dev_test == NULL)
        print_mcd("calloc failed!!");
    dev_test->num = 8;
    dev_test->deviceTotalNum = 8;
    print_mcd("addr %p", dev_test);
    print_mcd("calloc size = %ld, %ld, %d", malloc_usable_size(dev_test), sizeof(device_t *), sizeof(device_info_t));
    // return 0;

    // device_info_t *dev_info_test = (device_info_t *)calloc(dev_test->num,  sizeof(device_info_t ));
    device_info_t *dev_info_test = (device_info_t *)calloc(dev_test->num,  sizeof(device_info_t *));
    // print_mcd("calloc size = %ld, %ld", malloc_usable_size(dev_test), malloc_usable_size(dev_test->device_info));
    if(dev_info_test == NULL)
        print_mcd("calloc failed !!");

    dev_test->device_info = dev_info_test;
    print_mcd("calloc size = %ld, %ld", malloc_usable_size(dev_test), malloc_usable_size(dev_test->device_info));
    print_mcd("%p, %p", dev_test->device_info, dev_info_test);
    for (i = 0; i < dev_test->num; i++)
    {
        print_mcd("%p", &dev_test->device_info[i]);
        strncpy(dev_info_test[i].name, "testrfeertertwerrtwertfgsdgsdfgsdfgssdfasfafafasfsafasfadfgdswertwertwertwetr1", DEVICE_NAME_LEN - 1);
        strncpy(dev_info_test[i].deviceId, "123111111111", DEVICE_ID_LEN - 1);
        dev_info_test[i].online_state = i;
    }

    return dev_test;
}


int main(int argc, char const *argv[])
{
    msg_apps_t appMsg;
    mqtt_msg_apps_t msg;
    memset(&g_dev_test, 0, sizeof(g_dev_test));
    
    
    
    device_t * dev_test = _test_point();
    

    msg.content = dev_test;
    msg.code = 1;
    msg.size = sizeof(device_t *);
    print_mcd("addr %p", msg.content);
    appMsg.type = 2;
    memcpy(appMsg.content, &msg, sizeof(mqtt_msg_apps_t));
    
// 
    mqtt_msg_apps_t *test_mqtt_msg = &appMsg.content;
    print_mcd("addr %p", test_mqtt_msg->content);
    device_t *test_dev = test_mqtt_msg->content;

    for (size_t i = 0; i < test_dev->num; i++)
    {
        print_mcd("%p", &test_dev->device_info[i]);
        print_mcd("%s, %s, %ld, %d", test_dev->device_info[i].name, test_dev->device_info[i].deviceId,i, test_dev->device_info[i].online_state);   
    }

// 
    g_dev_test.deviceTotalNum = test_dev->deviceTotalNum;
    g_dev_test.num = test_dev->num;
    g_dev_test.device_info = (device_info_t *)calloc(g_dev_test.num, sizeof(device_info_t *));

    memcpy(g_dev_test.device_info, test_dev->device_info, sizeof(device_info_t) * g_dev_test.num);
    
    free(dev_test->device_info);
    dev_test->device_info = NULL;
    print_mcd("%ld", malloc_usable_size(dev_test->device_info));
    free(dev_test);
    dev_test = NULL;
    print_mcd("%ld", malloc_usable_size(dev_test));
    
    print_mcd("%ld", malloc_usable_size(g_dev_test.device_info));


    print_mcd("num = %d, total num = %d", g_dev_test.num, g_dev_test.deviceTotalNum);
    for (size_t i = 0; i < g_dev_test.num; i++)
    {
        print_mcd("%p", &g_dev_test.device_info[i]);
        print_mcd("%s, %s, %ld, %d", g_dev_test.device_info[i].name, g_dev_test.device_info[i].deviceId,i, g_dev_test.device_info[i].online_state);   
        
    }
    
    exit(0);
}


