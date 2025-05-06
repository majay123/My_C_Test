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
 * @Date         : 2024-03-18 16:59:56
 * @LastEditors  : MCD
 * @LastEditTime : 2025-03-27 11:00:34
 * @FilePath     : /My_C_Test/chinese_char/main.c
 * @Description  : 
 * 
 * ******************************************
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <string.h>


#define REFRENCEID_MAX_SIZE     (128)
#define TYPE_MAX_SIZE           (16)
#define NAME_MAX_SIZE           (128)
#define MUSICID_MAX_SIZE        (32)
#define PIC_URL_MAX_SIZE        (256)
#define PIC_BIT_MAP_SIZE        (3 * 1024)

#define RANK_MODE       "RankMusic"
#define LIST_MODE       "SheetMusic"
#define RADIO_MODE      "RadioMusic"

enum {
    HOPE_PRO_NULL,
    HOPE_PRO_BLE_MUSIC,
    HOPE_PRO_LOCAL_MUSIC,
    HOPE_PRO_LINEIN_MUSIC,
    HOPE_PRO_SCENES_MUSIC = 5,
};


typedef struct
{
    char refrenceId[REFRENCEID_MAX_SIZE];
    char type[TYPE_MAX_SIZE];
    char name[NAME_MAX_SIZE];
}rank_info_t;

typedef struct
{
    char pic_url[PIC_URL_MAX_SIZE];
    char refrenceId[REFRENCEID_MAX_SIZE];
    char type[TYPE_MAX_SIZE];
    char name[NAME_MAX_SIZE];
    int page;   //1
    int size;   //100
    char mode[TYPE_MAX_SIZE];
    int id;
    int gid;
    int sid;
    char category_id[REFRENCEID_MAX_SIZE * 8];
}detail_info_t;

int is_chinese_character(uint8_t *ch) {
    if ((ch[0] & 0xE0) == 0xE0 && (ch[1] & 0x80) == 0x80 && (ch[2] & 0x80) == 0x80) {
        return 1;
    }
    return 0;
}

int count_chinese_characters(const char *str) {
    int chinese_count = 0;
    uint8_t *ptr = (uint8_t *)str;

    while (*ptr != '\0') {
        if (is_chinese_character(ptr)) {
            chinese_count++;
            // Move the pointer to the next character
            ptr += 3;
        } else {
            // Move the pointer to the next byte
            ptr++;
        }
    }

    return chinese_count;
}

int count_characters(const char *str) {
    int char_count = 0;
    uint8_t *ptr = (uint8_t *)str;

    while (*ptr != '\0') {
        if (is_chinese_character(ptr)) {
            char_count++;
            // Move the pointer to the next character
            ptr += 3;
        } else {
            char_count++;
            // Move the pointer to the next byte
            ptr++;
        }
    }

    return char_count;
}


void utf8_strncpy(char *dest, const char *src, size_t n) {
    size_t i = 0;
    size_t count = 0;

    while (count < n && src[i] != '\0') {
        if ((src[i] & 0xC0) != 0x80) {  // Check if it's the start of a new UTF-8 character
            count++;
        }
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';  // Null-terminate the destination string
}

int main()
{
    const char *sentence = "Hello,你好世界 我的家";  // 包含中文字符的句子
    char dest[50] = {0}; 
    // const char *p = sentence;
    uint8_t count = count_characters(sentence);
    printf("中文字符数: %d\n", count);

    utf8_strncpy(dest, sentence, count + 1);
    printf("p: %s\n", dest);

    printf("%d\n", sizeof(detail_info_t));

    return 0;
}