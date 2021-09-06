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
 * @Date         : 2021-08-24 13:33:36
 * @LastEditors  : MCD
 * @LastEditTime : 2021-08-27 14:50:47
 * @FilePath     : /My_C_Test/CRC16_MODBUS/rs485_dispatch.c
 * @Description  : 
 * 
 * ******************************************
 */

#include <string.h>

#include "common.h"
#include "eide_rs485.h"
// #include "queue.h"
#include "rs485_dispatch.h"

// 该位称为预置值，使用人工算法（长除法）时
//需要将除数多项式先与该与职位 异或 ，才能得到最后的除数多项式
#define CRC16_CCITT_SEED            (0x0000)
// 该位为简式书写 实际为0x11021
#define CRC16_CCITT_POLY16          (0x1021)

static unsigned short eide_serial = 0;

// for test
#if 1
char src_mac_addr[8] = {0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78, 0x89};
char broadcast_mac_addr[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
#endif


//CRC-16/XMODEM
unsigned short do_crc16_XMODEM1(unsigned char *ptr, int len)
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

unsigned short crc16_ccitt(unsigned char *puchMsg, unsigned int usDataLen)
{
	unsigned short wCRCin = CRC16_CCITT_SEED;
	unsigned short wCPoly = CRC16_CCITT_POLY16;
	unsigned char wChar = 0;
    int i;
 
	while (usDataLen--) 
	{
		wChar = *(puchMsg++);
		wCRCin ^= (wChar << 8);
		for (i = 0; i < 8; i++) 
		{
			if (wCRCin & 0x8000)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	return (wCRCin);
}

// recv funcs
int eide_rs485_dispatch_msg(char *data, int datalen)
{
}

// static int _eide_rs485_simple_control(llq_t *llq_handle, rs485_data_t *data, int datalen)
// {
//     llq_add(llq_handle, (char *)data, datalen, 1);
// }

// send funcs

/**
* @author  		MCD
* @date  		2021-08-27-10:32
* @details		装载填充 数据头
*/
static int _eide_rs485_head_fill(char version, unsigned short serial, char flag, unsigned short offset, char *src_address, char *dst_address, char encryption, eide_rs485_head_t *header)
{
    unsigned short flag_offset = 0;

    if (header == NULL)
        return -1;

    header->guid[0] = EIDE_RS485_SOF;
    header->guid[1] = EIDE_RS485_SOF;
    header->version = EIDE_RS485_VERSION;
    header->serial_number[0] = eide_serial & 0x00FF;
    header->serial_number[1] = eide_serial >> 8;
    flag_offset = ((flag_offset & 0x000F) | (flag & 0x0F)) << 12;
    flag_offset = (flag_offset & 0xF000) | (offset & 0x0FFF);
    header->flag_offset[0] = flag_offset >> 8;
    header->flag_offset[1] = flag_offset && 0x00FF;
    memcpy(header->src_address, src_mac_addr, EIDE_MODULE_MACADDR_SIZE);
    memcpy(header->dst_address, broadcast_mac_addr, EIDE_MODULE_MACADDR_SIZE);
    header->encryption_type = EIDE_RS485_ENCRYPTION_TYPE;

    return 0;
}

/**
* @author  		MCD
* @date  		2021-08-27-09:03
* @details		配置码:0x01 用于获取设备信息、配置设备信息
*/
static int eide_rs485_configration(char operation_code, unsigned short config_func_code, eide_rs485_payload_t *payload)
{
    int len = 0;

    if (payload == NULL)
        return -1;
    payload->data_code = EIDE_DATACODE_CONFIG;
    payload->data[len++] = operation_code;
    payload->data[len++] = config_func_code >> 8;
    payload->data[len++] = config_func_code & 0x00FF;

    switch (operation_code) {
    case EIDE_CONFIG_READ:
        break;
    case EIDE_CONFIG_WRITE:
        break;
    case EIDE_CONFIG_ADD:
        break;
    case EIDE_CONFIG_DELETE:
        break;
    default:
        break;
    }
    len++;
    return len;
}

/**
* @author  		MCD
* @date  		2021-08-27-12:41
* @details		get module info 查询模块信息 配置功能码
*/
int eide_rs485_get_module_info(void)
{
    eide_rs485_msg_t rs485_msg;
    char payload_len = 0;
    unsigned short crc_checksum = 0;
    int i = 0;

    eide_serial++;
    print_mcd("eide_serial = %d", eide_serial);
    memset(&rs485_msg, 0, sizeof(eide_rs485_msg_t));

    _eide_rs485_head_fill(EIDE_RS485_VERSION, eide_serial, EIDE_RS485_FLAGS, 0x00, src_mac_addr, broadcast_mac_addr, EIDE_RS485_ENCRYPTION_TYPE, &rs485_msg.header);
    
    payload_len = eide_rs485_configration(EIDE_CONFIG_READ, EIDE_CONFIG_CODE_MODULE_DEVICE_INFO, &rs485_msg.payload);
    print_mcd("payload_len = %d", payload_len);

    rs485_msg.header.data_length = payload_len + EIDE_RS485_FIXED_LEN;
    print_mcd("data size = %d", rs485_msg.header.data_length);
    
    int crc_len = EIDE_RS485_HEAD_LEN  + payload_len;
    print_mcd(" crc_len = %d", crc_len);
    crc_checksum = do_crc16_XMODEM1((char *)&rs485_msg + 2, crc_len);
    print_mcd("crc_checksum = %x", crc_checksum);
    
    
    
    rs485_msg.payload.data[payload_len - 1] = crc_checksum >> 8;
    rs485_msg.payload.data[payload_len] = crc_checksum & 0x00FF;

    // char *data = (char *)&rs485_msg;
    // for ( i = 0; i < rs485_msg.header.data_length + 2; i++)
    // {
    //     printf("%d-0x%02x ", i, data[i]);
    // }
    // printf("\n");
    
}
// 27 + 2
// 1D 00 01 00 00 00 12 23 34 45 56 67 78 89 FF FF FF FF FF FF FF FF 00 01 00 00 02 4A BC