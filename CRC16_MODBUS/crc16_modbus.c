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
 * @LastEditTime : 2021-07-23 16:46:14
 * @FilePath     : /MyGithub_Project/My_C_Test/CRC16_MODBUS/crc16_modbus.c
 * @Description  : 
 * 
 * ******************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "common.h"

//
int main(int argc, char const *argv[])
{
    unsigned int temp = 0xFFFF;
    unsigned int ret = 0;
    char buff[5] = {0};

    buff[0] = 0x56;
    buff[1] = 0x04;
    buff[2] = 0x02;
    buff[3] = 0x01;
    buff[4] = 0x00;

    for(int i = 0; i < 5; i++)
    {
        temp = buff[i] ^ temp;
        for(int j = 0; j < 8; j++)
        {
            if(temp & 0x01)
            {
                temp = temp >> 1;
                temp = temp ^ 0xa001;
            }
            else
                temp = temp >> 1;
        }
    }

    print_mcd("0x%x", temp);
    ret = (temp >> 8) & 0x00ff;
    ret = ret | ((temp << 8) & 0xff00);
    print_mcd("0x%02x",ret);

    char buf[5][12]= {
        "test1",
        "test2",
        "test3",
        "test4",
        "test5",
    };
    char buf1[128] = {0};
    for (size_t i = 0; i < 5; i++)
    {
        strcat(buf1,buf[i]);
        strcat(buf1,"\t");
        // print_mcd("%s", buf[i]);
    }
    
    print_mcd("%s", buf1);

    return 0;
}
