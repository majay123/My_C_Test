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
 * ************Copyright 2024 MCD************
 * 
 * @version      : 
 * @Company      : HOPE
 * @Author       : MCD
 * @Date         : 2024-12-16 14:52:38
 * @LastEditors  : MCD
 * @LastEditTime : 2024-12-16 14:52:39
 * @FilePath     : /My_C_Test/Hope_Work/tuya_test/tttt.c
 * @Description  : 
 * 
 * ******************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// 判断字符串是否为合法数字
int is_valid_number(const char* str) {
    // 检查空字符串
    if (str == NULL || *str == '\0') {
        return 0;
    }

    // 允许开头的正负号
    if (*str == '-' || *str == '+') {
        str++;
    }

    // 检查剩余字符是否都是数字
    while (*str) {
        if (!isdigit(*str)) {
            return 0;
        }
        str++;
    }

    return 1;
}

// 比较函数，用于字符串数字的升序排序
int compare_asc(const void* a, const void* b) {
    const char* str_a = *(const char**)a;
    const char* str_b = *(const char**)b;
    
    // 比较字符串长度
    if (strlen(str_a) != strlen(str_b)) {
        return strlen(str_a) - strlen(str_b);
    }
    
    // 长度相同时，逐字符比较
    return strcmp(str_a, str_b);
}

// 排序数字字符串数组的函数
void sort_number_strings(char** arr, int size) {
    // 先验证所有输入是否为数字
    for (int i = 0; i < size; i++) {
        if (!is_valid_number(arr[i])) {
            printf("错误：输入 %s 不是有效的数字\n", arr[i]);
            return;
        }
    }

    // 使用快速排序，传入比较函数
    qsort(arr, size, sizeof(char*), compare_asc);
}

// 打印字符串数组
void print_array(char** arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%s ", arr[i]);
    }
    printf("\n");
}

int main() {
    // 测试数组（包含一个非数字字符串）
    char* numbers[] = {
        "1000", "2", "300", "45", "67", "123", "5678", "89", "abc"
    };
    
    int size = sizeof(numbers) / sizeof(numbers[0]);
    
    printf("排序前：");
    print_array(numbers, size);
    
    // 执行排序
    sort_number_strings(numbers, size);
    
    printf("排序后（从小到大）：");
    print_array(numbers, size);
    
    // 再测试一个只有数字的数组
    char* valid_numbers[] = {
        "1000", "2", "300", "45", "67", "123", "5678", "89"
    };
    
    int valid_size = sizeof(valid_numbers) / sizeof(valid_numbers[0]);
    
    printf("\n仅数字数组：");
    print_array(valid_numbers, valid_size);
    
    // 执行排序
    sort_number_strings(valid_numbers, valid_size);
    
    printf("排序后（从小到大）：");
    print_array(valid_numbers, valid_size);
    
    return 0;
}