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
 * @Date         : 2024-12-18 15:59:39
 * @LastEditors  : MCD
 * @LastEditTime : 2024-12-18 15:59:40
 * @FilePath     : /My_C_Test/extracted_name/main.c
 * @Description  : 
 * 
 * ******************************************
 */
#include <stdio.h>
#include <string.h>

int extract_filename(const char *path, char *buff, size_t buff_size) {
    // 找到最后一个 '/' 的位置
    const char *last_slash = strrchr(path, '/');
    if (last_slash == NULL) {
        return -1; // 返回错误，路径无效
    }

    // 提取文件名部分
    const char *filename = last_slash + 1;

    // 找到最后一个 '.' 的位置
    const char *last_dot = strrchr(filename, '.');
    if (last_dot == NULL) {
        return -2; // 返回错误，没有文件扩展名
    }

    // 计算不带扩展名的长度
    size_t name_length = last_dot - filename;

    // 检查缓冲区是否足够大
    if (name_length >= buff_size) {
        return -3; // 返回错误，缓冲区太小
    }

    // 复制不带扩展名的文件名到缓冲区
    strncpy(buff, filename, name_length);
    buff[name_length] = '\0'; // 确保缓冲区以 '\0' 结尾

    return 0; // 成功
}

int main() {
    const char *path = "/mnt/card/test/你好.flac";
    char buff[256] = {0}; // 用于存储提取的文件名

    int result = extract_filename(path, buff, sizeof(buff));
    if (result == 0) {
        printf("Extracted name: %s\n", buff);
    } else {
        printf("Error extracting name: %d\n", result);
    }

    return 0;
}
