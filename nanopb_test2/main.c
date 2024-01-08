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
 * @LastEditTime : 2023-05-09 16:26:46
 * @FilePath     : /My_C_Test/nanopb_test2/main.c
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

#if 0
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
#endif

#if 0
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
#endif

static char *nano_pb_encode(size_t *size)
{
    commonMessage com_msg = commonMessage_init_zero;
    Gatewayinfo gw_info = Gatewayinfo_init_zero;
    // uint32_t combo_size = 2;
    bool status = 0;
    // size_t message_length;
    int i = 0;

    printf("msg size = %ld\n", sizeof(com_msg));

    char *buf = NULL;

    serialNumber++;

    // buf = calloc(TEST_DEVICE_NEMBER * 10, sizeof(commonMessage));
    // pb_ostream_t stream = pb_ostream_from_buffer((pb_byte_t *)buf, sizeof(commonMessage) * TEST_DEVICE_NEMBER * 10);
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
    // combineMsg **combo = (combineMsg **)calloc(combo_size, sizeof(combineMsg *));
    // com_msg.combo.arg = encode_combineMsg_test();
    // printf(" com_msg.combo.arg = %p\n", com_msg.combo.arg);
    // com_msg.combo.funcs.encode = encode_combineMsg;
    //com_msg.combo.arg=one_combo;
    // com_msg.combo_size = TEST_DEVICE_NEMBER;
    com_msg.combo_size = TEST_DEVICE_NEMBER;
    // com_msg.combo = (struct _combineMsg *)encode_combineMsg_test();
    // combineMsg combo[pb_arraysize(commonMessage, combo[0])] = {combineMsg_init_zero};
    // Datapoint dpoint[pb_arraysize(leshiBTmeshGWMsg, dp[0])] = {Datapoint_init_zero};
    
    for (i = 0; i < TEST_DEVICE_NEMBER; i++) {
        sizeof(leshiBTmeshGWMsg);
        leshiBTmeshGWMsg *ls_msg = com_msg.combo[i].item.lsBTmeshGW_device;
        switch (i % 2) {
            case 0: {
                // printf("instream = %d\n", i);
                com_msg.combo[i].index = i;
                com_msg.combo[i].which_item = combineMsg_lsBTmeshGW_device_tag;
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
                ls_msg->dp_size = 4;
                // memset(dpoint, 0, pb_arraysize(leshiBTmeshGWMsg, dp[0]) * sizeof(Datapoint));
                ls_msg->dp[0].dpid = 1;
                ls_msg->dp[0].type = messageDpType_DP_TYPE_RAW;
                ls_msg->dp[0].len = 10;
                ls_msg->dp[0].which_t_value = Datapoint_v_raw_tag;
                memcpy((char *)ls_msg->dp[0].t_value.v_raw, "0123456789", sizeof("0123456789"));

                ls_msg->dp[1].dpid = 1;
                ls_msg->dp[1].type = messageDpType_DP_TYPE_VALUE;
                ls_msg->dp[1].len = 4;
                ls_msg->dp[1].which_t_value = Datapoint_v_value_tag;
                ls_msg->dp[1].t_value.v_value = 1000;
                
                ls_msg->dp[2].dpid = 1;
                ls_msg->dp[2].type = messageDpType_DP_TYPE_STRING;
                ls_msg->dp[2].len = 11;
                ls_msg->dp[2].which_t_value = Datapoint_v_string_tag;
                memcpy((char *)ls_msg->dp[2].t_value.v_string, "0123456789a", sizeof("0123456789a"));

                ls_msg->dp[3].dpid = 1;
                ls_msg->dp[3].type = messageDpType_DP_TYPE_ENUM;
                ls_msg->dp[3].len = 1;
                ls_msg->dp[3].which_t_value = Datapoint_v_enum_tag;
                ls_msg->dp[3].t_value.v_enum = 11;
                
                // ls_msg->dp = &dpoint;
            }
                break;
            case 1: {
                // printf("instream = %d\n", i);
                com_msg.combo[i].index = i;
                com_msg.combo[i].which_item = combineMsg_lsBTmeshGW_device_tag;
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
                ls_msg->dp_size = 3;
                // memset(dpoint, 0, pb_arraysize(leshiBTmeshGWMsg, dp[0]) * sizeof(Datapoint));
                ls_msg->dp[0].dpid = 1;
                ls_msg->dp[0].type = messageDpType_DP_TYPE_RAW;
                ls_msg->dp[0].len = 5;
                ls_msg->dp[0].which_t_value = Datapoint_v_raw_tag;
                memcpy((char *)ls_msg->dp[1].t_value.v_raw, "abcde", sizeof("abcde"));

                ls_msg->dp[1].dpid = 1;
                ls_msg->dp[1].type = messageDpType_DP_TYPE_VALUE;
                ls_msg->dp[1].len = 4;
                ls_msg->dp[1].which_t_value = Datapoint_v_value_tag;
                ls_msg->dp[1].t_value.v_value = 500;
                
                ls_msg->dp[2].dpid = 1;
                ls_msg->dp[2].type = messageDpType_DP_TYPE_STRING;
                ls_msg->dp[2].len = 20;
                ls_msg->dp[2].which_t_value = Datapoint_v_string_tag;
                memcpy((char *)ls_msg->dp[2].t_value.v_string, "0123459876543216789a", sizeof("0123459876543216789a"));
                
                // ls_msg->dp = &dpoint;
                
            }
                break;
            default:
                break;
        }
    }
    // com_msg.combo = &combo;
    
        // printf("array size = %d\n", pb_arraysize());
        // pb_add_pointer(com_msg.combo, sizeof(combineMsg), (pb_byte_t *)encode_combineMsg_test());
        // for ( i = 0; i < TEST_DEVICE_NEMBER; i++)
        // {
        //     if (!pb_encode_submessage(&stream, combineMsg_fields, &com_msg.combo[i])) {
        //             printf("pb_encode_submessage error\n");
        //             return false;
        //         }
        // }
        // printf("pb_encode_submessage %p\n", com_msg.combo);
        // for ( i = 0; i < TEST_DEVICE_NEMBER; i++)
        // {
        //     printf("===========encode_combineMsg===========\n");
        //     printf("%s\n", com_msg.combo[i].item.lsBTmeshGW_device.deviceId);
        //     printf("%s\n", com_msg.combo[i].item.lsBTmeshGW_device.deviceName);
        //     printf("%s\n", com_msg.combo[i].item.lsBTmeshGW_device.devicePid);
        //     printf("%s\n", com_msg.combo[i].item.lsBTmeshGW_device.deviceVer);
        //     printf("%s\n", com_msg.combo[i].item.lsBTmeshGW_device.hb_time);
        //     printf("==================end==================\n");
        // }
    size_t test_size=0;
    pb_get_encoded_size(&test_size, commonMessage_fields, &com_msg);
    printf("test_size = %d\n", test_size);
    buf = calloc(test_size, sizeof(char));
    pb_ostream_t stream = pb_ostream_from_buffer((pb_byte_t *)buf, test_size);
    status = pb_encode(&stream, commonMessage_fields, &com_msg);
    *size = stream.bytes_written;
    printf("encode status:%d len:%ld %zu\n", status, stream.bytes_written, sizeof(com_msg));

    // free(com_msg.combo.arg);
    // com_msg.combo.arg = NULL;

    // free(buf);
#if 0
    commonMessage com_msg1 = commonMessage_init_zero;
    // Gatewayinfo gw_info = Gatewayinfo_init_zero;
    // char *buf = NULL;
    // bool status = 0;
    // size_t message_length;
    // int i;

    // buf = calloc(2, sizeof(commonMessage));
    printf("nano_pb_decode size = %ld\n", *size);
    pb_istream_t istream = pb_istream_from_buffer((pb_byte_t *)buf, *size);
    // com_msg.combo.funcs.decode = decode_combineMsg;

    status = pb_decode(&istream, commonMessage_fields, &com_msg1);
    printf("decode status:%d type = %d, group id = %d\n", status, com_msg1.type, com_msg1.groupId);
    printf("%s\n", com_msg1.gw_info.gw_macaddr);
    printf("%s\n", com_msg1.gw_info.v);

    printf("count = %d, %p\n", com_msg1.combo_size, com_msg1.combo);

    if(com_msg1.combo) {
        for(i = 0; i < com_msg1.combo_size; i++) {
            if(com_msg1.combo[i]->which_item == combineMsg_lsBTmeshGW_device_tag) {
                printf("%s\n",com_msg1.combo[i]->item.lsBTmeshGW_device.deviceId);
                printf("%s\n",com_msg1.combo[i]->item.lsBTmeshGW_device.deviceName);
                printf("%s\n",com_msg1.combo[i]->item.lsBTmeshGW_device.devicePid);
                printf("%s\n",com_msg1.combo[i]->item.lsBTmeshGW_device.deviceVer);
                printf("%s\n",com_msg1.combo[i]->item.lsBTmeshGW_device.hb_time);
            }
        }
    }
    else {
        printf("no combo\n");
    }
#endif
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
    printf("decode status:%d type = %d, group id = %d\n", status, com_msg1.type, com_msg1.groupId);
    printf("%s\n", com_msg1.gw_info.gw_macaddr);
    printf("%s\n", com_msg1.gw_info.v);

    printf("count = %d, %p\n", com_msg1.combo_size, com_msg1.combo);

    /* if(com_msg1.combo)  */{
        for(i = 0; i < com_msg1.combo_size; i++) {
            if(com_msg1.combo[i].which_item == combineMsg_lsBTmeshGW_device_tag) {
                printf("=============decode[%d]start=============\n", i);
                printf("deviceId: %s\n",com_msg1.combo[i].item.lsBTmeshGW_device.deviceId);
                printf("deviceName: %s\n",com_msg1.combo[i].item.lsBTmeshGW_device.deviceName);
                printf("devicePid: %s\n",com_msg1.combo[i].item.lsBTmeshGW_device.devicePid);
                printf("deviceVer: %s\n",com_msg1.combo[i].item.lsBTmeshGW_device.deviceVer);
                printf("hb_time: %s\n",com_msg1.combo[i].item.lsBTmeshGW_device.hb_time);

                printf("dp_size: %d\n", com_msg1.combo[i].item.lsBTmeshGW_device.dp_size);
                #if 1
                for(j = 0; j < com_msg1.combo[i].item.lsBTmeshGW_device.dp_size; j++) {
                    printf("dpid[%d]: %d\n",j, com_msg1.combo[i].item.lsBTmeshGW_device.dp[j].dpid);
                    printf("type[%d]: %d\n",j, com_msg1.combo[i].item.lsBTmeshGW_device.dp[j].type);
                    printf("len[%d]: %d\n",j, com_msg1.combo[i].item.lsBTmeshGW_device.dp[j].len);
                    switch(com_msg1.combo[i].item.lsBTmeshGW_device.dp[j].which_t_value) {
                        case Datapoint_v_raw_tag:
                        printf("v_raw[%d]: %s\n",j, com_msg1.combo[i].item.lsBTmeshGW_device.dp[j].t_value.v_raw);
                            break;
                        case Datapoint_v_bool_tag:
                        printf("v_bool[%d]: %d\n",j, com_msg1.combo[i].item.lsBTmeshGW_device.dp[j].t_value.v_bool);
                            break;
                        case Datapoint_v_value_tag:
                        printf("v_value[%d]: %d\n",j, com_msg1.combo[i].item.lsBTmeshGW_device.dp[j].t_value.v_value);
                            break;
                        case Datapoint_v_string_tag:
                        printf("v_string[%d]: %s\n",j, com_msg1.combo[i].item.lsBTmeshGW_device.dp[j].t_value.v_string);
                            break;
                        case Datapoint_v_enum_tag:
                        printf("v_enum[%d]: %d\n",j, com_msg1.combo[i].item.lsBTmeshGW_device.dp[j].t_value.v_enum);
                            break;
                        case Datapoint_v_bitmap_tag:
                        printf("v_bitmap[%d]: %d\n",j, com_msg1.combo[i].item.lsBTmeshGW_device.dp[j].t_value.v_bitmap);
                            break;
                    }
                }
            #endif
                printf("=============decode end=============\n");
            }
        }
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

    nano_pb_decode(buf, size);
    // pb_release(buf);

    free(buf);

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