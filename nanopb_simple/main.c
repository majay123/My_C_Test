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
 * @Date         : 2023-04-27 12:47:21
 * @LastEditors  : MCD
 * @LastEditTime : 2023-05-10 11:41:06
 * @FilePath     : /My_C_Test/nanopb_simple/main.c
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

#include "dmessage.pb.h"
#include "pb_decode.h"
#include "pb_encode.h"

static int serialNumber = 0;
#define TEST_DEVICE_NEMBER (20)
#define TEST_DPOINT_NEMBER (6)

typedef struct {
    void *data;
    size_t size;
    size_t index;
} com_type_t;


static char *nano_pb_encode(size_t *size)
{
    commonMessage com_msg = commonMessage_init_zero;
    bool status = 0;
    int i = 0;

    printf("msg size = %ld\n", sizeof(com_msg));

    char *buf = NULL;

    serialNumber++;

    buf = calloc(TEST_DEVICE_NEMBER, sizeof(commonMessage));
    pb_ostream_t stream = pb_ostream_from_buffer((pb_byte_t *)buf, sizeof(commonMessage) * TEST_DEVICE_NEMBER);
    
   
    
    // com_msg.combo_size = TEST_DEVICE_NEMBER;
    // com_msg.combo = (struct _combineMsg *)encode_combineMsg_test();
    // combineMsg combo[pb_arraysize(commonMessage, combo[0])] = {combineMsg_init_zero};

    // sizeof(combo);
    // combo[0].index = 0;
    // combo[0].
    // Datapoint dpoint[pb_arraysize(leshiBTmeshGWMsg, dp[0])] = {Datapoint_init_zero};
    com_msg.combo_size = TEST_DEVICE_NEMBER;
    // char *name = calloc(128, sizeof(char));
    // memcpy(name, "ls_device_name1", sizeof("ls_device_name1"));
    // com_msg.deviceName = name;
    // for(i = 0; i < TEST_DEVICE_NEMBER; i++) {
    //     com_msg.combo[i].index = i;
    //     com_msg.combo[i].which_item = combineMsg_lsBTmeshGW_device_tag;
    //     struct _leshiBTmeshGWMsg *lsBTmeshGW_device = (struct _leshiBTmeshGWMsg *)malloc(sizeof(leshiBTmeshGWMsg));
    //     com_msg.combo[i].item.lsBTmeshGW_device = lsBTmeshGW_device;
    //     strcpy(lsBTmeshGW_device->deviceName, "ls_device_name1");
    //     strcpy(lsBTmeshGW_device->devicePid, "asdfkKDSkfls12");
    //     lsBTmeshGW_device->eleCata = 11310 + i;  
    // }
    status = pb_encode(&stream, commonMessage_fields, &com_msg);
    *size = stream.bytes_written;
    size_t decode_size = stream.bytes_written;
    printf("encode status:%d len:%ld %ld\n", status, stream.bytes_written, decode_size);

    // free(com_msg.combo.arg);
    // com_msg.combo.arg = NULL;
    
    commonMessage com_msg1 = commonMessage_init_zero;
    printf("buf:%s\n", buf);
    pb_istream_t istream = pb_istream_from_buffer((pb_byte_t *)buf, decode_size);
    status = pb_decode(&stream, commonMessage_fields, &com_msg1);

    // printf("decoding failed: %s\n", PB_GET_ERROR(&stream));
    printf("status = %d, %d\n", status, com_msg1.combo_size);
    // printf("name = %s, %p\n", com_msg1.deviceName, com_msg1.deviceName);
    return buf;
}

static void nano_pb_decode(char *buf, size_t size)
{
    commonMessage com_msg1 = commonMessage_init_zero;
    // Gatewayinfo gw_info = Gatewayinfo_init_zero;
    // char *buf = NULL;
    bool status = 0;
    // size_t message_length;
    int i, j;

    // buf = calloc(2, sizeof(commonMessage));
    printf("nano_pb_decode size = %ld\n", size);
    pb_istream_t stream = pb_istream_from_buffer((pb_byte_t *)buf, size);
    // com_msg.combo.funcs.decode = decode_combineMsg;

    status = pb_decode(&stream, commonMessage_fields, &com_msg1);


    // printf("count = %d, %p\n", com_msg1.combo_size, com_msg1.combo);

    /* if(com_msg1.combo)  */{
    }
    // else {
    //     printf("no combo\n");
    // }
}

int main(int argc, char const *argv[])
{
    // size_t message_length;
    char *buf = NULL;
    size_t size = 0;
    buf = nano_pb_encode(&size);
    printf("size = %ld\n", size);

    // nano_pb_decode(buf, size);
    // pb_release(buf);

    free(buf);

    return 0;
}
