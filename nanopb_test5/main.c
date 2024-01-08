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
 * @LastEditTime : 2023-05-12 09:21:29
 * @FilePath     : /My_C_Test/nanopb_test5/main.c
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
#define BK_FAIL            (-1)
#define BK_OK              (0)

#define REQUIRE(in, tag)                  \
    do {                                  \
        if (in) {                         \
            printf("%s %s\n", #tag, #in); \
            goto tag;                     \
        }                                 \
    } while (0)

typedef struct {
    void *data;
    size_t size;
    size_t index;
} com_type_t;

static int _ls_bt_gw_event_encode_post(int event_id, const pb_msgdesc_t *fields, const void *src_struct)
{
    int ret = BK_FAIL;
    size_t malloc_size = 0;
    bool status = false;
    pb_byte_t *buf = NULL;

    if (!pb_get_encoded_size(&malloc_size, fields, src_struct)) {
        printf("get encode size failed!\n");
        ret = BK_FAIL;
        goto Error;
    }
    if (malloc_size > 0) {
        buf = (pb_byte_t *)calloc(malloc_size, sizeof(pb_byte_t));
        REQUIRE(buf == NULL, Error);
        pb_ostream_t stream = pb_ostream_from_buffer(buf, malloc_size);

        status = pb_encode(&stream, fields, src_struct);
        if (!status) {
            printf("pb_encode failed");
            ret = BK_FAIL;
            goto Error;
        }
        printf("encode status:%d len:%ld\n", status, stream.bytes_written);

        ret = BK_OK;
    }
Error:
    if (buf)
        free(buf);

    return ret;
}

static char *nano_pb_encode(size_t *size)
{
    return NULL;
}

static void nano_pb_decode(char *buf, size_t size)
{
    return 0;
}

int main(int argc, char **argv)
{
    leshiBTmeshDeviceMsg *ls_dev = (leshiBTmeshDeviceMsg *)calloc(1, sizeof(leshiBTmeshDeviceMsg));
    REQUIRE(ls_dev == NULL, Error);

    strcpy(ls_dev->devicePid, "lroj54nnitbqwfct");
    strcpy(ls_dev->deviceId, "b2838807f1");
    strcpy(ls_dev->deviceVer, "1.0.1");
    _ls_bt_gw_event_encode_post(messageType_MSGCODE_ADD_SUB_DEVICE_RSP, leshiBTmeshDeviceMsg_fields, (const void *)ls_dev);
Error:
    return 0;
}
#if 0
void common_func()
{
    commonMessage com_msg = commonMessage_init_zero;
    bool status = false;
    char *buf = NULL;
    int ret = -1;

    buf = os_zalloc(LS_BT_GW_MALLOC_MAX_SIZE);
    BK_ASSERT(buf != NULL);
    pb_ostream_t stream = pb_ostream_from_buffer((pb_byte_t *)buf, LS_BT_GW_MALLOC_MAX_SIZE);

    com_msg.type = messageType_MSGCODE_GET_GATEWAY_INFO_RSP;
    com_msg.has_dir = true;
    com_msg.dir.dst = messageDirection_MSG_DIR_GWSERVICE;
    com_msg.dir.src = messageDirection_MSG_DIR_BLESERVICE;
    com_msg.serial = (++serialNumber);
    
    /* process code */

    LS_BT_GW_LOGI("encode status:%d len:%ld\n", status, stream.bytes_written);
    ret = ls_bt_gw_event_post(messageType_MSGCODE_GET_GATEWAY_INFO_RSP, buf, stream.bytes_written);
    if (ret != BK_OK) {
        LS_BT_GW_LOGE("event post failed");
        goto Error;
    }

    ret = BK_OK;

Error:
    if (buf)
        os_free(buf);

    return ret;
}
#endif