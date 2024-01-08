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
 * @Date         : 2021-06-29 10:38:35
 * @LastEditors  : MCD
 * @LastEditTime : 2023-04-21 16:07:10
 * @FilePath     : /My_C_Test/point_test_2/point.c
 * @Description  : 
 * 
 * ******************************************
 */

#include "common.h"
#include <fcntl.h>
#include <malloc.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

//***************Register_Table {Register Address, Register Data}***********************/
struct REG_TAB {
    unsigned char Address;
    unsigned char Data;
};

//***initial step: 1.Download m_reg_tab_initialization script; 2. Delay 10ms; 3.Download m_reg_tab script********//
const struct REG_TAB m_reg_tab_initialization[] =
    {
        //  initialization script
        {0x00, 0x00},
        {0x04, 0x00},
        {0xfc, 0x86},
        {0xfd, 0x25},
        {0xfe, 0x15},
        {0x00, 0x01},
        {0x02, 0x20},
        {0x00, 0x00},
        {0x0F, 0xD1},
        {0x00, 0x00},
        {0x10, 0xD1},
        {0x00, 0x00},
        {0x00, 0x00},
        {0x00, 0x00},

};

static int acm8625_write_regs(struct device *dev, uint16_t data_length, const struct REG_TAB *reg_address)
{
    int i;
    int ret = 0;
    const struct REG_TAB *reg_access_ptr = reg_address;

    for (i = 0; i < data_length; i++) {
        printf("%d, add = 0x%02x, data = 0x%02x\n", i, reg_access_ptr->Address, reg_access_ptr->Data);
        reg_access_ptr++;
    }
    return 1;
}

static char *test_malloc(void)
{
    char *pointer = (char *)malloc(sizeof(char) * 10);
    if (pointer == NULL)
        return NULL;
    return pointer;
}

static int creat_test(char **test)
{
    *test = test_malloc();
    return 1;
}

// static int test_pointer(char *pointer){
//     char *test;
//     creat_test(&test);
//     return 1;
// }

static int test_del(char *test)
{
    if (test != NULL)
        free(test);
    return 1;
}

static void delete_test(char **test)
{
    if (*test) {
        test_del(*test);
        *test = NULL;
    }
}
int main(int argc, char const *argv[])
{
    // char *test = NULL;

    // creat_test(&test);

    // if (test != NULL) {
    //     printf("test pointer!\n");
    // }
    // else
    //     printf("test failed\n");

    // delete_test(&test);
    // if (test != NULL)
    //     delete_test(test);
    // delete_test(&test);
}

#if 0
int main(int argc, char const *argv[])
{
    char *book_table[] = {
        "old man", "new man", "test book", "tunshi book",
        "xiaogou", "dadao book", "hundun"
    };

    char **english_book[4];
    char **best_book[3];
#if 0
    best_book[0] = &book_table[0];
    best_book[1] = &book_table[2];
    best_book[2] = &book_table[5];

    english_book[0] = &book_table[0];
    english_book[1] = &book_table[1];
    english_book[2] = &book_table[5];
    english_book[3] = &book_table[6];

    print_mcd("%p, %p, %p", &book_table[0], &(*best_book[0]), &(*english_book[0]));

    int i = 0;
    uint8_t a = 1;
    for (i = 0; i < 10; i++) {
        a = ~a & 0x01;
        printf("%d\n", a);
    }
    char tmp[] = "DUI.SmartHome.Device.TurnOff";
    char tmp2[] = "DUI.SmartHome.";

    printf("%d\n", strncmp(tmp, tmp2, strlen("DUI.SmartHome.")));

    int a = atoi("0");
    print_mcd("a = %d", a);
#endif
    // int array[5] = {100,10,2, 3,4};
    // int *ptr = array;
    // int *p1 = &array[0];
    // int *p2 = &array[2];

    // printf("%p\n", &array[0]);
    // printf("%p\n", &array[1]);
    // printf("%d, %d\n", *ptr, *ptr++);
//    printf("%p, %p\n", ptr, ptr + 1);
//    printf("%p, %p\n", &array[0], &array[4]);
    // printf("%p, %p, %ld\n",p2, p1, p2 - p1);
    // acm8625_write_regs(NULL, (sizeof(m_reg_tab_initialization)/2), &m_reg_tab_initialization[0]);
    // uint8_t ctrl_data[1024] = {0};
    // uint8_t id_len = 0;
    // size_t cur_len = 0;
    // uint8_t *cdata =  (uint8_t *)ctrl_data;

    // *cdata = 100;
    // cdata++;
    // memcpy(cdata, "0293499336", 10);
    // cdata += 10;

    // for(size_t i = 0; i < 11; i ++) {
    //     printf("%02x ", cdata[i]);
    // }
    // printf("\n");

    // printf("cdata size = %d\n", cdata - &ctrl_data[0]);

    // printf("0: %02x, 1: %02x\n", BREAK_UINT32(0x19aadc83, 0), BREAK_UINT32(0x19aadc83, 1));

    // char *test = malloc(10);
    // free(test);
    // // free(test);
    // // memcpy(test, "astrejadfisaee", 100);
    // free(test);

    return 0;
}
#endif
