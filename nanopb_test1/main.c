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
 * @LastEditTime : 2023-05-05 16:23:27
 * @FilePath     : /My_C_Test/nanopb_test1/main.c
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
#define TEST_DEVICE_NEMBER (2)
#define TEST_DPOINT_NEMBER (6)

typedef struct {
    void *data;
    size_t size;
    size_t index;
} com_type_t;

bool encode_type_raw(pb_ostream_t *ostream, const pb_field_t *field, void *const *arg)
{
    if (!pb_encode_tag_for_field(ostream, field)) {
        printf("pb_encode_tag_for_field error\n");
        return false;
    }
    if (!pb_encode_submessage(ostream, type_raw_fields, *arg)) {
        printf("pb_encode_submessage error\n");
        return false;
    }
    return true;
}

bool encode_type_string(pb_ostream_t *ostream, const pb_field_t *field, void *const *arg)
{
    if (!pb_encode_tag_for_field(ostream, field)) {
        printf("pb_encode_tag_for_field error\n");
        return false;
    }
    if (!pb_encode_submessage(ostream, type_string_fields, *arg)) {
        printf("pb_encode_submessage error\n");
        return false;
    }
    return true;
    return true;   
}

bool encode_dpoint(pb_ostream_t *ostream, const pb_field_t *field, void *const *arg)
{
    Datapoint *dpoint = (Datapoint *)*arg;
    int i = 0;

    for (i = 0; i < TEST_DPOINT_NEMBER; i++) {
        if (!pb_encode_tag_for_field(ostream, field)) {
            printf("pb_encode_tag_for_field error\n");
            return false;
        }
        if (!pb_encode_submessage(ostream, Datapoint_fields, &dpoint[i])) {
            printf("pb_encode_submessage error\n");
            return false;
        }
    }
}

static void *encode_combineMsg_test()
{
    combineMsg *one_combo = (combineMsg *)calloc(TEST_DEVICE_NEMBER, sizeof(combineMsg));
    int i = 0;
    printf("one_combo = %p\n", one_combo);
    for (i = 0; i < TEST_DEVICE_NEMBER; i++) {
        leshiBTmeshGWMsg *ls_msg = &one_combo[i].item.lsBTmeshGW_device;
        switch (i % 2) {
            case 0:
                // printf("instream = %d\n", i);
                one_combo[i].index = i;
                one_combo[i].which_item = combineMsg_lsBTmeshGW_device_tag;
                memcpy((char *)ls_msg->deviceId, "01d1dfb06a", sizeof("01d1dfb06a"));
                memcpy((char *)ls_msg->deviceName, "Hope_test_light1", sizeof("Hope_test_light1"));
                ls_msg->eleCata = 110801;
                memcpy((char *)ls_msg->devicePid, "fkxcslivaluonzdp", sizeof("fkxcslivaluonzdp"));
                ls_msg->online = 1;
                ls_msg->dp_size = 2;
                ls_msg->groupId = 1;
                ls_msg->which_rets = leshiBTmeshGWMsg_ret_tag;
                ls_msg->rets.ret = 1;
                memcpy((char *)ls_msg->deviceVer, "V1.2.3", sizeof("V1.2.3"));
                memcpy((char *)ls_msg->hb_time, "20223-04-27-15_53", sizeof("20223-04-27-15_53"));
                // ls_msg->dp.funcs.encode = encode_dpoint;
                // ls_msg->dp.arg = ();
                break;
            case 1:
                // printf("instream = %d\n", i);
                one_combo[i].index = i;
                one_combo[i].which_item = combineMsg_lsBTmeshGW_device_tag;
                memcpy((char *)ls_msg->deviceId, "b09451bb6d", sizeof("b09451bb6d"));
                memcpy((char *)ls_msg->deviceName, "Hope_test_light2", sizeof("Hope_test_light2"));
                ls_msg->eleCata = 110801;
                memcpy((char *)ls_msg->devicePid, "h536ubqx1tui1jsj", sizeof("h536ubqx1tui1jsj"));
                ls_msg->online = 1;
                ls_msg->dp_size = 2;
                // ls_msg->dp = (Datapoint **)calloc(2, sizeof(Datapoint *));
                ls_msg->groupId = 1;
                ls_msg->which_rets = leshiBTmeshGWMsg_ret_tag;
                ls_msg->rets.ret = 1;
                memcpy((char *)ls_msg->deviceVer, "V1.0.3", sizeof("V1.0.3"));
                memcpy((char *)ls_msg->hb_time, "20223-04-27-15_53", sizeof("20223-04-27-15_53"));
                break;
            default:
                break;
        }
    }
    return (void *)one_combo;
}
bool encode_combineMsg(pb_ostream_t *ostream, const pb_field_t *field, void *const *arg)
{
    // combineMsg one_combo = combineMsg_init_default;
    combineMsg *one_combo = (combineMsg *)*arg;
    int i = 0;
    printf("arg = %p\n", *arg);

    for (i = 0; i < TEST_DEVICE_NEMBER; i++) {
        if (!pb_encode_tag_for_field(ostream, field)) {
            printf("pb_encode_tag_for_field error\n");
            return false;
        }
        if (!pb_encode_submessage(ostream, combineMsg_fields, &one_combo[i])) {
            printf("pb_encode_submessage error\n");
            return false;
        }
    }
}

