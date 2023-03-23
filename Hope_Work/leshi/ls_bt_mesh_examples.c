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
 * @Date         : 2023-03-21 12:50:39
 * @LastEditors  : MCD
 * @LastEditTime : 2023-03-23 11:09:24
 * @FilePath     : /My_C_Test/Hope_Work/leshi/ls_bt_mesh_examples.c
 * @Description  : 
 * 
 * ******************************************
 */
#include "ls_bt_mesh_examples.h"
#include "common.h"
#include "ls_bt_mesh.h"

// {"pid":"fkxcslivaluonzdp","cids":["01d1dfb06a"],"ver":"1.0.2"}   调光灯
// {"pid":"5zkpavp5cxonvvxl","cids":["b223f59481"],"ver":"1.0.1"}   调光灯
// {"pid":"lroj54nnitbqwfct","cids":["b2838807f1"],"ver":"1.0.1"}   4键开关
// {"pid":"h536ubqx1tui1jsj","cids":["b09451bb6d"],"ver":"1.0.2"}   电机

void leshi_sure_band(void)
{
    leshi_mesh_t test_cmd;
    // uint8_t *data = "{\"cid\":[\"b2838807f1\", \"b223f59481\"],\"rets\":[0, 0]}";
    char *data = "{\"cid\":[\"01d1dfb06a\"],\"rets\":[0]}";
    // uint32_t lenght = 0;
    int i = 0;
    uint32_t sum = 0;

    test_cmd.header.head[0] = 0x55;
    test_cmd.header.head[1] = 0xaa;
    test_cmd.header.version = 0x00;
    test_cmd.header.cmd = 0x13;
    uint16_t datalen = strlen(data);
    printf("datalen = %d\n", datalen);
    test_cmd.header.data_len[0] = HI_UINT16(datalen);
    test_cmd.header.data_len[1] = LO_UINT16(datalen);
    test_cmd.data = (uint8_t *)data;

    // lenght = sizeof(leshi_mesh_header_t) + datalen;
    uint8_t *header = (uint8_t *)&test_cmd.header;
    for (i = 0; i < sizeof(leshi_mesh_header_t); i++) {
        printf("%02x ", header[i]);
        sum += header[i];
        // printf("sum = %x\n", sum);
    }
    for (i = 0; i < datalen; i++) {
        printf("%02x ", test_cmd.data[i]);
        sum += test_cmd.data[i];
        // printf("sum = %x\n", sum);
    }
    printf("%02x\n", sum % 256);
    printf("\n");

    printf("sum = %02x\n", sum % 256);
}

static void _leshi_show_dp(_leshi_dp_t *dp)
{
    if (NULL == dp)
        return;

    printf("dpid = %d, type = %d, len = %d ", dp->dpid, dp->type, BUILD_UINT16(dp->len[1], dp->len[0]));
    switch (dp->type) {
        case DP_TYPE_RAW:
            break;
        case DP_TYPE_BOOL:
        case DP_TYPE_ENUM:
            printf("value = %d\n", dp->value[0]);
            break;
        case DP_TYPE_VALUE:
            printf("value %d\n", BUILD_UINT32(dp->value[3], dp->value[2], dp->value[1], dp->value[0]));
            break;
        case DP_TYPE_STRING:
            printf("value %s\n", dp->value);
            break;
        case DP_TYPE_BITMAP:
            break;

        default:
            break;
    }
}

