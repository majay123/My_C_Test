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
 * @Date         : 2021-10-13 09:17:27
 * @LastEditors  : MCD
 * @LastEditTime : 2022-11-14 09:48:08
 * @FilePath     : /My_C_Test/simple_test/main.c
 * @Description  : 
 * 
 * ******************************************
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// #include "common.h"

#define HI_UINT16(a) (((a) >> 8) & 0xFF)
#define LO_UINT16(a) ((a)&0xFF)

static uint32_t _hex_to_uint(uint8_t *hex, size_t len)
{
    // uint8_t time[4] = {0x00, 0xff,0xe0, 0x19};
    // char *end;
    uint32_t running_time = 0;
    for (size_t i = 0; i < len; i++) {
        running_time = running_time << 8;
        running_time = running_time | hex[i];
        //
    }
    // printf("Running time: %x, %ld\n", running_time, running_time);
    return running_time;
}

static void _uint_to_hex(uint32_t num, uint8_t *data, size_t len)
{
    int i = len - 1;
    for (; i >= 0; i--) {
        data[i] = (uint8_t)((num >> (i * 8)) & 0xFF);
        printf("(%ld)%02x ", i, data[i]);
    }
    printf("\n");
}

void hextodecstring(uint8_t *src_data, size_t len, uint8_t *dest_data)
{
    size_t offset = 0;
    int i = 0;

    for (i = 0; i < len; i++) {
        offset += snprintf(dest_data + offset, sizeof(dest_data), "%02d", src_data[i]);  // 格式化的数据写入字符串
    }
}

int my_itoa(uint8_t *buf, uint32_t value, size_t size)
{
    uint8_t *p = buf;
    int i = 1;

    if (buf == NULL || size <= 0)
        return -1;
    for (i = 1; i <= size; i++) {
        *p = value >> (8 * (size - i));
        p++;
    }
    return 0;
}

void cancelchar(char *a, char n)
{
    char *p;
    for (p = a; *p != '\0'; p++)
        if (*p != n)
            *a++ = *p;
    *a = '\0';
}

void del_sp(char *src, char n)  // 删除C风格字符串中的空格
{
    char *fp = src;
    while (*src) {
        if (*src != n) {  // 如果不是空格就复制
            *fp = *src;
            fp++;
        }
        src++;
    }
    *fp = '\0';  //封闭字符串
}

void split(char *str, char *delims)
{
    char *result = NULL;
    result = strtok(str, delims);
    while (result != NULL) {
        printf("result is \"%s\", %d\n", result, atoi(result));
        result = strtok(NULL, delims);
    }
}

int hexStr2bytes(const uint8_t *hexStr, uint8_t *buf, int bufLen)
{
    int i;
    int len;

    if (NULL == hexStr) {
        len = 0;
    }
    else {
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
        }
        else if (ch1 >= 'a' && ch1 <= 'f') {
            val = ((ch1 - 'a') + 10) * 16;
        }
        else if (ch1 >= 'A' && ch1 <= 'F') {
            val = ((ch1 - 'A') + 10) * 16;
        }
        else {
            return -1;
        }

        if (ch2 >= '0' && ch2 <= '9') {
            val += ch2 - '0';
        }
        else if (ch2 >= 'a' && ch2 <= 'f') {
            val += (ch2 - 'a') + 10;
        }
        else if (ch2 >= 'A' && ch2 <= 'F') {
            val += (ch2 - 'A') + 10;
        }
        else {
            return -1;
        }

        buf[i] = val & 0xff;
    }

    return 0;
}

int get_addr(char *src_addr, char *dst_addr, size_t len)
{
    del_sp(src_addr, ':');
    hexStr2bytes(src_addr, dst_addr, len);
}

static int _check_IPaddr_correct(const char *ip)

{
    int dots = 0;     //字符 . 的个数
    int setions = 0;  //ip每一部分总和（0-255）
    int strnum = 0;   //字符串长度记录

    if (NULL == ip || *ip == '.')  //排除输入参数为NULL, 或者第一个字符为'.'的字符串
    {
        return -1;  //格式错误
    }

    while (*ip)  //字符串不为空
    {
        if (*ip == '.')  //如果当前字符为点.则开始判断上一段是否合法在0~255之间
        {
            dots++;                              //字符 . 的个加一
            if (setions >= 0 && setions <= 255)  //检查ip是否合法
            {
                setions = 0;  //对上一段IP总和进行清零
            }
            else {
                printf("error type 1\n");
                return -1;  //格式错误，某一段数值不在0~255之间
            }
        }
        else if (*ip >= '0' && *ip <= '9')  //判断是不是数字
        {
            setions = setions * 10 + (*ip - '0');  //求每一段总和
        }
        else {
            printf("error type 2\n");
            return -1;  //该字符既不是数字也不是点
        }
        ip++;               //指向下一个字符
        if (++strnum > 15)  //3*4+3=15,IP地址字符串非结束符字符数最多15个
        {
            printf("error type 3\n");
            return -1;  //防止出现“1234567890987654321.”这种前面全是数字的字符串输入进来。
        }
    }
    /*判断IP最后一段是否合法*/
    if (setions >= 0 && setions <= 255) {
        if (dots == 3)  //防止出现：“192.168.123”这种不是三个点的情况
        {
            return 0;  //IP地址合法正确
        }
    }
    printf("error type 4\n");
    return -1;
}

