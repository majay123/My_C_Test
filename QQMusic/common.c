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
 * @Date         : 2024-07-24 13:01:29
 * @LastEditors  : MCD
 * @LastEditTime : 2024-10-08 13:26:33
 * @FilePath     : /My_C_Test/QQMusic/common.c
 * @Description  : 
 * 
 * ******************************************
 */

#include "common.h"
#include "cJSON.h"

/**
 * @brief 
 * 
 * @param a 
 * @param b 
 * @return int 
 */
int compare_keys(const void *a, const void *b)
{
    cJSON *json_a = *(cJSON **)a;
    cJSON *json_b = *(cJSON **)b;
    return strcmp(json_a->string, json_b->string);
}

/**
 * @brief 
 * 
 * @param json_string 
 * @return int 
 */
int sort_keys(const char *json_string)
{
    // 解析 JSON 字符串
    cJSON *json = cJSON_Parse(json_string);
    if (json == NULL) {
        printf("Error parsing JSON\n");
        return 1;
    }

    // 获取 JSON 对象中的键值对数量
    int num_keys = cJSON_GetArraySize(json);

    // 创建一个数组来存储键值对
    cJSON **items = (cJSON **)malloc(num_keys * sizeof(cJSON *));
    if (items == NULL) {
        printf("Memory allocation error\n");
        cJSON_Delete(json);
        return 1;
    }

    // 将键值对存储到数组中
    for (int i = 0; i < num_keys; i++) {
        items[i] = cJSON_GetArrayItem(json, i);
    }

    // 使用 qsort 函数对键值对按键名进行排序
    qsort(items, num_keys, sizeof(cJSON *), compare_keys);

    // 创建一个新的 JSON 对象来存储排序后的键值对
    cJSON *sorted_json = cJSON_CreateObject();
    for (int i = 0; i < num_keys; i++) {
        cJSON_AddItemToObject(sorted_json, items[i]->string, cJSON_Duplicate(items[i], 1));
    }

    // 将排序后的 JSON 对象转换为字符串
    char *json_buf = cJSON_PrintUnformatted(sorted_json);
    if (json_buf == NULL) {
        printf("Error creating JSON string\n");
        cJSON_Delete(json);
        cJSON_Delete(sorted_json);
        free(items);
        return 1;
    }

    // 打印排序后的 JSON 字符串
    printf("Sorted JSON:\n%s\n", json_buf);

    // 释放内存
    free(json_buf);
    cJSON_Delete(json);
    cJSON_Delete(sorted_json);
    free(items);

    return 0;
}

/**
 * @brief Get the external ip object
 * 
 * @return char* 获取IP后,需要将ip的内存释放掉,因为用的是strdup
 */
char *get_external_ip()
{
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;
    char *ip = NULL;
    cJSON *json = NULL;

    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://open.nbhope.cn:443/contents/network/info");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    printf("json: %s\n", chunk.memory);
    json = cJSON_Parse(chunk.memory);
    if (!json) {
        printf("Error parsing JSON\n");
        goto clean;
    }

    if (cJSON_GetObjectItem(json, "code") == NULL) {
        printf("Error code JSON\n");
        goto clean;
    }
    else {
        if (cJSON_GetObjectItem(json, "code")->valueint != 100000) {
            printf("Error code data\n");
            goto clean;
        }
        else {
            cJSON *object_item = cJSON_GetObjectItem(json, "object");
            if (object_item != NULL) {
                cJSON *clientIp_item = cJSON_GetObjectItem(object_item, "clientIp");
                ip = strdup(clientIp_item->valuestring);
            }
        }
    }
clean:
    if (chunk.memory) {
        free(chunk.memory);
    }
    if (json) {
        cJSON_Delete(json);
    }
    return ip;
}

void get_external_public_ip(void)
{
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;

    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        // curl_easy_setopt(curl, CURLOPT_URL, "https://deve.nbhope.cn:4443/contents/network/info");
        curl_easy_setopt(curl, CURLOPT_URL, "https://open.nbhope.cn:443/contents/network/info");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            free(chunk.memory);
            return NULL;
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    printf("json: %s\n", chunk.memory);
    // cJSON *json = cJSON_Parse(chunk.memory);
    // if (!json) {
    //     printf("Error parsing JSON\n");
    //     free(chunk.memory);
    //     return NULL;
    // }
    // cJSON *ip_item = cJSON_GetObjectItem(json, "ip");
    // char *ip = strdup(ip_item->valuestring);
    // cJSON_Delete(json);
    free(chunk.memory);

    return;
}

void get_external_ip_location(void)
{
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;

    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        // curl_easy_setopt(curl, CURLOPT_URL, "https://deve.nbhope.cn:4443/contents/network/info");
        curl_easy_setopt(curl, CURLOPT_URL, "https://open.nbhope.cn:443/contents/place/info");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            free(chunk.memory);
            return NULL;
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    printf("json: %s\n", chunk.memory);
    // cJSON *json = cJSON_Parse(chunk.memory);
    // if (!json) {
    //     printf("Error parsing JSON\n");
    //     free(chunk.memory);
    //     return NULL;
    // }
    // cJSON *ip_item = cJSON_GetObjectItem(json, "ip");
    // char *ip = strdup(ip_item->valuestring);
    // cJSON_Delete(json);
    free(chunk.memory);

    return;
}

