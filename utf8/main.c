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
 * @Date         : 2023-07-10 09:55:53
 * @LastEditors  : MCD
 * @LastEditTime : 2023-07-12 13:54:03
 * @FilePath     : /My_C_Test/utf8/main.c
 * @Description  : 
 * 
 * ******************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#include <stdint.h>


void utf8_to_unicode(const char *utf8_str, uint16_t *unicode_arr, int *unicode_len) {
    int len = strlen(utf8_str);
    int i = 0;
    int index = 0;

    while (i < len) {
        uint16_t unicode = 0;
        int bytes = 0;

        if ((utf8_str[i] & 0x80) == 0) {
            // Single-byte UTF-8 encoding
            unicode = utf8_str[i];
            bytes = 1;
        } else if ((utf8_str[i] & 0xE0) == 0xC0) {
            // 2-byte UTF-8 encoding
            unicode = ((utf8_str[i] & 0x1F) << 6) | (utf8_str[i+1] & 0x3F);
            bytes = 2;
        } else if ((utf8_str[i] & 0xF0) == 0xE0) {
            // 3-byte UTF-8 encoding
            unicode = ((utf8_str[i] & 0x0F) << 12) | ((utf8_str[i+1] & 0x3F) << 6) | (utf8_str[i+2] & 0x3F);
            bytes = 3;
        } else if ((utf8_str[i] & 0xF8) == 0xF0) {
            // 4-byte UTF-8 encoding
            unicode = ((utf8_str[i] & 0x07) << 18) | ((utf8_str[i+1] & 0x3F) << 12) | ((utf8_str[i+2] & 0x3F) << 6) | (utf8_str[i+3] & 0x3F);
            bytes = 4;
        } else {
            printf("Error: Invalid UTF-8 encoding\n");
            return;
        }

        unicode_arr[index++] = unicode;
        i += bytes;
    }

    *unicode_len = index;
}


/* 从字符串中删除指定的子字符串 */
char* remove_substring(const char* str, const char* substr) {
    size_t len = strlen(str);
    size_t substr_len = strlen(substr);

    // 创建新的字符串，最大长度为原字符串长度+1
    char* result = (char*)malloc((len + 1) * sizeof(char));
    if (result == NULL) {
        fprintf(stderr, "Error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    size_t i = 0;
    size_t j = 0;

    // 遍历原字符串
    while (i < len) {
        // 如果找到子字符串，则跳过子字符串
        if (strncmp(str + i, substr, substr_len) == 0) {
            i += substr_len;
        } else {
            // 复制非子字符串的字符到新字符串
            result[j++] = str[i++];
        }
    }

    // 添加字符串结束符
    result[j] = '\0';

    return result;
}

// int main() {
//     const char* str = "Hello, World! Hello, OpenAI!";
//     const char* substr = "Hello, ";

//     char* result = remove_substring(str, substr);
//     printf("Result: %s\n", result);

//     free(result);

//     return 0;
// }

char buffer[] = "https:\/\/developer.y.qq.com\/openid\/1\/?a=2000000613&c=auh-O30dw30wWSQJwuGYz6Wu";

int main() {
    // const char utf8_str[] = "路由aac";  // 要转换的UTF-8编码字符串
    // for(int i = 0; i < strlen(utf8_str); i++) {
        
    // printf("0x%02x\n", utf8_str[i]);
    // }
    // unsigned int unicode = 0x8DEF;  // 要转换的Unicode编码

    // printf("UTF-8转Unicode编码结果：");
    // utf8_to_unicode2(utf8_str);

    // printf("Unicode转UTF-8编码结果：");
    // unicode_to_utf8(unicode);
    // const char *str = "1.0.11";
    // char *result = remove_substring(str, ".");
    // printf("Result: %s\n", result);
    // free(result);

#if 0
    const char utf8_str[] = "路由_-aac";  // The UTF-8 encoded string to convert
    
    int unicode_len = 0;
    uint16_t unicode_arr[100];              // Array to store the converted Unicode code points

    printf("UTF-8 to Unicode conversion result:\n");
    utf8_to_unicode(utf8_str, unicode_arr, &unicode_len);

    printf("Unicode code points:\n");
    for (int i = 0; i < unicode_len; i++) {
        printf("%04X ", unicode_arr[i]);
    }
    printf("\n");
    printf("Length: %d\n", unicode_len);

    uint8_t *buf = (uint8_t *)unicode_arr;

    for (int i = 0; i < unicode_len * sizeof(uint16_t); i++) {
        printf("%02X ", buf[i]);
    }
    printf("\n");
    printf("Length: %d\n", unicode_len);
#endif

    // char *addr = (char *)malloc(100);
    // printf("Address: %p\n", addr);
    // addr[0] = 0xa;
    
    // int add_p = &addr[0];
    // printf("Address: %p\n", &add_p);
    // // char *addr2 = add_p;
    // // printf("Address: %x\n", addr2[0]);
    
    // free(addr);

    printf("buf: %s\n", buffer);
    return 0;
}