bool decode_combineMsg(pb_istream_t *ostream, const pb_field_t *field, void *const *arg)
{
    combineMsg one_combo = combineMsg_init_zero;
    int i = 0;

    if (!pb_decode(ostream, combineMsg_fields, &one_combo)) {
        printf("pb_decode error\n");
        return false;
    }

    printf("===========decode_combineMsg===========\n");
    printf("%d\n", one_combo.index);
    // printf("%s\n", one_combo.item.lsBTmeshGW_device.deviceId);
    // printf("%s\n", one_combo.item.lsBTmeshGW_device.deviceName);
    // printf("%s\n", one_combo.item.lsBTmeshGW_device.devicePid);
    // printf("%s\n", one_combo.item.lsBTmeshGW_device.deviceVer);
    // printf("%s\n", one_combo.item.lsBTmeshGW_device.hb_time);
    // printf("==================end==================\n");
}

static char *nano_pb_encode(size_t *size)
{
    commonMessage com_msg = commonMessage_init_zero;
    Gatewayinfo gw_info = Gatewayinfo_init_zero;
    // uint32_t combo_size = 2;
    bool status = 0;
    size_t message_length;

    printf("msg size = %d\n", sizeof(com_msg));

    char *buf = NULL;

    serialNumber++;

    buf = calloc(TEST_DEVICE_NEMBER, sizeof(commonMessage));
    pb_ostream_t stream = pb_ostream_from_buffer((pb_byte_t *)buf, sizeof(commonMessage) * TEST_DEVICE_NEMBER);
    com_msg.type = messageType_MSGCODE_JOIN_ACTION_REQ;
    com_msg.has_dir = true;
    com_msg.dir.dst = messageDirection_MSG_DIR_GWSERVICE;
    com_msg.dir.src = messageDirection_MSG_DIR_BLESERVICE;
    com_msg.serial = serialNumber;
    com_msg.has_gw_info = true;

    gw_info.cap = 1;
    gw_info.m = 2;
    memcpy((char *)gw_info.v, "v1.0.1", sizeof("v1.0.1"));
    memcpy((char *)gw_info.gw_macaddr, "ab113f0a", sizeof("ab113f0a"));

    memcpy(&com_msg.gw_info, &gw_info, sizeof(Gatewayinfo));
    com_msg.groupId = 0x01;
    com_msg.has_ota = 0;
    com_msg.combo_size = TEST_DEVICE_NEMBER;
    // combineMsg **combo = (combineMsg **)calloc(combo_size, sizeof(combineMsg *));
    com_msg.combo.arg = encode_combineMsg_test();
    printf(" com_msg.combo.arg = %p\n", com_msg.combo.arg);
    com_msg.combo.funcs.encode = encode_combineMsg;
    //com_msg.combo.arg=one_combo;

    status = pb_encode(&stream, commonMessage_fields, &com_msg);
    *size = stream.bytes_written;
    printf("encode status:%d len:%d %zu\n", status, stream.bytes_written, sizeof(com_msg));

    free(com_msg.combo.arg);
    com_msg.combo.arg = NULL;

    // free(buf);

    return buf;
}

static void nano_pb_decode(char *buf, size_t size)
{
    commonMessage com_msg = commonMessage_init_zero;
    // Gatewayinfo gw_info = Gatewayinfo_init_zero;
    // char *buf = NULL;
    bool status = 0;
    size_t message_length;
    int i;

    // buf = calloc(2, sizeof(commonMessage));
    pb_istream_t stream = pb_istream_from_buffer((pb_byte_t *)buf, size);
    com_msg.combo.funcs.decode = decode_combineMsg;

    status = pb_decode(&stream, commonMessage_fields, &com_msg);
    printf("decode status:%d type = %d, group id = %d\n", status, com_msg.type, com_msg.groupId);
    printf("%s\n", com_msg.gw_info.gw_macaddr);
    printf("%s\n", com_msg.gw_info.v);

    // if(com_msg.combo) {
    //     for(i = 0; i < com_msg.combo_size; i++) {
    //         if(com_msg.combo[i]->which_item == combineMsg_lsBTmeshGW_device_tag) {
    //             printf("%s\n",com_msg.combo[i]->item.lsBTmeshGW_device.deviceId);
    //             printf("%s\n",com_msg.combo[i]->item.lsBTmeshGW_device.deviceName);
    //             printf("%s\n",com_msg.combo[i]->item.lsBTmeshGW_device.devicePid);
    //             printf("%s\n",com_msg.combo[i]->item.lsBTmeshGW_device.deviceVer);
    //             printf("%s\n",com_msg.combo[i]->item.lsBTmeshGW_device.hb_time);
    //         }
    //     }
    // }
    // else {
    //     printf("no combo\n");
    // }
}

int main(int argc, char const *argv[])
{
    size_t message_length;
    char *buf = NULL;
    size_t size = 0;
    buf = nano_pb_encode(&size);
    printf("size = %d\n", size);

    nano_pb_decode(buf, size);

    free(buf);

    return 0;
}