void leshi_datapoint_parse(void)
{
    /* 4键开关 */
    // uint8_t data[] = {0x0A, 0x62, 0x32, 0x38, 0x33, 0x38, 0x38, 0x30, 0x37, 0x66, 0x31,
    //                   0x01, 0x01, 0x00, 0x01, 0x01,
    //                   0x02, 0x01, 0x00, 0x01, 0x01,
    //                   0x03, 0x01, 0x00, 0x01, 0x01,
    //                   0x04, 0x01, 0x00, 0x01, 0x01,
    //                   0x68, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x02,
    //                   0x6A, 0x03, 0x00, 0x08, 0x66, 0x31, 0x30, 0x37, 0x38, 0x38, 0x38, 0x33};

    /* 电机 */
    // uint8_t data[] = {0x0A, 0x62, 0x30, 0x39, 0x34, 0x35, 0x31, 0x62, 0x62, 0x36, 0x64,
    //                   0x02, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x64,
    //                   0x03, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x64,
    //                   0x0B, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x64,
    //                   0x6A, 0x03, 0x00, 0x08, 0x36, 0x64, 0x62, 0x62, 0x35, 0x31, 0x39, 0x34};

    /* 调光开关 */
    uint8_t data[] = {0x0A, 0x30, 0x31, 0x64, 0x31, 0x64, 0x66, 0x62, 0x30, 0x36, 0x61,
                      0x14, 0x01, 0x00, 0x01, 0x01, 
                      0x16, 0x02, 0x00, 0x04, 0x00, 0x00, 0x03, 0xE8, 
                      0x17, 0x02, 0x00, 0x04, 0x00, 0x00, 0x02, 0x79, 
                      0x6A, 0x03, 0x00, 0x08, 0x36, 0x61, 0x62, 0x30, 0x64, 0x66, 0x64, 0x31};

    uint8_t data_len;

    leshi_ctrl_data_t *ctrl_data;
    _leshi_dp_t *dp;
    uint16_t len = 0;
    uint16_t tmp_len = 0;
    uint8_t dp_num = 0;
    uint8_t i = 0;

    // ctrl_data->datapoint = &dp;
    data_len = sizeof(data);
    printf("data len = %d\n", data_len);
    ctrl_data = (leshi_ctrl_data_t *)calloc(1, sizeof(leshi_ctrl_data_t));
    // REQUIRE(NULL == ctrl_data, Error);
    ctrl_data->id_len = data[0];
    len++;
    printf("id len = %d\n", ctrl_data->id_len);

    // 实际长度是10，但是需要多申请一位内存
    // 如果不多申请一位，可能会导致溢出
    ctrl_data->sub_id = calloc(ctrl_data->id_len + 1, sizeof(uint8_t));
    if (ctrl_data->sub_id) {
        memcpy(ctrl_data->sub_id, &data[1], ctrl_data->id_len);
        printf("sub id = %s, subid len = %ld\n", ctrl_data->sub_id, strlen((const char *)(ctrl_data->sub_id)));
        len += ctrl_data->id_len;
        printf("len = %d\n", len);
    }
    else {
        printf("malloc failed!!");
        return;
    }
    tmp_len = len;

    _leshi_dp_t tdp;
    while (data_len > tmp_len) {
        memset(&tdp, 0, sizeof(_leshi_dp_t));
        memcpy(&tdp, &data[tmp_len], LESHI_DATAPOINT_HEADER_LEN);
        tmp_len += LESHI_DATAPOINT_HEADER_LEN;
        tmp_len += BUILD_UINT16(tdp.len[1], tdp.len[0]);
        dp_num++;
    }
    printf("get dp num = %d\n", dp_num);
    ctrl_data->dp_num = dp_num;
    if (dp_num > 0) {
        ctrl_data->datapoint = (_leshi_dp_t *)calloc(dp_num, sizeof(_leshi_dp_t));
        while (data_len > len) {
            dp = &ctrl_data->datapoint[i];
            memcpy(dp, &data[len], 4);
            len += 4;
            uint16_t dp_len = BUILD_UINT16(dp->len[1], dp->len[0]);
            dp->value = calloc(dp_len + 1, sizeof(uint8_t));
            memcpy(dp->value, &data[len], dp_len);
            len += dp_len;
            i++;
        }
#if 1
        for (i = 0; i < dp_num; i++) {
            _leshi_show_dp(&ctrl_data->datapoint[i]);
        }
#endif
    }

    printf("finally len = %d\n", len);
    for (i = 0; i < dp_num; i++) {
        free(ctrl_data->datapoint[i].value);
    }
    free(ctrl_data->datapoint);
    free(ctrl_data->sub_id);
    free(ctrl_data);
}

