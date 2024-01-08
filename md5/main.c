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
 * @Date         : 2023-11-08 16:51:42
 * @LastEditors  : MCD
 * @LastEditTime : 2023-12-07 16:26:56
 * @FilePath     : /My_C_Test/md5/main.c
 * @Description  : 
 * 
 * ******************************************
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

// 左循环移位宏定义
#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

// MD5算法的四个基本转换函数
#define F(X, Y, Z) (((X) & (Y)) | ((~(X)) & (Z)))
#define G(X, Y, Z) (((X) & (Z)) | ((Y) & (~(Z))))
#define H(X, Y, Z) ((X) ^ (Y) ^ (Z))
#define I(X, Y, Z) ((Y) ^ ((X) | ~(Z)))


// 定义SHA-1算法的常数
#define SHA1_BLOCK_SIZE 64
#define SHA1_DIGEST_SIZE 20

// 定义SHA-1算法的辅助宏
#define SHA1_ROL(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

// SHA-1算法的轮函数
#define SHA1_F1(x, y, z) (((x) & (y)) | (~(x) & (z)))
#define SHA1_F2(x, y, z) ((x) ^ (y) ^ (z))
#define SHA1_F3(x, y, z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define SHA1_F4(x, y, z) ((x) ^ (y) ^ (z))

// SHA-1算法主循环
void sha1_transform(uint32_t state[5], const uint8_t data[64]) {
    uint32_t a = state[0];
    uint32_t b = state[1];
    uint32_t c = state[2];
    uint32_t d = state[3];
    uint32_t e = state[4];
    uint32_t w[80];
    
    for (int i = 0; i < 16; i++) {
        w[i] = (data[i * 4] << 24) | (data[i * 4 + 1] << 16) | (data[i * 4 + 2] << 8) | data[i * 4 + 3];
    }
    
    for (int i = 16; i < 80; i++) {
        w[i] = SHA1_ROL(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
    }
    
    for (int i = 0; i < 80; i++) {
        uint32_t temp = SHA1_ROL(a, 5) + 0;
        if (i < 20) {
            temp += SHA1_F1(b, c, d) + e + 0x5A827999 + w[i];
        } else if (i < 40) {
            temp += SHA1_F2(b, c, d) + e + 0x6ED9EBA1 + w[i];
        } else if (i < 60) {
            temp += SHA1_F3(b, c, d) + e + 0x8F1BBCDC + w[i];
        } else {
            temp += SHA1_F4(b, c, d) + e + 0xCA62C1D6 + w[i];
        }
        e = d;
        d = c;
        c = SHA1_ROL(b, 30);
        b = a;
        a = temp;
    }
    
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
}

// SHA-1哈希计算函数
void sha1(const uint8_t *message, size_t length, uint8_t digest[SHA1_DIGEST_SIZE]) {
    uint32_t state[5] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};
    uint8_t buffer[SHA1_BLOCK_SIZE];
    size_t bit_len = length * 8;
    size_t i;

    for (i = 0; i < length / SHA1_BLOCK_SIZE; i++) {
        memcpy(buffer, message + i * SHA1_BLOCK_SIZE, SHA1_BLOCK_SIZE);
        sha1_transform(state, buffer);
    }

    memset(buffer, 0, SHA1_BLOCK_SIZE);

    for (i = 0; i < length % SHA1_BLOCK_SIZE; i++) {
        buffer[i] = message[i + (length / SHA1_BLOCK_SIZE) * SHA1_BLOCK_SIZE];
    }

    buffer[i] = 0x80;

    if (i >= 56) {
        sha1_transform(state, buffer);
        memset(buffer, 0, SHA1_BLOCK_SIZE);
    }

    // 将消息的位长度附加到末尾
    buffer[63] = (bit_len >> 0) & 0xFF;
    buffer[62] = (bit_len >> 8) & 0xFF;
    buffer[61] = (bit_len >> 16) & 0xFF;
    buffer[60] = (bit_len >> 24) & 0xFF;
    buffer[59] = (bit_len >> 32) & 0xFF;
    buffer[58] = (bit_len >> 40) & 0xFF;
    buffer[57] = (bit_len >> 48) & 0xFF;
    buffer[56] = (bit_len >> 56) & 0xFF;

    sha1_transform(state, buffer);

    // 将最终哈希值写入digest数组
    for (i = 0; i < SHA1_DIGEST_SIZE; i++) {
        digest[i] = (state[i >> 2] >> ((3 - (i & 3)) * 8)) & 0xFF;
    }
}

// 填充消息数据，返回填充后的消息长度
size_t pad_message(const uint8_t *input, uint8_t *output, size_t length) {
    size_t new_length = length;
    while ((new_length % 64) != 56) {
        new_length++;
    }
    
    output = (uint8_t *)calloc(new_length + 8, 1);
    if (output == NULL) {
        return 0;
    }
    
    memcpy(output, input, length);
    output[length] = 0x80; // 添加一个1后跟零字节
    
    for (size_t i = length + 1; i < new_length; i++) {
        output[i] = 0x00;
    }
    
    // 原始消息长度的位数，以64位的形式附加到消息的末尾
    uint64_t original_length = length * 8;
    memcpy(output + new_length, &original_length, 8);
    
    return new_length + 8;
}

