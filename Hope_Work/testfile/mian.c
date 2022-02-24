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
 * ************Copyright 2022 MCD************
 * 
 * @version      : 
 * @Company      : HOPE
 * @Author       : MCD
 * @Date         : 2022-02-21 10:01:18
 * @LastEditors  : MCD
 * @LastEditTime : 2022-02-21 10:29:34
 * @FilePath     : /My_C_Test/Hope_Work/testfile/mian.c
 * @Description  : 
 * 
 * ******************************************
 */
#define _FILE_OFFSET_BITS 64

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
    char data[1024] = "";
	long long a = 0;
	int fd = open("/mnt/card/test.txt", O_CREAT | O_RDWR, 0776);
    
	memset(data, 0, sizeof(data));
	if(fd > 0) {
        printf("start write file\n");
		while(1) {
            if (fd > 0 && write(fd, data, sizeof(data)) != sizeof(data))
                printf("#func =%s, line ==%d fwrite err \n",__FILE__,__LINE__);
			// write(fd, data, sizeof(data));
			a++;
			if(a >= 1024*1024*6)
				break;
		}
	}

	close(fd);
	printf("write over");
}