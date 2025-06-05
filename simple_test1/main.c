/*
 * 
 * ************Copyright 2025 MCD************
 * 
 * @version      : 
 * @Company      : HOPE
 * @Author       : MCD
 * @Date         : 2025-05-06 09:51:19
 * @LastEditors  : MCD
 * @LastEditTime : 2025-05-20 09:12:02
 * @FilePath     : /My_C_Test/simple_test1/main.c
 * @Description  : 
 * 
 * ******************************************
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

// 判断是否是符号字符
int is_symbol(char c) {
    return ispunct((unsigned char)c);
}

// 去除符号，保留非符号字符
void remove_symbols(const char *src, char *dest) {
    while (*src) {
        if (!is_symbol(*src)) {
            *dest++ = *src;
        }
        src++;
    }
    *dest = '\0'; // 添加结束符
}

void test_for_v(int *vip) 
{
    *vip = 22;
}


#define ELOG_FILE_NAME      "/tmp/elog_file.log"
static bool elog_file_rotate(void)
{
#define SUFFIX_LEN                     10
    /* mv xxx.log.n-1 => xxx.log.n, and xxx.log => xxx.log.0 */
    int n, err = 0;
    char oldpath[256]= {0}, newpath[256] = {0};
    size_t base = strlen(ELOG_FILE_NAME);
    bool result = true;
    FILE *tmp_fp;

    memcpy(oldpath, ELOG_FILE_NAME, base);
    memcpy(newpath, ELOG_FILE_NAME, base);


    for (n = 5 - 1; n >= 0; --n) {
        snprintf(oldpath + base, SUFFIX_LEN, n ? ".%d" : "", n - 1);
        snprintf(newpath + base, SUFFIX_LEN, ".%d", n);

        printf("oldpath: %s, newpath: %s\n", oldpath, newpath);
        /* remove the old file */
        // if ((tmp_fp = fopen(newpath , "r")) != NULL) {
        //     fclose(tmp_fp);
        //     remove(newpath);
        // }
        // /* change the new log file to old file name */
        // if ((tmp_fp = fopen(oldpath , "r")) != NULL) {
        //     fclose(tmp_fp);
        //     err = rename(oldpath, newpath);
        // }

        // if (err < 0) {
        //     result = false;
        //     goto __exit;
        // }
    }

__exit:
    /* reopen the file */
    // fp = fopen(local_cfg.name, "a+");

    return result;
}

#include <stdio.h>
#include <string.h>

int match_http_url() {
    const char *input = "live://https://dds-ack.dui.ai/runtime/v1/longtext/e2b0810314c84e41bde0375451fdbf9e?productId=278576556&aispeech-da-env=hd-ack";

    // 查找 http:// 或 https:// 出现的位置
    const char *http_pos = strstr(input, "http");
    const char *https_pos = strstr(input, "http");

    printf("http_pos: %s, https_pos: %s\n", http_pos, https_pos);

    const char *url_start = NULL;

    // 优先使用 https，如果没有再使用 http
    if (https_pos) {
        url_start = https_pos;
    } else if (http_pos) {
        url_start = http_pos;
    }

    if (url_start) {
        // 复制出 URL（直到遇到空格或字符串结束）
        char url[512] = {0};
        int i = 0;
        while (url_start[i] != '\0' && url_start[i] != ' ' && i < sizeof(url) - 1) {
            url[i] = url_start[i];
            i++;
        }
        url[i] = '\0';

        printf("匹配到的 URL: %s\n", url);
    } else {
        printf("没有找到 http 或 https 开头的 URL\n");
    }

    return 0;
}


int main() {
    const char *a = "Somniomancernullsetaaa";
    const char *b = "Somnio)(=><*&^&mancer[nullset~!@#@#|)]aaa";

    // char cleaned_b[256];  // 清理后的 B 字符串
    // remove_symbols(b, cleaned_b);

    // if (strcmp(a, cleaned_b) == 0) {
    //     printf("1\n");
    // } else {
    //     printf("0\n");
    // }
    // int vip = 0;
    // test_for_v((int *)&vip);
    // printf("%d\n", vip);

    // elog_file_rotate();

    match_http_url();
    return 0;
}