// 计算MD5哈希值
void calculate_md5(const uint8_t *message, size_t length, uint8_t digest[16]) {
    // 初始哈希值（32位整数）
    uint32_t A = 0x67452301;
    uint32_t B = 0xefcdab89;
    uint32_t C = 0x98badcfe;
    uint32_t D = 0x10325476;
    
    // 表示MD5循环中的常量值
    uint32_t k[] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
    };

    if (message == NULL)
        return 0;
    
    // 对消息进行填充
    uint8_t *padded_message = NULL;
    size_t new_length = pad_message(message, padded_message, length);

    if(padded_message == NULL)
        return 0;
        
    
    // 处理每个64字节块的消息
    for (size_t offset = 0; offset < new_length; offset += 64) {
        uint32_t *words = (uint32_t *)(padded_message + offset);
        uint32_t AA = A;
        uint32_t BB = B;
        uint32_t CC = C;
        uint32_t DD = D;
        
        for (int i = 0; i < 64; i++) {
            uint32_t F, g;
            
            if (i < 16) {
                F = F(BB, CC, DD);
                g = i;
            } else if (i < 32) {
                F = G(BB, CC, DD);
                g = (5 * i + 1) % 16;
            } else if (i < 48) {
                F = H(BB, CC, DD);
                g = (3 * i + 5) % 16;
            } else {
                F = I(BB, CC, DD);
                g = (7 * i) % 16;
            }
            
            uint32_t temp = DD;
            DD = CC;
            CC = BB;
            BB = BB + LEFTROTATE((AA + F + k[i] + words[g]), 7);
            AA = temp;
        }
        
        A += AA;
        B += BB;
        C += CC;
        D += DD;
    }
    
    free(padded_message);
    
    // 将最终哈希值写入digest数组
    uint8_t *p = (uint8_t *)&A;
    for (int i = 0; i < 4; i++) {
        digest[i] = p[i];
    }
    
    p = (uint8_t *)&B;
    for (int i = 0; i < 4; i++) {
        digest[i + 4] = p[i];
    }
    
    p = (uint8_t *)&C;
    for (int i = 0; i < 4; i++) {
        digest[i + 8] = p[i];
    }
    
    p = (uint8_t *)&D;
    for (int i = 0; i < 4; i++) {
        digest[i + 12] = p[i];
    }
}

#define LIGHT_TMP_COLOR_MIN      (3000)
#define LIGHT_TMP_COLOR_MAX      (6400)

int main() {
    const char *text = "Hello, World!";
    uint8_t md5_hash[16]; // MD5哈希值是16字节
    uint8_t sha1_hash[SHA1_DIGEST_SIZE];
    
    // calculate_md5((const uint8_t *)text, strlen(text), md5_hash);
    
    // printf("MD5 Hash: ");
    // for (int i = 0; i < 16; i++) {
    //     printf("%02x", md5_hash[i]);
    // }
    // printf("\n");

    sha1((const uint8_t *)text, strlen(text), sha1_hash);

    // printf("SHA-1 Hash: ");
    // for (int i = 0; i < SHA1_DIGEST_SIZE; i++) {
    //     printf("%02x", sha1_hash[i]);
    // }
    // printf("\n");

    // 创建一个足够大的字符数组来存储转换后的字符串
    char str[100]; // 假设数组不会太大，这里分配足够的空间

    // 使用 sprintf 将数组元素转换成字符串并以逗号分隔
    int pos = 0;
    for (int i = 0; i < SHA1_DIGEST_SIZE - 4; i++) {
        pos += sprintf(str + pos, "%02x", sha1_hash[i]);
        // if (i < SHA1_DIGEST_SIZE - 1) {
        //     pos += sprintf(str + pos, ", ");
        // }
    }

    printf("Array as a string: %s\n", str);
    int attrValue = 3600;
    int result = 0;
    int result1 = 0;

    result = ((attrValue - LIGHT_TMP_COLOR_MIN) * 10000 / (LIGHT_TMP_COLOR_MAX - LIGHT_TMP_COLOR_MIN - 3) + 5) / 10;

    result1 = result * (LIGHT_TMP_COLOR_MAX - LIGHT_TMP_COLOR_MIN) / 1000 + LIGHT_TMP_COLOR_MIN;
    printf("result = %d, result1 = %d\n", result, result1);

    return 0;
}

// #include "net/wifi_manager.h"
// #include "net/ethernet_manager.h"
// #include "net/config.h"

// wifi_info_t wifi_info;
// eth_info_t eth_info;
// ret = wifi_get_wifi_info(&wifi_info);
// ret = ethernet_get_eth_info(&eth_info);

// if(wifi_info.status == WIFI_STATE_CONNECTED)
// else if(eth_info.status == ETHERNET_STATE_ENABLED)