typedef struct {
    uint8_t addr[2];
    uint8_t len;
    uint8_t data;
    uint8_t key;
} test_def;

typedef struct ms_universal_state_t {
    uint8_t addr_out;
    uint8_t addr_in;
    uint8_t onoff_status;
    uint8_t temper;
    uint8_t mode;
} ms_universal_state_t;

typedef struct ms_ac_info_t {
    ms_universal_state_t universal_state;
    uint8_t speed;
    uint8_t room_temper;
    uint8_t fault_code;
    uint8_t reserve;
    uint8_t other_info;
} ms_ac_info_t;

typedef struct ms_rs485_cmd_data_t {
    uint8_t ctrl;
    uint8_t dev_num;
    uint8_t dev_addr[32 * 2];
} ms_rs485_cmd_data_t;

typedef struct ms_rs485_send_msg_t {
    uint8_t addr;
    uint8_t cmd;
    ms_rs485_cmd_data_t data;
    uint8_t check;
} ms_rs485_send_msg_t;

uint8_t sum_check(const uint8_t *data, int len)
{
    int i;
    uint8_t sum = 0;

    for (i = 0; i < len; i++) {
        sum += data[i];
    }

    return sum;
}

ssize_t maisi_rs485_get_dev_status(uint8_t gaddr, uint8_t cmd, uint8_t dev_num, uint8_t *daddr)
{
    uint16_t length = 0;
    ms_rs485_send_msg_t msg;
    uint8_t *send_data = (uint8_t *)&msg;
    uint8_t chk = 0;

    msg.addr = gaddr;
    msg.cmd = cmd;
    msg.data.dev_num = dev_num;

    if (dev_num == 1)
        msg.data.ctrl = 0x01;
    else if (dev_num > 1)
        msg.data.ctrl = 0x0F;
    else
        return 0;

    memcpy((uint8_t *)msg.data.dev_addr, daddr, dev_num * 2);

    length = dev_num * 2 + 4;
    chk = sum_check(send_data, length);
    send_data[length] = chk;

    for (int i = 0; i < (length + 1); i++) {
        printf("0x%02x ", send_data[i]);
    }
    printf("\n");
}

void testp(char *onoff)
{
    char on = 1;

    *onoff = on;
}

