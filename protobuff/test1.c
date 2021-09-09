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
 * @Date         : 2021-09-09 15:34:06
 * @LastEditors  : MCD
 * @LastEditTime : 2021-09-09 15:34:07
 * @FilePath     : /My_C_Test/protobuff/test1.c
 * @Description  : 
 * 
 * ******************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "dmessage.pb-c.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr)[0])
#endif

int test(int argc, const char * argv[])
{
#if 0
    size_t i = 0;
    uint32_t len = 0;
    CommonMessage msg =COMMON_MESSAGE__INIT;

   	msg.type=MESSAGE_TYPE__MSGCODE_CHECK_BIND_STATUS_REQ;
   	   	   	
   	msg.n_device=4;
   	msg.device=calloc(sizeof(CommonMessage__DeviceMsg *)*4,sizeof(char));
   	CommonMessage__DeviceMsg *dmsg=calloc(sizeof(CommonMessage__DeviceMsg)*4,sizeof(char));

   	for(i=0;i<4;i++)
   	{
   		char temp[128]={0};
   		common_message__device_msg__init(&dmsg[i]);
   		dmsg[i].deviceid=i;
   		
   		snprintf(temp,sizeof(temp),"hello world,msg%zu",i);
   		dmsg[i].devicename=strdup(temp);
   		dmsg[i].on=(!!i);
   		msg.device[i]=&dmsg[i];
   	}

    //pack message
    len = common_message__get_packed_size(&msg);
    printf("packed size:%u Byte\n\n", len);
    uint8_t *buffer = (uint8_t *)calloc(len, sizeof(char));
    common_message__pack(&msg, buffer);
    for(i=0;i<4;i++)
    {
    	free(msg.device[i]->devicename);
    }
    free(msg.device);
    free(dmsg);
    
    // send message with len

    //--------------------------------------------------------------------------//
    
    // received message

    //unpack message
    CommonMessage *msg1 = common_message__unpack(NULL, len, buffer);
    
    printf("message type:%d\n",msg1->type);
    for(i=0;i<msg1->n_scene;i++)
    {
    	printf("sceneid:%s scenename:%s\n",msg1->scene[i]->sceneid,msg1->scene[i]->scenename);
    }
    
    for(i=0;i<msg1->n_device;i++)
    {
    	printf("deviceid:%u on:%d devicename:%s\n",msg1->device[i]->deviceid,msg1->device[i]->on,
    		msg1->device[i]->devicename);
    }

    free(buffer);
    common_message__free_unpacked(msg1, NULL);

    return 0;
#endif
}