void get_external_weather(void)
{
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;

    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        // curl_easy_setopt(curl, CURLOPT_URL, "https://deve.nbhope.cn:4443/contents/network/info");
        curl_easy_setopt(curl, CURLOPT_URL, "https://open.nbhope.cn:443/hopeApi/weather/info");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            free(chunk.memory);
            return NULL;
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    printf("json: %s\n", chunk.memory);
    // cJSON *json = cJSON_Parse(chunk.memory);
    // if (!json) {
    //     printf("Error parsing JSON\n");
    //     free(chunk.memory);
    //     return NULL;
    // }
    // cJSON *ip_item = cJSON_GetObjectItem(json, "ip");
    // char *ip = strdup(ip_item->valuestring);
    // cJSON_Delete(json);
    free(chunk.memory);

    return;
}

/**
 * @brief 获取MD5
 * 
 * @param str 
 * @param outputBuffer 
 */
void md5_string(const char *str, char *outputBuffer)
{
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5((unsigned char *)str, strlen(str), (unsigned char *)&digest);

    for (int i = 0; i < 16; ++i)
        sprintf(&outputBuffer[i * 2], "%02x", (unsigned int)digest[i]);
    
    printf("func1: %s\n", outputBuffer);

    // func2
    MD5_CTX_T md5;
    uint8_t hash[MD5_SUM_HEX_LEN] = {0};
    uint8_t y_temp[MD5_SUM_STR_LEN + 1] = {0};
    int i;
    MD5Init(&md5);
    MD5Update(&md5, str, strlens((char *)str));
    MD5Final(&md5, hash);

    for (int i = 0; i < 16; ++i)
        sprintf(&outputBuffer[i * 2], "%02x", (unsigned int)hash[i]);
    
    printf("func2: %s\n", outputBuffer);
}

/**
 * @brief Function to URL encode a string (for simplicity, handles only specific characters)
 * need free
 * 
 * @param str 
 * @return char* 
 */
char *url_encode(const char *str)
{
    char *enc_str = malloc(strlen(str) * 3 + 1);  // Allocate enough space
    char *pstr = (char *)str, *pbuf = enc_str;
    while (*pstr) {
        if ((*pstr >= '0' && *pstr <= '9') ||
            (*pstr >= 'A' && *pstr <= 'Z') ||
            (*pstr >= 'a' && *pstr <= 'z') ||
            *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~') {
            *pbuf++ = *pstr;
        }
        else {
            pbuf += sprintf(pbuf, "%%%02X", (unsigned char)*pstr);
        }
        pstr++;
    }
    *pbuf = '\0';
    return enc_str;
}

int get_strlen(const char *str)
{
    int pos = 0;
    int add = 0;

    if (str == NULL) {
        return -1;
    }

    while (str[pos]) {
        if (str[pos] & 1 << 7) {
            if (str[pos] & 1 << 6) {
                if (str[pos] & 1 << 5) {
                    if (str[pos] & 1 << 4) {
                        add++;
                        pos += 4;
                        continue;
                    }
                    // a word is three byte but we only need a byte
                    add++;
                    pos += 3;
                    continue;
                }
                add++;
                pos++;
                continue;
            }
        }
        else {
            pos++;
            add++;
            continue;
        }
    }
    return add;
}

uint8_t calc_des(size_t len)
{
    uint8_t x_temp[256] = {0};
    uint8_t y_temp[MD5_SUM_STR_LEN + 1] = {0};
    uint8_t des = 0;
    MD5_CTX_T md5;
    uint8_t i = 0;
    uint8_t hash[MD5_SUM_HEX_LEN] = {0};

    snprintf((char *)x_temp, sizeof(x_temp), "%u%s%s%s%s", len, APP_DEFAULT_HTTP_KEY, APP_DEFAULT_HTTP_CID,
             APP_DEFAULT_HTTP_SID, APP_DEFAULT_HTTP_VERSION);
    MD5Init(&md5);
    MD5Update(&md5, x_temp, strlens((char *)x_temp));
    MD5Final(&md5, hash);

    for (i = 0; i < MD5_SUM_HEX_LEN; i++)
        snprintf((char *)y_temp, sizeof(y_temp), "%s%02X", y_temp, hash[i]);

    for (i = 0; i < MD5_SUM_STR_LEN; i++)
        des ^= y_temp[i];

    return des;
}

uint8_t calc_des2(size_t len)
{
    MD5_CTX md5;
    unsigned char decrypt[16] = {0};
    char hex_str[33] = "";
    unsigned char hex = 0;
    char len_str[10] = "";
    char des_str[5] = "";
    int i;

    // sprintf(len_str, "%d", len);
    snprintf(len_str, sizeof(len_str), "%d", len);
    size_t l = strlen(len_str) + strlen(APP_DEFAULT_HTTP_KEY) + strlen(APP_DEFAULT_HTTP_CID) + strlen(APP_DEFAULT_HTTP_SID) + strlen(APP_DEFAULT_HTTP_VERSION) + 1;
    char *str_split = (char *)malloc(l);
    if (str_split == NULL) {
        // print_regist("malloc failed");
        return -1;
    }
    bzero(str_split, l);

    sprintf(str_split, "%s%s%s%s%s", len_str, APP_DEFAULT_HTTP_KEY, APP_DEFAULT_HTTP_CID, APP_DEFAULT_HTTP_SID, APP_DEFAULT_HTTP_VERSION);
    MD5Init(&md5);
    MD5Update(&md5, (unsigned char *)str_split, strlen(str_split));
    MD5Final(&md5, decrypt);
    hex = decrypt[0];
    bzero(hex_str, sizeof(hex_str));
    char tmp[3] = "";
    for (i = 0; i < 16; i++) {
        memset(tmp, 0, 3);
        sprintf(tmp, "%02X", decrypt[i]);
        strcat(hex_str, tmp);
        // sprintf(hex_str, "%s%02X", hex_str, decrypt[i]);
    }
    hex = hex_str[0];
    for (i = 1; i < 32; i++) {
        hex ^= hex_str[i];
    }

    return hex;
}