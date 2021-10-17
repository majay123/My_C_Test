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
 * @Date         : 2021-07-15 15:07:39
 * @LastEditors  : MCD
 * @LastEditTime : 2021-09-28 10:57:12
 * @FilePath     : /My_C_Test/CRC16_MODBUS/crc16_modbus.c
 * @Description  : 
 * 
 * ******************************************
 */

#include "common.h"
#include "eide_rs485.h"
#include "rs485_dispatch.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// #define MY_RELLOC(data, num, type)           \
// do{if(data){free(data);data = calloc(num, sizeof(type));}}while(0);            
#define MY_RELLOC(data, num, type)              \
do{                                             \
    if(data){                                   \
        free(data);                             \
        data = NULL;}                           \
    if(num > 0) {data = calloc(num, sizeof(type));}           \
}while(0)

#define MY_RELLOC_REQUIRE(data, num, type, tag)                               \
do{                                             \
    MY_RELLOC(data, num, type);                 \
    if(!data){print_mcd("%s %s malloc is null", #tag, #data); goto tag;}}while(0)

#define PRINT_SIZEOF(type)      \
do{printf("%d\n",sizeof(type));}while(0);

// 该位称为预置值，使用人工算法（长除法）时
//需要将除数多项式先与该与职位 异或 ，才能得到最后的除数多项式
#define CRC16_CCITT_SEED (0x0000)
// 该位为简式书写 实际为0x11021
#define CRC16_CCITT_POLY16 (0x1021)

static unsigned short crc16_ccitt_table[256] =
    {
        0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
        0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
        0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
        0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
        0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
        0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
        0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
        0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
        0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
        0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
        0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
        0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
        0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
        0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
        0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
        0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
        0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
        0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
        0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
        0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
        0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
        0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
        0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
        0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
        0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
        0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
        0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
        0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
        0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
        0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
        0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
        0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0};
//CRC-16/CCITT
unsigned short do_crc16_CCITT(unsigned char *pbuf, size_t len)
{
    unsigned short crc = CRC16_CCITT_SEED;

    while (len-- > 0) {
        crc = crc16_ccitt_table[(crc >> 8 ^ *pbuf++) & 0xff] ^ (crc << 8);
    }

    return crc;
}

//CRC-16/XMODEM
unsigned short do_crc16_XMODEM(unsigned char *ptr, int len)
{
    unsigned int i;
    unsigned short crc = 0x0000;

    while (len--) {
        crc ^= (unsigned short)(*ptr++) << 8;
        for (i = 0; i < 8; ++i) {
            if (crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;
            else
                crc <<= 1;
        }
    }

    return crc;
}

unsigned short do_crc(unsigned char *ptr, int len)
{

    unsigned int i;
    unsigned short crc = 0x0000;

    while (len--) {

        crc ^= (unsigned short)(*ptr++) << 8;
        for (i = 0; i < 8; ++i) {

            if (crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;
            else
                crc <<= 1;
        }
    }

    return crc;
}

//CRC-16/MODBUS
unsigned short do_crc16_MODBUS(unsigned char *ptr, int len)
{
    unsigned char i;
    unsigned short crc = 0xFFFF;
    unsigned short ret = 0;
    if (len == 0) {
        len = 1;
    }
    while (len--) {
        crc ^= *ptr;
        for (i = 0; i < 8; i++) {
            if (crc & 1) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
        ptr++;
    }

    ret = (crc >> 8) & 0x00ff;
    ret = ret | ((crc << 8) & 0xff00);

    return (ret);
}

unsigned short CRC16_XMODEM(unsigned char *puchMsg, unsigned int usDataLen)
{
    unsigned short wCRCin = 0x0000;
    unsigned short wCPoly = 0x1021;
    unsigned char wChar = 0;

    while (usDataLen--) {
        wChar = *(puchMsg++);
        wCRCin ^= (wChar << 8);
        for (int i = 0; i < 8; i++) {
            if (wCRCin & 0x8000)
                wCRCin = (wCRCin << 1) ^ wCPoly;
            else
                wCRCin = wCRCin << 1;
        }
    }
    return (wCRCin);
}

int my_itoa(char *buf, int value, char size)
{
    char *p = buf;
    int i = 1;

    if (buf == NULL || size <= 0)
        return -1;
    for (i = 1; i <= size; i++) {
        *p = value >> (8 * (size - i));
        p++;
    }
    return 0;
}

static size_t _test_get_data_len(uint8_t data_type)
{
    size_t len = 0;

    switch (data_type) {
    case EIDE_DATA_TYPE_NO:
        break;
    case EIDE_DATA_TYPE_BOOLEAN:
    case EIDE_DATA_TYPE_CHAR:
    case EIDE_DATA_TYPE_UNSIGNED_CHAR:
        len += sizeof(char);
        break;
    case EIDE_DATA_TYPE_SHORE_INT:
    case EIDE_DATA_TYPE_UNSIGNED_SHORT_INT:
        len += sizeof(short);
        break;
    case EIDE_DATA_TYPE_INT:
    case EIDE_DATA_TYPE_UNSIGNED_INT:
    case EIDE_DATA_TYPE_FLOAT:
        len += sizeof(int);
        break;
    default:
        break;
    }

    return len;
}

static size_t _test_data_fill(eide_rs485_devs_ctrl_t *ctrl_data, eide_rs485_payload_t *payload)
{
    size_t len = 0;
    size_t data_len = 0;
    uint8_t ctrl_num = 0, func_num = 0;
    uint8_t data_type = 0;
    size_t i = 0, j = 0;
    uint8_t *data = payload->data;

    if (payload == NULL || ctrl_data == NULL)
        return -1;
    payload->data_code = EIDE_DATACODE_CONTROL;
    len += 1;
    ctrl_num = ctrl_data->ctrl_dev_count;
    *data = ctrl_num;
    data++;
    len += 1;
    for (i = 0; i < ctrl_num; i++) {
        *data = ctrl_data->ctrl[i].dev_id;
        len += 1;
        data++;
        func_num = ctrl_data->ctrl[i].dev_func_num;
        *data = func_num;
        len += 1;
        data++;
        for (j = 0; j < func_num; j++) {
            *data = ctrl_data->ctrl[i].funcs[j].func_id;
            len += 1;
            data++;
            data_type = ctrl_data->ctrl[i].funcs[j].data_type;
            // print_mcd("data_type = %d", data_type);
            *data = data_type;
            len += 1;
            data++;
            data_len= _test_get_data_len(data_type);
            // print_mcd("data len: %d", data_len);
            memcpy(data, ctrl_data->ctrl[i].funcs[j].data, data_len);
            len += data_len;
            data += data_len;
        }
    }

    return len;
}


static void _test_prase_msg(eide_rs485_payload_t *payload, size_t len)
{
    uint8_t *data = payload->data;
    eide_rs485_devs_ctrl_t rep_dev_ctrl;
    size_t i, j;
    uint8_t dev_num = 0, fun_num = 0, data_type = 0;
    size_t data_len = 0;
    for(i = 0; i < len; i++) {
        printf("%02x ", data[i]);
    }
    printf("\n");

    memset(&rep_dev_ctrl, 0, sizeof(eide_rs485_devs_ctrl_t));
    dev_num = *data;
    data++;
    rep_dev_ctrl.ctrl_dev_count = dev_num;
    for (i = 0; i < dev_num; i++) {
        rep_dev_ctrl.ctrl[i].dev_id = *data;
        data++;
        fun_num = *data;
        data++;
        rep_dev_ctrl.ctrl[i].dev_func_num = fun_num;
        for (j = 0; j < fun_num; j++) {
            rep_dev_ctrl.ctrl[i].funcs[j].func_id = *data;
            print_mcd("func id = %02x", rep_dev_ctrl.ctrl[i].funcs[j].func_id);
            data++;
            data_type = *data;
            rep_dev_ctrl.ctrl[i].funcs[j].data_type = data_type;
            data++;
            data_len = _test_get_data_len(data_type);
            if(data_len > 4)
                data_len = 4;
            memcpy(&rep_dev_ctrl.ctrl[i].funcs[j].data, data, data_len);
            data += data_len;
        }
    }
    
    print_mcd("get device num = %02x", rep_dev_ctrl.ctrl_dev_count);
    for(i = 0; i < rep_dev_ctrl.ctrl_dev_count; i++) {
        rep_dev_ctrl.ctrl[i].dev_id;
        print_mcd("dev id = %02x, dev func num = %02x", rep_dev_ctrl.ctrl[i].dev_id, rep_dev_ctrl.ctrl[i].dev_func_num);
        for(j = 0; j < rep_dev_ctrl.ctrl[i].dev_func_num; j++) {
            print_mcd("func id = %02x, data type = %02x", rep_dev_ctrl.ctrl[i].funcs[j].func_id, rep_dev_ctrl.ctrl[i].funcs[j].data_type);
            data_type = rep_dev_ctrl.ctrl[i].funcs[j].data_type;
            data_len = _test_get_data_len(data_type);
            size_t k = 0;
            for (k = 0; k < data_len; k++) {
                print_mcd("data = %02x", rep_dev_ctrl.ctrl[i].funcs[j].data[k]);
            }
        }
        printf("\n\n");
    }
}

static int test_ctrl_devs()
{
    eide_rs485_devs_ctrl_t ctrl_data;
    eide_rs485_devs_ctrl_t tmp_data;
    eide_rs485_payload_t payload;
    // eide_rs485_dev_ctrl_t **dev_ctrl = &ctrl_data.ctrl;
    int i = 0, j = 0;

    memset(&payload, 0, sizeof(eide_rs485_payload_t));
    memset(&ctrl_data, 0, sizeof(eide_rs485_devs_ctrl_t));

    ctrl_data.ctrl_dev_count = 4;
    for (i = 0; i < ctrl_data.ctrl_dev_count; i++) {
        ctrl_data.ctrl[i].dev_id = i + 1;
        if( i== 3)
            ctrl_data.ctrl[i].dev_func_num = 3;
        else
            ctrl_data.ctrl[i].dev_func_num = 2;
        for (j = 0; j < ctrl_data.ctrl[i].dev_func_num; j++) {
            ctrl_data.ctrl[i].funcs[j].func_id = j + 1;
            if (j == 0) {
                ctrl_data.ctrl[i].funcs[j].data_type = 1;
                ctrl_data.ctrl[i].funcs[j].data[0] = 1;
            } else {
                ctrl_data.ctrl[i].funcs[j].data_type = 3;
                ctrl_data.ctrl[i].funcs[j].data[0] = 50;
            }
        }
    }
    size_t len = _test_data_fill(&ctrl_data, &payload) - 1;
    // size_t len = 30;
    print_mcd("data len: %d", len);

    for(i = 0; i < len; i++) {
        printf("%02x ", payload.data[i]);
    }
    printf("\n");

    _test_prase_msg(&payload, len);
}


static void _test_macaddr_to_decstring(void )
{
    int i = 0;
    // mac addr hex to dec string
// func1
    uint8_t mac_addr[8] = {0xF0, 0x0A, 0x4F, 0x04, 0x10, 0x2D, 0x01, 0x01};
    uint8_t act_addr[25] = {0};
    uint8_t act_addr1[25] = {0};
    uint8_t act_addr2[25] = {0};
    int offset = 0;
    int offset1 = 0;
    snprintf(act_addr, sizeof(act_addr), "%02d%02d%02d%02d%02d%02d%02d%02d", mac_addr[0],mac_addr[1],mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5],mac_addr[6],mac_addr[7]);
    // snprintf(act_addr, sizeof(act_addr), "%02d", mac_addr);
    print_mcd("%s", act_addr);
//func2
    for(i = 0; i < 8; i++){
        offset+=snprintf(act_addr1+offset,sizeof(act_addr1), "%02d",mac_addr[i]);  // 格式化的数据写入字符串
    }
    // act_addr1[offset]= '\0';
    print_mcd("%s",act_addr1);

//func3
    for(i = 0; i < 8; i++){
        offset1+=sprintf(act_addr2+offset1, "%02d",mac_addr[i]);  // 格式化的数据写入字符串
    }
    act_addr2[offset]= '\0';
    print_mcd("%s",act_addr2);
}

// my func
static int _test_macaddr_string_to_macaddr_hex()
{
    char *mac_addr = "11164F0E102D0100";
    size_t i = 0;
    uint8_t data1, data2;
    uint8_t  val;
    
    // print_mcd("%d", strlen(mac_addr));
    for(i = 0; i < strlen(mac_addr); ){
        data1 = mac_addr[i];
        data2 = mac_addr[i + 1];
        switch(data1){
        case '0'...'9': 
            val = (data1 - '0');   
            break;
        case 'a'...'f':
            val = (data1 - 'a') + 0x0A; 
            break;
        case 'A'...'F':
            val = (data1 - 'A') + 0x0A; 
            break;
        default:
            return -1;
            // break;
        }
        switch(data2){
        case '0'...'9': 
            val = (val << 4) | (data2 - '0');   
            break;
        case 'a'...'f':
            val = val << 4 | (data2 - 'a') + 0x0A; 
            break;
        case 'A'...'F':
            val = val << 4 | (data2 - 'A') + 0x0A; 
            break;
        default:
            return -1;
            // break;
        }
        // data = (mac_addr[i] - '0') * 16 + (mac_addr[i+1] - '0');
        printf("%02X", val);
        i += 2;
    }
    return 0;

}

static void _test_index(int index)
{
    if(index < 0 )
        index = 0;
}

typedef enum {
    MSG_TEST = 0,
    MSG_TEST1 = 2,
}msg_test_e;

static int _test_argv(void *arg)
{
    // uint8_t *num = (uint8_t *)arg;

    // num = (uint8_t *)arg;
    print_mcd("num = %d", (uint8_t *)arg);
    int num  = 100;
    return num;
}


int main(int argc, char const *argv[])
{
    unsigned int temp = 0xFFFF;
    unsigned int ret = 0;
    char buff[5] = {0};
    unsigned short crc = 0x000;
    char flag = 0x0A;
    int len = 0;
    int i = 0;
    uint8_t num1 = MSG_TEST1;
    int  num2;


    num2 = _test_argv(MSG_TEST1);

    print_mcd("num2 = %d", num2);

    return 0;
    
    #if 0
    crc = ((crc & 0x000F) | (flag & 0x0F)) << 12;

    print_mcd("crc = %x", crc);

    buff[len++] = 0x01;
    buff[len++] = 0x02;
    print_mcd("len  = %d", len);
    for (i = 0; i < len; i++) {
        print_mcd("buff  = 0x%02x", buff[i]);
    }
    
    eide_rs485_get_module_info();

    char tmp_buf2[27] = {0x1D, 0x00, 0x01, 0x00, 0x00, 0x00, 0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78, 0x89, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x01, 0x00, 0x00, 0x02};
    print_mcd("0x%x", do_crc16_XMODEM(tmp_buf2, sizeof(tmp_buf2)));
    #endif
    // _test_macaddr_string_to_macaddr_hex();
    // _test_macaddr_to_decstring();
#if 0
    eide_rs485_payload_t payload;
    memset(&payload, 0, sizeof(payload));
    eide_rs485_payload_t *test = &payload;
    bool ret1 = false;
    print_mcd("%d", ret = false ? false : true);

    
    eide_rs485_payload_t *test_len = (eide_rs485_payload_t *)calloc(1, sizeof(eide_rs485_payload_t));
    print_mcd("%p, %d", test_len, malloc_usable_size(test_len));
    // eide_rs485_payload_t *test_len = NULL;
    int num = 0;
    switch (num){
        case 1: {
            if(1) {
                // MY_RELLOC_REQUIRE(test_len, 1, eide_rs485_payload_t, Error);
                MY_RELLOC_REQUIRE(test_len, 10, eide_rs485_payload_t, Error);
                print_mcd("%p, %d", test_len, malloc_usable_size(test_len));
                print_mcd("ok");
                // memcpy()
            }
            else {
                print_mcd("0");
            }
        }
        break;
        case 2: 
        break;
        case 3: 
        break;
    }
    if(1) {
        MY_RELLOC_REQUIRE(test_len, 1, eide_rs485_payload_t, Error);
        MY_RELLOC_REQUIRE(test_len, 10, eide_rs485_payload_t, Error);
        print_mcd("%p, %d", test_len, malloc_usable_size(test_len));
        print_mcd("ok");
    }
    else {
        print_mcd("0");
    }
    // REQUIRE(!test_len, Error);
    return 0;
Error:
    print_mcd("error");
#endif
    // for (i = 0; i < 10; i++){
    //     test_len[i].data_code = i;
    //     test_len[i].data[0] = i;
    // }

    // for (i = 0; i < 10; i++){
    //     print_mcd("%d, %d", test_len[i].data_code, test_len[i].data[0]);
    // }
    
    // print_mcd("%d", sizeof(eide_rs485_payload_t));
    // PRINT_SIZEOF(eide_rs485_payload_t *);
    
    // char *data = test->data;

    // *data = 0x71;
    // data++;
    // *data = 0x11;

    // print_mcd("0x%x", payload.data[0]);
    // print_mcd("0x%x", payload.data[1]);
    // print_mcd("0x%x", payload.data[2]);

    // test_ctrl_devs();


#if 0
    eide_rs485_ctrl_t ctrl_test;
    char data[EIDE_RS485_MAX_DATA_SIZE] = {0};
    char data1[EIDE_RS485_MAX_DATA_SIZE] = {0};
    memset(&ctrl_test, 0, sizeof(eide_rs485_ctrl_t));
    ctrl_test.ctrl_dev_count = 1;
    ctrl_test.device_id = 0x01;
    ctrl_test.dev_func_num = 1;
    ctrl_test.func_id = 0x02;
    ctrl_test.data_type = 0x03;
    ctrl_test.data = calloc(1, sizeof(unsigned short));
    unsigned short v = 0x0264;
    my_itoa(ctrl_test.data, v, sizeof(unsigned short));

    // ctrl_test.data= v;
    // memcpy(ctrl_test.data, &v, sizeof(char));
    // snprintf(data1, sizeof(int), "%x", v);
    // my_itoa(data1, v, 2);
    // ctrl_test.data = itoa(v);
    // itoa(v, ctrl_test.data, 2);
    memcpy(data, &ctrl_test, 5);
    // printf("ctrl_test data: %d\n", *ctrl_test.data);
    // printf("ctrl_test data: %d\n", *(++ctrl_test.data));

    memcpy(data+5, ctrl_test.data, 2);
    for ( i = 0; i < sizeof(eide_rs485_ctrl_t); i++)
    {
        printf("0x%02x ", data[i]);
    }
    printf("\n");

    char tmp_buf3[27+ 7] = {0x00, 0x00, 0x02, 0x0C, 0x44, 0x30, 0x36, 0x31, 0x30, 0x5F, 0x4D, 0x6F, 0x64, 0x75, 0x6C, 0x65, 0x00, 0xC9, 0x02, 0x07, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x06, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x88, 0x01};
    char *data2 = (char *)tmp_buf3;
    data2 = data2 + 3;
    char len1 = *data2;
    printf("data 10 = 0x%02x\n", *data2);
    char name[64] = {0};
    // memset(name, 0, len1);
    snprintf(name, len1+ 1, "%s",data2+1);
    printf("name = %s\n", name);
    data2 = data2 + len1;
    unsigned short module_type = 0;
    char h = *(data2 + 1);
    char l = *(data2 + 2);
    printf("h = 0x%02x, l = 0x%02x\n", h, l);;
    module_type = ((module_type | *(data2 + 1)) << 8) | (*(data2 + 2) & 0x00ff);
    printf("module_type = 0x%04x, %d\n", module_type, module_type);
    data2 = data2 + 2;
    char type_number = *(data2 + 1);
    data2 += 1;
    char dev_num;
    int j = 0;
    printf("type number = 0x%02x\n", type_number);
    for( i = 0; i < type_number; i++)
    {
        data2 += 1;
        dev_num = *data2;
        printf("dev number = %d\n", dev_num);
        for ( j = 0; j < dev_num; j++)
        {
            data2 += 1;
            printf("dev id = 0x%02x\n", *data2);
        }
    // printf("type num = 0x%02x\n", *(data2 + 1));
        
    }
    // data2 = data2 + 1;
    // memcpy(name, data2 + 1, len1);
    
    // name[len1] =0;
#endif

#if 0
    buff[0] = 0x56;
    buff[1] = 0x04;
    buff[2] = 0x02;
    buff[3] = 0x01;
    buff[4] = 0x00;
    char tmp_buff[41] = {0x2B, 0x00, 0x18, 0x00, 0x00, 0x00, 0x11, 0x16, 0x4F, 0x0E, 0x10, 0x2D, 0x01, 0x00, 0x1E, 0x00, 0x26, 0x00, 0x3E, 0x7F, 0x62, 0x69, 0x00, 0x05, 0x00, 0x03, 0x01, 0x01, 0x02, 0x03, 0x00, 0x07, 0x01, 0x02, 0x03, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00};
    char tmp_buf1[36] = {0x26, 0x00 ,0x19 ,0x00 ,0x00 ,0x00 ,0x11 ,0x16 ,0x4F ,0x0E ,0x10 ,0x2D ,0x01 ,0x00 ,0x1E ,0x00 ,0x26 ,0x00 ,0x3E ,0x7F ,0x62 ,0x69 ,0x00 ,0x05 ,0x00 ,0x02 ,0x01 ,0x01 ,0x02 ,0x03 ,0x64 ,0x01 ,0x01 ,0x01 ,0x01 ,0x01};
    char tmp_buf2[27] = {0x1D ,0x00 ,0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x01 ,0x00 ,0x10 ,0x03 };

    // crc = (buff[0] << 8) & buff[1];
    crc = (( buff[0]) << 8) | buff[1];
    print_mcd("crc = %x",crc);

    // for(int i = 0; i < 5; i++)
    // {
    //     temp = buff[i] ^ temp;
    //     for(int j = 0; j < 8; j++)
    //     {
    //         if(temp & 0x01)
    //         {
    //             temp = temp >> 1;
    //             temp = temp ^ 0xa001;
    //         }
    //         elsec
    //             temp = temp >> 1;
    //     }
    // }

    // print_mcd("0x%x", temp);
    // ret = (temp >> 8) & 0x00ff;
    // ret = ret | ((temp << 8) & 0xff00);
    print_mcd("0x%x", do_crc16_MODBUS(buff, 5));
    print_mcd("0x%x", CRC16_XMODEM(tmp_buff, 41));
    print_mcd("0x%x", do_crc16_CCITT(tmp_buff, 41));
    print_mcd("0x%x", do_crc16_XMODEM(tmp_buff, 41));
    print_mcd("0x%x", do_crc(tmp_buf1, sizeof(tmp_buf1)));
    print_mcd("0x%x", do_crc(tmp_buf2, sizeof(tmp_buf2)));

    char buf[5][12] = {
        "test1",
        "test2",
        "test3",
        "test4",
        "test5",
    };
    char buf1[128] = {0};
    for (size_t i = 0; i < 5; i++) {
        strcat(buf1, buf[i]);
        strcat(buf1, "\t");
        // print_mcd("%s", buf[i]);
    }

    print_mcd("%s", buf1);
#endif
    return 0;
}
