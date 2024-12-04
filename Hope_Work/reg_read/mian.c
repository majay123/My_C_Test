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
 * @LastEditTime : 2024-11-04 17:30:58
 * @FilePath     : /My_C_Test/Hope_Work/reg_read/mian.c
 * @Description  : 
 * 
 * ******************************************
 */

#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>

#define BASE_ADDR 0xB02B0000      // DAC基地址
#define ANACTL0_OFFSET 0x1C       // DAC_ANACTL0寄存器的偏移

int main() {
    int fd;
    volatile uint32_t *reg_base;
    uint32_t reg_value;

    // 打开/dev/mem
    fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0) {
        perror("Cannot open /dev/mem");
        return 1;
    }

    // 映射寄存器地址
    reg_base = (uint32_t *)mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, BASE_ADDR);
    if (reg_base == MAP_FAILED) {
        perror("mmap failed");
        close(fd);
        return 1;
    }

    // 读取DAC_ANACTL0寄存器的值
    reg_value = *(reg_base + (ANACTL0_OFFSET / sizeof(uint32_t)));
    printf("DAC_ANACTL0 Register value: 0x%X\n", reg_value);

    // 解除映射
    munmap((void *)reg_base, 0x1000);
    close(fd);
    return 0;
}