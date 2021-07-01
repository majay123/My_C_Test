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
 * @LastEditTime : 2021-06-30 16:52:40
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
    print_mcd("calloc size = %ld, %ld, %ld", malloc_usable_size(dev_test), sizeof(device_t *), sizeof(device_info_t));
    // return 0;

    // device_info_t *dev_info_test = (device_info_t *)calloc(dev_test->num,  sizeof(device_info_t ));
    device_info_t *dev_info_test = (device_info_t *)calloc(dev_test->num,  sizeof(device_info_t)); //392
    // print_mcd("calloc size = %ld, %ld", malloc_usable_size(dev_test), malloc_usable_size(dev_test->device_info));
    if(dev_info_test == NULL)
        print_mcd("calloc failed !!");

    dev_test->device_info = dev_info_test;
    print_mcd("calloc size = %ld, %ld", malloc_usable_size(dev_test), malloc_usable_size(dev_test->device_info));
    print_mcd("%p, %p", dev_test->device_info, dev_info_test);
    for (i = 0; i < dev_test->num; i++)
    {
        print_mcd("%p", &dev_test->device_info[i]);
        // strncpy(dev_info_test[i].name, "testrfeertertwerrtwertfgsdgsdfgsdfgssdfasfafafasfsafasfadfgdswertwertwertwetr1", DEVICE_NAME_LEN - 1);
        // strncpy(dev_info_test[i].deviceId, "123111111111", DEVICE_ID_LEN - 1);
        snprintf(dev_info_test[i].name, DEVICE_ID_LEN, "%s", "adjfjajasd");
        snprintf(dev_info_test[i].deviceId, DEVICE_ID_LEN, "%s", "123132111113");
        dev_info_test[i].online_state = i;
    }

    return dev_test;
}

// #define ARRAY_SIZE      (10)
struct datalist
{
    size_t number;
    int type;
    char ne;
    // int data[];
};


int main(int argc, char const *argv[])
{
    msg_apps_t appMsg;
    mqtt_msg_apps_t msg;
    memset(&g_dev_test, 0, sizeof(g_dev_test));
    
    // char buf[3] = {0};
    // snprintf(buf, 3, "%s", "hellp");
    // print_mcd("%s",buf);
    // return 0;  
    // struct datalist *plist;
    // plist = (struct datalist *)malloc(sizeof(struct datalist) /*+ sizeof(int) * ARRAY_SIZE*/); 

    // print_mcd("size = %d, %d, %d", malloc_usable_size(plist), sizeof(struct datalist), sizeof(int) * ARRAY_SIZE);
    // print_mcd("%d", sizeof(int **));
    // return 0;     
    int array[20] = {0};
    int *ptr = array;
    int i = 0;
    int *pi, a;
    *pi = 314;
    print_mcd("%d", *pi);
    for ( i = 0; i < 20; i++)
    {
        (*ptr)++;
        // ptr++;
        printf("%d", ptr[i]);
    }
    
    return 0;
    
    device_t * dev_test = _test_point();
    

    msg.content = (char *)dev_test;
    msg.code = 1;
    msg.size = sizeof(device_t *);
    print_mcd("addr %p", msg.content);
    appMsg.type = 2;
    memcpy(appMsg.content, &msg, sizeof(mqtt_msg_apps_t));
    
// 
    mqtt_msg_apps_t *test_mqtt_msg = (mqtt_msg_apps_t *)&appMsg.content;
    
    print_mcd("addr %p", test_mqtt_msg->content);
    device_t *test_dev = (device_t *)test_mqtt_msg->content;

    for (size_t i = 0; i < test_dev->num; i++)
    {
        print_mcd("%p", &test_dev->device_info[i]);
        print_mcd("%s, %s, %ld, %d", test_dev->device_info[i].name, test_dev->device_info[i].deviceId,i, test_dev->device_info[i].online_state);   
    }

// 
    g_dev_test.deviceTotalNum = test_dev->deviceTotalNum;
    g_dev_test.num = test_dev->num;
    print_mcd("test");
    print_mcd("g = %d", g_dev_test.num);
    g_dev_test.device_info = (device_info_t *)calloc(g_dev_test.num, sizeof(device_info_t ));
    if(g_dev_test.device_info == NULL)
        print_mcd("failed");
    memcpy(g_dev_test.device_info, test_dev->device_info, sizeof(device_info_t ) * g_dev_test.num);
    print_mcd("sta");
    
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