int main(int argc, char const *argv[])
{
    uint8_t time1[4] = {0x00, 0x01, 0xe0, 0x19};
    uint8_t data[4] = {0};
    uint8_t data1[4] = {0};
    char test1[20] = "00:E0:99:13:C7:62";
    uint8_t buf[20] = {0};
    _Bool test = 0;
    uint8_t data2[10] = {0x03, 0x44, 0x21, 0xdd, 0x1d, 0xa1, 0x4c, 0x4d, 0x1f, 0x9a};
    // char *end;
    uint32_t running_time = 10 * 1000;
    char *text = "Res_test";

    strdup(text "ok");

    // char ip[20] = "192.168.2.110";
    // char *test11 = strrchr(ip, '.');
    // char ip2[20] = {0};

    // printf("Running %s, %d\n", test11, (test11 - (char *)ip) + 1);
    // printf("%p, %p\n", test11, (char *)ip);
    // memcpy(ip2, ip, (test11 - (char *)ip) + 1);
    // printf("%s\n", ip2);

    // int ret = -1;
    // ret = _check_IPaddr_correct(ip);
    // printf("ret = %d\n", ret);

    // char *onoff = (char *)malloc(sizeof(char));
    // char on = 1;

    // testp(onoff);
    // printf("onoff = %d\n", *onoff);

    // ms_ac_info_t *ac_info = (ms_ac_info_t *)data2;

    // printf("0x%02x, 0x%02x, 0x%02x, 0x%02x\n", ac_info->universal_state.addr_in, ac_info->universal_state.addr_out, ac_info->fault_code, ac_info->reserve);
    // ms_ac_info_t *ac_info = (ms_ac_info_t *)calloc(10, sizeof(ms_ac_info_t));
    // for (int i = 0; i < 10; i++)
    //     printf("ac_info [%d] addr = %p\n", i, &ac_info[i]);
    // test_def test_def;
    // memset(&test_def, 0, sizeof(test_def));
    // printf("%d\n",sizeof(test_def));
    // printf("%p, %p %p \n",  &test_def.addr[0], &test_def.addr[1],&test_def.len);
    // // printf("%p\n",  &test_def);

    // printf("%p\n",  (char *)&test_def+2);
#if 0
    uint8_t *ac_addrs = NULL;
    uint8_t ac_num = 0;
    uint16_t channelid = 0x0123;
    for (int i = 0; i < 5; i++) {
        printf("1 count = %d\n", ac_num);
        if (!ac_addrs) {
            ac_addrs = (uint8_t *)calloc(2, sizeof(uint8_t));
            printf("1 ac_addrs = %p\n", ac_addrs);
        }
        else {
            printf("2 ac_addrs = %p\n", ac_addrs);
            ac_addrs = (uint8_t *)realloc(ac_addrs, 2 * (ac_num + 1));
            printf("3 ac_addrs = %p\n", ac_addrs);
        }
        // TEST_ASSERT_NOT_NULL(ac_addrs);
        ac_addrs[i * 2] = HI_UINT16(channelid);
        ac_addrs[i * 2 + 1] = LO_UINT16(channelid);

        ac_num++;
        channelid += 0x3;
    }
    printf("2 count = %d\n", ac_num);
    for (int i = 0; i < ac_num; i++) {
        printf("0x%02x, 0x%02x\n", ac_addrs[i * 2], ac_addrs[i * 2 + 1]);
    }
// #endif
//     uint8_t addrs[10] = {0x01, 0x21, 0x01, 0x22, 0x01, 0x23, 0x01, 0x24, 0x01, 0x25};
//     maisi_rs485_get_dev_status(0x01, 0x51, 5, addrs);

#if 0
    size_t i = 0;
    srand(time(0)); 
    for(; i < 20; i++)
	printf("%d \n", rand() % (6)) ;
    // char *result = (char *)calloc(100, sizeof(char));
    char *result = NULL;
    if(result) printf("result calloc Ok\n");
    else printf("result calloc failed\n");

    _Bool on = 0;

    printf("on = %d\n", on);
    on = !on;
    printf("on = %d\n", on);
    on = !on;
    printf("on = %d\n", on);
    on = !on;
    printf("on = %d\n", on);
#endif
#if 0
        char *temp_mem = NULL;
    char *temp_mem1 = NULL;
    size_t size = 1;
    char aa[] = "回家模式";
    char aaa[64] = {0};

    if (aaa != NULL)
        printf("aaa = %s\n", aaa);
    temp_mem = malloc(100);
    printf("temp_mem = %p\n", temp_mem);
    temp_mem1 = realloc(temp_mem, size);
    printf("temp_mem = %p, temp_mem1 = %p\n", temp_mem, temp_mem1);
    temp_mem1 = realloc(temp_mem, 0);  //already free temp_mem;
    temp_mem = temp_mem1;
    printf("temp_mem = %p, temp_mem1 = %p\n", temp_mem, temp_mem1);
    // printf("%s\r\n", strstr( aa, "模式"));
    char *aa1 = strstr(aa, "模式");
    memcpy(aaa, aa, aa1 - aa);
    printf("aaa = %s %d\r\n", aaa, aa1 - aa);
#endif
    // printf("%d\r\n", strcspn( aa, "模式"));
    // aaaa = strtok(aa, "模式");
    // printf("get %s\r\n", aaa);
    // if()
    // printf("%d, %p\n", test_def.len, test+ 2);

    // del_sp(test1, ':');
    // printf("%s \n", test1);
    // hexStr2bytes(test1, buf, 8);
    // get_addr(test1, buf, 8);
    // for (size_t i = 0; i < 8; i++)
    // {
    //     printf("%02X ", buf[i]);
    // }
    // printf("\n");

    // time_t t;
    // srand((unsigned) time(&t));
    // printf("%d %d\n", rand() % 256, rand() % 256);

    // running_time = _hex_to_uint(time, sizeof(time));
    // printf("Running time: %d\n", running_time);
    // _uint_to_hex(running_time, data, sizeof(data));
    // printf("Running time: %x, %ld\n", running_time, running_time);
    // my_itoa(data1, running_time, sizeof(data1));
    // printf("%02x %02x %02x %02x\n", data1[0], data1[1], data1[2], data1[3]);
    // uint8_t mac_addr[8] = {0xF0, 0x0A, 0x4F, 0x04, 0x10, 0x2D, 0x01, 0x01};
    // uint8_t dest_addr[25] = {0};
    // hextodecstring(mac_addr, sizeof(mac_addr), dest_addr);
    // printf("%s\n", dest_addr);
    // test = (test == 0 ?  1 : 0);
    // printf("test = %d\n", test);
    // test = (test == 0 ? 1 : 0);
    // printf("test = %d\n", test);
    // test = (test == 0 ? 1 : 0);
    // printf("test = %d\n", test);
    // printf("%d\n",atoi("200"));
    // test ^= test;
    // printf("test = %d\n", test);
    // test ^= test;
    // printf("test = %d\n", test);
    return 0;
}
