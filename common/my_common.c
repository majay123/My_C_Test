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
 * @Date         : 2021-09-16 08:58:59
 * @LastEditors  : MCD
 * @LastEditTime : 2021-09-16 09:01:04
 * @FilePath     : /My_C_Test/common/my_common.c
 * @Description  : 
 * 
 * ******************************************
 */

#include "my_common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*********************************************************************
* MACROS
*/

/*********************************************************************
* GLOBAL VARIABLES
*/

/*
*CRC 余式表
*/
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
    0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};

/*********************************************************************
* LOCAL FUNCTIONS
*/

/*********************************************************************
 * @fn      setTimer

 * @brief
 *   This function is called to start a timer to expire in n mSecs then call callback func.
 *
 * @param   timeout_value - in milliseconds.
 * @param   repeat - true for repeat when timer expires,false for expiration once.
 * @param   func - callback function
 * @param   *param - passed callback function
 *
 * @return  timer handle, or NULL failure.
 */
timer_t setTimer(size_t timeout_value, unsigned int repeat, _function func, void *param)
{
    struct sigevent sevp;
    struct itimerspec it;
    timer_t timer_id = 0;
    time_t tv_sec;
    long tv_nsec;

    if (timeout_value == 0) {
        return NULL;
    }

    memset(&sevp, 0, sizeof(sevp));
    memset(&it, 0, sizeof(it));
    sevp.sigev_value.sival_ptr = param; //param pass to func
    sevp.sigev_notify = SIGEV_THREAD;
    sevp.sigev_notify_function = func;
    if (timer_create(CLOCK_MONOTONIC, &sevp, &timer_id) < 0) {
        fprintf(stderr, "timer_create errno %d,%s\n", errno, strerror(errno));
        return NULL;
    }

    tv_sec = timeout_value / 1000;                  // seconds
    tv_nsec = (timeout_value % 1000) * 1000 * 1000; // milliseconds to Nanoseconds

    if (repeat != 0) {
        /* interval */
        it.it_interval.tv_sec = tv_sec;
        it.it_interval.tv_nsec = tv_nsec; //change milliseconds to Nanoseconds
    }

    /* first expiration */
    it.it_value.tv_sec = tv_sec;
    it.it_value.tv_nsec = tv_nsec;

    if (timer_settime(timer_id, 0, &it, NULL) < 0) {
        timer_delete(timer_id);
        fprintf(stderr, "timer_settime errno %d,%s\n", errno, strerror(errno));
        return NULL;
    }

    return timer_id;
}

/*********************************************************************
 * @fn      killTimer

 * @brief
 *
 * @param   timer_id return by setTimer
 *
 * @return  0 success, or -1 failure.
 */
int killTimer(timer_t timer_id)
{
    if (timer_id == NULL) {
        fprintf(stderr, "error -1,timer_id 0x%X not exist\n", timer_id);
        return -1;
    }
    if (timer_delete(timer_id) < 0) {
        fprintf(stderr, "timer_delete errno %d,%s\n", errno, strerror(errno));
        return -1;
    }

    return 0;
}

void select_sleep(int seconds)
{
    struct timeval timeout;
    timeout.tv_sec = seconds;
    timeout.tv_usec = 0;
    select(0, NULL, NULL, NULL, &timeout);
}

void select_usleep(int micro_seconds)
{
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = micro_seconds;
    select(0, NULL, NULL, NULL, &timeout);
}

char *strlwr(char *s)
{
    char *tmp = s;

    for (; *tmp; ++tmp) {
        *tmp = tolower((unsigned char)*tmp);
    }

    return s;
}

char *strupr(char *s)
{
    char *tmp = s;

    for (; *tmp; ++tmp) {
        *tmp = toupper((unsigned char)*tmp);
    }

    return s;
}

int hexStr2bytes(const uint8_t *hexStr, uint8_t *buf, int bufLen)
{
    int i;
    int len;

    if (NULL == hexStr) {
        len = 0;
    } else {
        len = (int)strlen(hexStr) / 2;

        if (bufLen < len) {
            len = bufLen;
        }
    }

    for (i = 0; i < len; i++) {
        char ch1, ch2;
        int val;

        ch1 = hexStr[i * 2];
        ch2 = hexStr[i * 2 + 1];
        if (ch1 >= '0' && ch1 <= '9') {
            val = (ch1 - '0') * 16;
        } else if (ch1 >= 'a' && ch1 <= 'f') {
            val = ((ch1 - 'a') + 10) * 16;
        } else if (ch1 >= 'A' && ch1 <= 'F') {
            val = ((ch1 - 'A') + 10) * 16;
        } else {
            return -1;
        }

        if (ch2 >= '0' && ch2 <= '9') {
            val += ch2 - '0';
        } else if (ch2 >= 'a' && ch2 <= 'f') {
            val += (ch2 - 'a') + 10;
        } else if (ch2 >= 'A' && ch2 <= 'F') {
            val += (ch2 - 'A') + 10;
        } else {
            return -1;
        }

        buf[i] = val & 0xff;
    }

    return 0;
}


static int hexStr2bytes1(char *input, char *ret, int length)
{
    int retlen = 0;
    if(!input || !length) goto End;

    size_t ilen = strlen(input);
    if(ilen > 100 ||  ilen == 0) 
        goto End;
    // retlen = ilen >> 1;
    
    char *p = input;
    unsigned char *r = ret;
    unsigned char flag = 1;
    while(*p) {
        unsigned char val = 0;
        switch (*p)
        {
        case '0'...'9': {
            val = *p - '0';
            break;
        }
        case 'A'...'Z': {
            val = *p - 'A' + 0x0a;
            break;
        }
        case 'a'...'z': {
            val = *p - 'a' + 0x0a;
            break;
        }
        default:
            goto End_of_while;
        }
        if(flag) {
            *r = val;
        } else {
            *r = (*r << 4) | val;
            ++r;
            ++retlen;
        }
        if(length < (char *)r - ret) {
            return length;
        }
        flag = !flag;
        End_of_while:
        ++p;
    }
    if(!flag) {
        ++retlen;
        *r <<= 4;
    }
    End:
        return retlen;
}

int bytes2hexStr(const uint8_t *buf, char *hexStr, int bufLen)
{
    int i;

    for (i = 0; i < bufLen; i++) {
        snprintf(hexStr + 2 * i, 3, "%02x", buf[i]);
    }
    *(char *)(hexStr + 2 * i + 1) = '\0';

    return i;
}

/**
* @author  		MCD
* @date  		2021-08-24-11:07
* @details		CRC crc16_ccitt函数
*/
unsigned short crc16_ccitt_tab(unsigned char *pbuf, size_t len)
{
    unsigned short crc = CRC16_CCITT_SEED;

    while (len-- > 0) {
        crc = crc16_ccitt_table[(crc >> 8 ^ *pbuf++) & 0xff] ^ (crc << 8);
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

int my_itoa(char *buf, int value, char size)
{
    char *p = buf;
    int  i  = 1;

    if(buf == NULL || size <= 0) return -1;
    for ( i = 1; i <= size; i++)
    {
        *p = value >> (8 * (size - i));
        p++;
    }
    return 0;
}