void leshi_ctrl_switch_dev(void)
{
    uint32_t i = 0;
    uint32_t sum = 0;
    /* 4键开关控制 */
    // uint8_t data_on[] = {0x55, 0xaa, 0x00, 0x0c, 0x00, 0x1f,
    //                      0x0A, 0x62, 0x32, 0x38, 0x33, 0x38, 0x38, 0x30, 0x37, 0x66, 0x31,
    //                      0x01, 0x01, 0x00, 0x01, 0x01,
    //                      0x02, 0x01, 0x00, 0x01, 0x01,
    //                      0x03, 0x01, 0x00, 0x01, 0x01,
    //                      0x04, 0x01, 0x00, 0x01, 0x01};
    /* 2键开关控制 */
    // uint8_t data_on[] = {0x55, 0xaa, 0x00, 0x0c, 0x00, 0x15,
    //                      0x0A, 0x62, 0x32, 0x32, 0x33, 0x66, 0x35, 0x39, 0x34, 0x38, 0x31,
    //                      0x01, 0x01, 0x00, 0x01, 0x01,
    //                      0x02, 0x01, 0x00, 0x01, 0x01};

    /* 蓝牙mesh电机 */
    // uint8_t data_on[] = {0x55, 0xaa, 0x00, 0x0c, 0x00, 0x13,
    //                      0x0A, 0x62, 0x30, 0x39, 0x34, 0x35, 0x31, 0x62, 0x62, 0x36, 0x64,
    //                      0x02, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x32};

    /* 调光开关控制 */
    uint8_t data_on[] = {0x55, 0xaa, 0x00, 0x0c, 0x00, 0x13,
                         0x0A, 0x30, 0x31, 0x64, 0x31, 0x64, 0x66, 0x62, 0x30, 0x36, 0x61,
                        //  0x16, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04};
                         0x17, 0x02, 0x00, 0x04, 0x00, 0x00, 0x03, 0xe8};

    int datalen = sizeof(data_on);
    for (i = 0; i < datalen; i++) {
        printf("%02x ", data_on[i]);
        sum += data_on[i];
        // printf("sum = %x\n", sum);
    }
    // printf("\n");

    printf("%02x\n", sum % 256);
}

void leshi_delete_one_dev(void)
{
    leshi_mesh_t test_cmd;
    // uint8_t *data = "{\"cid\":[\"b2838807f1\", \"b223f59481\"],\"rets\":[0, 0]}";
    char *data = "{\"sub_id\":\"b09451bb6d\",\"devkey\":\"01\"}"; /* ,\"devkey\":\"\" */
    // uint32_t lenght = 0;
    int i = 0;
    uint32_t sum = 0;

    test_cmd.header.head[0] = 0x55;
    test_cmd.header.head[1] = 0xaa;
    test_cmd.header.version = 0x00;
    test_cmd.header.cmd = 0x09;
    uint16_t datalen = strlen((const char *)data);
    printf("datalen = %d\n", datalen);
    test_cmd.header.data_len[0] = HI_UINT16(datalen);
    test_cmd.header.data_len[1] = LO_UINT16(datalen);
    test_cmd.data = (uint8_t *)data;

    // lenght = sizeof(leshi_mesh_header_t) + datalen;
    uint8_t *header = (uint8_t *)&test_cmd.header;
    for (i = 0; i < sizeof(leshi_mesh_header_t); i++) {
        printf("%02x ", header[i]);
        sum += header[i];
        // printf("sum = %x\n", sum);
    }
    for (i = 0; i < datalen; i++) {
        printf("%02x ", test_cmd.data[i]);
        sum += test_cmd.data[i];
        // printf("sum = %x\n", sum);
    }
    printf("%x\n", sum % 256);
    printf("\n");

    printf("sum = %x\n", sum % 256);
}

void leshi_addto_group(void)
{
    leshi_mesh_t test_cmd;
    // uint8_t *data = "{\"cid\":[\"b2838807f1\", \"b223f59481\"],\"rets\":[0, 0]}";
    char *data = "{\"gid\":\"01\",\"cids\":[\"b223f59481\"]}"; /* ,\"devkey\":\"\" */
    // uint32_t lenght = 0;
    int i = 0;
    uint32_t sum = 0;

    test_cmd.header.head[0] = 0x55;
    test_cmd.header.head[1] = 0xaa;
    test_cmd.header.version = 0x00;
    test_cmd.header.cmd = 0x0e;
    uint16_t datalen = strlen((const char *)data);
    printf("datalen = %d\n", datalen);
    test_cmd.header.data_len[0] = HI_UINT16(datalen);
    test_cmd.header.data_len[1] = LO_UINT16(datalen);
    test_cmd.data = (uint8_t *)data;

    // lenght = sizeof(leshi_mesh_header_t) + datalen;
    uint8_t *header = (uint8_t *)&test_cmd.header;
    for (i = 0; i < sizeof(leshi_mesh_header_t); i++) {
        printf("%02x ", header[i]);
        sum += header[i];
        // printf("sum = %x\n", sum);
    }
    for (i = 0; i < datalen; i++) {
        printf("%02x ", test_cmd.data[i]);
        sum += test_cmd.data[i];
        // printf("sum = %x\n", sum);
    }
    printf("%x\n", sum % 256);
    printf("\n");

    printf("sum = %x\n", sum % 256);
}

