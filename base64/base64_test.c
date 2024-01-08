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
 * @LastEditTime : 2023-10-25 10:13:57
 * @FilePath     : /My_C_Test/base64/base64_test.c
 * @Description  : 
 * 
 * ******************************************
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

static const size_t BASE64_ENCODE_INPUT = 3;
static const size_t BASE64_ENCODE_OUTPUT = 4;
static const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

size_t base64EncodeGetLength( size_t size )
{
    return (((size + BASE64_ENCODE_INPUT - 1) / BASE64_ENCODE_INPUT) * BASE64_ENCODE_OUTPUT) + 1; 
}


void base64_encode(const uint8_t *data, size_t input_length, char *output) {
    size_t i = 0;
    size_t j = 0;
    uint8_t a3[3];
    uint8_t a4[4];

    while (input_length--) {
        a3[i++] = *(data++);
        if (i == 3) {
            a4[0] = (a3[0] & 0xfc) >> 2;
            a4[1] = ((a3[0] & 0x03) << 4) + ((a3[1] & 0xf0) >> 4);
            a4[2] = ((a3[1] & 0x0f) << 2) + ((a3[2] & 0xc0) >> 6);
            a4[3] = a3[2] & 0x3f;

            for (i = 0; i < 4; i++) {
                output[j++] = base64_chars[a4[i]];
            }
            i = 0;
        }
    }

    if (i) {
        for (size_t k = i; k < 3; k++) {
            a3[k] = 0;
        }

        a4[0] = (a3[0] & 0xfc) >> 2;
        a4[1] = ((a3[0] & 0x03) << 4) + ((a3[1] & 0xf0) >> 4);
        a4[2] = ((a3[1] & 0x0f) << 2) + ((a3[2] & 0xc0) >> 6);
        a4[3] = a3[2] & 0x3f;

        for (size_t k = 0; k < i + 1; k++) {
            output[j++] = base64_chars[a4[k]];
        }

        while (i++ < 3) {
            output[j++] = '=';
        }
    }
    output[j] = '\0';
}

int main() {
#if 0
    const char *input = "{\"scene\":\"10游戏开始3\",\"device\":\"group\"}\n{\"scene\":\"10游戏开始3\",\"device\":\"group\"}\n";  // 包含中文字符的输入数据

    size_t size = base64EncodeGetLength(strlen(input));

    char *encoded = calloc(size, sizeof(char));
    // char encoded[256];

    // 计算输入数据长度（包括中文字符）
    size_t input_length = strlen(input);

    base64_encode((const uint8_t *)input, input_length, encoded);
    printf("Base64 Encoded: %s\n", encoded);

    free(encoded);
#endif
    uint8_t retry = 0;

    while(retry++ < 5) {
        printf("Retrying: %d\n", retry);
        if(retry == 3)
            break;
    }

    return 0;
}