void leshi_group_ctrl_devs(void)
{
    uint32_t i = 0;
    uint32_t sum = 0;
    /* 4键开关控制 */
    uint8_t data_on[] = {0x55, 0xaa, 0x00, 0x14, 0x00, 0x17,
                         0x02, 0x30, 0x31,
                         0x01, 0x01, 0x00, 0x01, 0x00,
                         0x02, 0x01, 0x00, 0x01, 0x00,
                         0x03, 0x01, 0x00, 0x01, 0x00,
                         0x04, 0x01, 0x00, 0x01, 0x00};
    /* 2键开关控制 */
    // uint8_t data_on[] = {0x55, 0xaa, 0x00, 0x14, 0x00, 0x0D,
    //                      0x02, 0x30, 0x31,
    //                      0x01, 0x01, 0x00, 0x01, 0x00,
    //                      0x02, 0x01, 0x00, 0x01, 0x00};

    /* 蓝牙mesh电机 */
    // uint8_t data_on[] = {0x55, 0xaa, 0x00, 0x0c, 0x00, 0x13,
    //                      0x0A, 0x62, 0x30, 0x39, 0x34, 0x35, 0x31, 0x62, 0x62, 0x36, 0x64,
    //                      0x02, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x32};

    int datalen = sizeof(data_on);
    for (i = 0; i < datalen; i++) {
        printf("%02x ", data_on[i]);
        sum += data_on[i];
        // printf("sum = %x\n", sum);
    }
    // printf("\n");

    printf("%02x\n", sum % 256);
}

void leshi_delfrom_group(void)
{
    leshi_mesh_t test_cmd;
    // uint8_t *data = "{\"cid\":[\"b2838807f1\", \"b223f59481\"],\"rets\":[0, 0]}";
    char *data = "{\"gid\":\"01\",\"cids\":[\"b223f59481\"]}"; /* ,\"devkey\":\"\" */
    // uint32_t lenght = 0;
    int i = 0;
    uint32_t sum = 0;

    test_cmd.header.head[0] = 0x55;
    test_cmd.header.head[1] = 0xaa;
    test_cmd.header.version = 0x00;
    test_cmd.header.cmd = 0x0F;
    uint16_t datalen = strlen((const char *)data);
    printf("datalen = %d\n", datalen);
    test_cmd.header.data_len[0] = HI_UINT16(datalen);
    test_cmd.header.data_len[1] = LO_UINT16(datalen);
    test_cmd.data = (uint8_t *)data;

    // lenght = sizeof(leshi_mesh_header_t) + datalen;
    uint8_t *header = (uint8_t *)&test_cmd.header;
    for (i = 0; i < sizeof(leshi_mesh_header_t); i++) {
        printf("%02x ", header[i]);
        sum += header[i];
        // printf("sum = %x\n", sum);
    }
    for (i = 0; i < datalen; i++) {
        printf("%02x ", test_cmd.data[i]);
        sum += test_cmd.data[i];
        // printf("sum = %x\n", sum);
    }
    printf("%x\n", sum % 256);
    printf("\n");

    printf("sum = %x\n", sum % 256);
}

void leshi_heart_beat(void)
{
    leshi_mesh_t test_cmd;  //b09451bb6d
    // uint8_t *data = "{\"cid\":[\"b2838807f1\", \"b223f59481\"],\"rets\":[0, 0]}";
    char *data = "{\"sub_id\":\"b223f59481\"}";
    // uint32_t lenght = 0;
    int i = 0;
    uint32_t sum = 0;

    test_cmd.header.head[0] = 0x55;
    test_cmd.header.head[1] = 0xaa;
    test_cmd.header.version = 0x00;
    test_cmd.header.cmd = 0x0A;
    uint16_t datalen = strlen((const char *)data);
    printf("datalen = %d\n", datalen);
    test_cmd.header.data_len[0] = HI_UINT16(datalen);
    test_cmd.header.data_len[1] = LO_UINT16(datalen);
    test_cmd.data = (uint8_t *)data;

    // lenght = sizeof(leshi_mesh_header_t) + datalen;
    uint8_t *header = (uint8_t *)&test_cmd.header;
    for (i = 0; i < sizeof(leshi_mesh_header_t); i++) {
        printf("%02x ", header[i]);
        sum += header[i];
        // printf("sum = %x\n", sum);
    }
    for (i = 0; i < datalen; i++) {
        printf("%02x ", test_cmd.data[i]);
        sum += test_cmd.data[i];
        // printf("sum = %x\n", sum);
    }
    printf("%x\n", sum % 256);
    printf("\n");

    printf("sum = %x\n", sum % 256);
}
