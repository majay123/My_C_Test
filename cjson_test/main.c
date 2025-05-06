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
 * @Date         : 2023-04-07 10:09:05
 * @LastEditors  : MCD
 * @LastEditTime : 2025-04-16 09:17:06
 * @FilePath     : /My_C_Test/cjson_test/main.c
 * @Description  : 
 * 
 * ******************************************
 */
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "cJSON.h"

#define MAX_SCENE_NAME_LENGTH  (28 * 2)
#define MAX_DEVICE_NAME_LENGTH (28)
#define MAX_DEVICE_ID_LENGTH   (16)
#define MAX_DEVICE_TYPE_LENGTH (24)
#define MAX_SCENE_DEVICE_SIZE  (10)
#define MAX_SCENE_DEVICE_SIZE1 (2)

#define REQUIRE(in, tag)                  \
    do {                                  \
        if (in) {                         \
            printf("%s %s\n", #tag, #in); \
            goto tag;                     \
        }                                 \
    } while (0)

#define REQ_JSON_OBJ(des, item, tag) \
    cJSON_GetObjectItem(des, #item); \
    REQUIRE((item == NULL), tag)

#define REQ_JSON_PARSE(str, item, tag) \
    cJSON_Parse(str);                  \
    REQUIRE((item == NULL), tag)

static const size_t BASE64_ENCODE_INPUT = 3;
static const size_t BASE64_ENCODE_OUTPUT = 4;
static const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const char json_buf[] = "{\"id\":\"3\",\"name\":\"zhuti\",\"list\":[{\"name\":\"音乐人在听\",\"id\":1069},{\"name\":\"KTV金曲\",\"id\":64}]}";
typedef struct _Datapoint {
    uint32_t dpid;
    char v_string[28];

} Datapoint;

typedef struct _btActionMsg {
    char deviceId[16];   /* control device id sub device id */
    char deviceName[28]; /* display device name(for switch name or device name or sceneName or groupName) */
    char devicePid[24];  /* device pid */
    int32_t dp_size;
    Datapoint dp[4]; /* datapoint(ctrl and status) only one */
} btActionMsg;

// 编码URL
char *url_encode(const char *input)
{
    const char *hex = "0123456789ABCDEF";
    size_t len = strlen(input);
    char *encoded = (char *)malloc(len * 3 + 1);  // 足够大以容纳编码后的字符串

    if (encoded) {
        size_t j = 0;
        for (size_t i = 0; i < len; i++) {
            char c = input[i];
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.' || c == '~') {
                encoded[j++] = c;
            }
            else {
                encoded[j++] = '%';
                encoded[j++] = hex[(c >> 4) & 0xF];
                encoded[j++] = hex[c & 0xF];
            }
        }
        encoded[j] = '\0';
    }

    return encoded;
}

// 解码URL
char *url_decode(const char *input)
{
    size_t len = strlen(input);
    char *decoded = (char *)malloc(len + 1);

    if (decoded) {
        size_t j = 0;
        for (size_t i = 0; i < len; i++) {
            if (input[i] == '%' && i + 2 < len) {
                char hex[3];
                hex[0] = input[i + 1];
                hex[1] = input[i + 2];
                hex[2] = '\0';
                int decoded_char = strtol(hex, NULL, 16);
                decoded[j++] = (char)decoded_char;
                i += 2;
            }
            else if (input[i] == '+') {
                decoded[j++] = ' ';
            }
            else {
                decoded[j++] = input[i];
            }
        }
        decoded[j] = '\0';
    }

    return decoded;
}

// int main() {
//     const char *input = "Hello World!";
//     char *encoded = urlencode(input);
//     printf("Encoded: %s\n", encoded);

//     char *decoded = urldecode(encoded);
//     printf("Decoded: %s\n", decoded);

//     free(encoded);
//     free(decoded);

//     return 0;
// }

static void cjson_test1(void)
{
    cJSON *root = cJSON_CreateObject();
    cJSON *cids_item = cJSON_AddArrayToObject(root, "cids");
    cJSON *rets_item = cJSON_AddArrayToObject(root, "rets");

    cJSON_AddStringToObject(cids_item, "cids", "test1");
    cJSON_AddStringToObject(cids_item, "cids", "test2");
    cJSON_AddStringToObject(cids_item, "cids", "test3");
    cJSON_AddStringToObject(cids_item, "cids", "test4");

    cJSON_AddNumberToObject(rets_item, "rets", 1);
    cJSON_AddNumberToObject(rets_item, "rets", 2);
    cJSON_AddNumberToObject(rets_item, "rets", 3);
    cJSON_AddNumberToObject(rets_item, "rets", 4);

    char *buf = cJSON_PrintUnformatted(root);
    printf("%s\n", buf);
    cJSON_free(buf);
    cJSON_Delete(root);
}

static void _cjson_test2(void)
{
    int ret = -1;
    cJSON *root = NULL;

    root = cJSON_CreateObject();

    if (NULL == root) {
        printf("cJSON_CreateObject error\n");
        return ret;
    }

    if (!cJSON_AddStringToObject(root, "dev_id", ("del_dev->deviceid"))) {
        printf("cJSON_AddItemToObject error\n");
        cJSON_Delete(root);
        return ret;
    }
    if (!cJSON_AddItemToObject(root, "dev_key", cJSON_CreateString("LESHI_DATA_DEVKEY"))) {
        printf("cJSON_AddItemToObject error\n");
        cJSON_Delete(root);
        return ret;
    }

    char *buf = cJSON_PrintUnformatted(root);
    if (NULL == buf) {
        printf("cJSON_PrintUnformatted error\n");
        cJSON_Delete(root);
        return ret;
    }
    printf("%s\n", buf);

    if (buf)
        free(buf);
    if (root)
        cJSON_Delete(root);
}

static int _cjson_test3(void)
{
    char *buf = "{\"dev_id\":\"adfa1244sadf3\",\"dev_key\":\"LESHI_DATA_DEVKEY\"}";
    cJSON *root = NULL;
    uint8_t data = 1;  // 0:接收数据成功 1:数据接收或者解析失败

    // REQUIRE(NULL == buf, Error);

    root = REQ_JSON_PARSE((const char *)buf, root, Error);

    /* Processe cjson information */
    cJSON *dev_id = REQ_JSON_OBJ(root, dev_id, Error);
    cJSON *dev_key = REQ_JSON_OBJ(root, dev_key, Error);

    printf("dev_id = %s, dev_key = %s\n", dev_id->valuestring, dev_key->valuestring);
    char *id = dev_id->valuestring;
    printf("id = %s\n", id);

Error:
    if (root != NULL)
        cJSON_Delete(root);
}

static int printf_ids(char *pid, char **ids, char *ver, size_t size)
{
    printf("pid = %s\n", pid);
    printf("ver = %s\n", ver);
    for (size_t i = 0; i < size; i++) {
        printf("ids[%d] = %s\n", i, ids[i]);
    }
}

static int _cjson_test4(void)
{
    int i = 0;
    cJSON *root = NULL;
    char *cmd_data = "{\"pid\":\"drmec7gltih2y8dk\", \"cids\":[\"adfa1244sadf3\", \"adcadfa1dafg\"],\"ver\":\"1.2.0\"}";

    printf("batch add sub dev: %s\n", cmd_data);
    root = REQ_JSON_PARSE((const char *)cmd_data, root, Error);

    /* Processe cjson information */
    cJSON *pid = REQ_JSON_OBJ(root, pid, Error);
    cJSON *cids = REQ_JSON_OBJ(root, cids, Error);
    cJSON *ver = REQ_JSON_OBJ(root, ver, Error);

    printf("pid = %s\n", pid->valuestring);
    int csize = cJSON_GetArraySize(cids);
    char **mdev = (char **)calloc(csize, sizeof(char *));
    if (csize > 0) {
        for (i = 0; i < csize; i++) {
            cJSON *cid_js = cJSON_GetArrayItem(cids, i);
            if (cid_js) {
                printf("cid = %s\n", cid_js->valuestring);
                mdev[i] = cid_js->valuestring;
            }
        }
    }
    printf_ids(pid->valuestring, mdev, ver->valuestring, csize);
Error:
    free(mdev);
    // free(mdev);

    if (root != NULL)
        cJSON_Delete(root);
}
char *test = "{\"code\":100000,\"desc\":\"success!\",\"message\":\"成功！\",\"object\":{\"authCode\":\"35C9AF1B921C4033898E9893FFAB38A4\",\"refrenceId\":1487623230113746944}}";
#define BREAK_UINT32(var, ByteNum) \
    (uint8_t)((uint32_t)(((var) >> ((ByteNum)*8)) & 0x00FF))

typedef struct
{
    uint8_t dpid;
    uint8_t type;
    uint8_t len[2];
    uint8_t *value;
} _leshi_dp_t;

char deviceIds[128][16] = {
    {"test111"},
    {"test222"},
    {"test333"},
    {"test444"},
    {"test555"},
    {"test666"},
    {"test777"},
};

char *about_info[6] = {
    "产品型号：",
    "软件版本：",
    "固件版本：",
    "SN    号：",
    "IP  地址：",
    "MAC 地址：",
};

void test_idx(int idx)
{
    printf("idx = %d\n", idx);
    idx = idx + 2;
    printf("idx = %d\n", idx);
}

static size_t base64EncodeGetLength(size_t size)
{
    return (((size + BASE64_ENCODE_INPUT - 1) / BASE64_ENCODE_INPUT) * BASE64_ENCODE_OUTPUT) + 1;
}

void base64_encode(const uint8_t *data, size_t input_length, char *output)
{
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

static btActionMsg *fill_data_test(void)
{
    char *devicId[] = {
        "013174e32f",
        "b2944c6422",
        "devicId33",
        "devicId44",
    };
    char *deviceName[] = {
        "色温灯",
        "devicename22",
        "devicename33",
        "devicename44",
    };
    char *deviceDpName[] = {
        "开关一",
        "开关二",
        "开关三",
        "开关四",
    };

    btActionMsg *msg = (btActionMsg *)calloc(4, sizeof(btActionMsg));
    for (size_t i = 0; i < 4; i++) {
        memcpy(msg[i].deviceId, devicId[i], strlen(devicId[i]));
        switch (i) {
            case 0:
            case 3:
                msg[i].dp_size = 0;
                if (i == 0)
                    memcpy(msg[i].devicePid, "fkxcslivaluonzdp", strlen("fkxcslivaluonzdp"));
                else
                    memcpy(msg[i].devicePid, "h536ubqx1tui1jsj", strlen("h536ubqx1tui1jsj"));
                memcpy(msg[i].deviceName, deviceName[i], strlen(deviceName[i]));
                break;
            case 1:
                msg[i].dp_size = 4;
                for (size_t j = 0; j < msg[i].dp_size; j++) {
                    memcpy(msg[i].dp[j].v_string, deviceDpName[j], strlen(deviceDpName[j]));
                }

                break;
            case 2:
                msg[i].dp_size = 2;
                for (size_t j = 0; j < msg[i].dp_size; j++) {
                    memcpy(msg[i].dp[j].v_string, deviceDpName[j], strlen(deviceDpName[j]));
                }
                break;
            default:
                break;
        }
    }

    return msg;
}

#define UPLOAD_BODY                                                                                \
    "appid=bc85460d&res_name=IFLYTEK.telephone_contact&pers_param=%7B%22auth_id%22%3A%"            \
    "22d3b6d50a9f8194b623b5e2d4e298c9d6%22%7D&data="                                               \
    "eyJuYW1lIjoi5byg5LiJIiwicGhvbmVOdW1iZXIiOiIxMzg4ODg4ODg4OCJ9DQp7Im5hbWUiOiLmnY7lm5siLCJwaG9u" \
    "ZU51bWJlciI6IjEzNjY2NjY2NjY2In0%3D"

#define UPLOAD_BODY1                                                  \                           
	"appid=%s&res_name=%s&pers_param=%s&data=%s"

int listen_up_Sync_devices_or_scenes(const char *resName, const char *data)
{
    cJSON *root = NULL;
    cJSON *param = NULL;
    char *json_buf = NULL;
    int ret = -1;

    printf("up_Sync_devices_or_scenes\n");

#if 0
    root = cJSON_CreateObject();
    param = cJSON_CreateObject();

    if (root == NULL || param == NULL) {
        printf("cJSON_CreateObject failed!\n");
        goto Error;
    }

    cJSON_AddStringToObject(param, "res_name", resName);
    cJSON_AddStringToObject(param, "id_name", "uid");
    cJSON_AddStringToObject(param, "appid", "bc85460d");
    cJSON_AddItemToObject(root, "param", param);
    cJSON_AddStringToObject(root, "data", data);

    json_buf = cJSON_PrintUnformatted(root);
    printf("json_buf = % s\n", json_buf);
#else
    char buffer[256] = {0};
    char *buf = "{\"auth_id\":\"%s\"}";
    sprintf(buffer, buf, "61686ed01f0f6c96e0b0a7b594577bcd");
    // printf("buffer: %s\n", buffer);

    char *encoded_param = url_encode(buffer);
    // printf("encoded_param: %s\n", encoded_param);
    char *encoded_data = url_encode(data);
    // printf("encoded_data: %s\n", encoded_data);
    // char buf2[1024] = {0};
    uint16_t buf_len = sizeof(UPLOAD_BODY1) + strlen("bc85460d") + strlen(resName) + strlen(encoded_param) + strlen(encoded_data) + 1;

    printf("buf_len: %d\n", buf_len);
    char *buf2 = calloc(buf_len, 1);
    // sprintf(buf2, UPLOAD_BODY1, "bc85460d", resName, encoded_param, encoded_data);
    snprintf(buf2, buf_len, UPLOAD_BODY1, "bc85460d", resName, encoded_param, encoded_data);
    printf("buf2: %s, buf2_len: %d\n", buf2, strlen(buf2));

    free(encoded_param);
    free(encoded_data);
    free(buf2);
#endif

Error:
    if (json_buf) {
        free(json_buf);
        json_buf = NULL;
    }

    if (root) {
        cJSON_Delete(root);
        root = NULL;
    }

    return ret;
}

static int listen_create_upload_aiui(const uint8_t *data, uint32_t data_len, uint8_t type)
{
    uint8_t *base64_data = NULL;
    uint32_t base64_len = 0;
    int out_len = 0;
    int ret = -1;

    const char *smartH_list[] = {
        "IFLYTEK.smartH_deviceAlias",
        "IFLYTEK.smartH_deviceScene",
    };

    // base64_len = base64EncodeGetLength(data_len);
    base64_len = 4 * ((data_len + 2) / 3) + 1;
    base64_data = calloc(base64_len, sizeof(char));
    REQUIRE(base64_data == NULL, Error);
    base64_encode((const char *)data, data_len, base64_data);
    printf("base64_encode: %s\n", base64_data);

    ret = listen_up_Sync_devices_or_scenes(smartH_list[type], (const char *)base64_data);

Error:
    if (base64_data)
        free(base64_data);

    return ret;
}

static void base64_test_data(char *outdata)
{
    const char *scene[] = {
        "1我在家了",
        "2我在家了2",
        "3我在家了3",
        "4我在家了4",
        "5我上班了5",
        "6我在学校了",
        "7游戏开始",
        "8我上班了1",
        "9我在学校了2",
        "10游戏开始3",
        "11我上班了4",
        "12我在学校了2",
        "13游戏开始3",
        "14我上班了6",
        "15我在学校7了",
        "16游戏开始7"};
    cJSON *root = NULL;
    char *buf = NULL;
    char *data = NULL;
    uint8_t *base64_data = NULL;
    // uint8_t *resName[32] = {0};
    uint32_t data_len = 0;
    uint8_t size = 16;
    int i = 0;

    char *sbuf = "{\"scene\":\"%s\",\"device\":\"group\", \"pd\": \"haier|haier-dragon\"}\n";
    // char *dbuf = "{\"did\":\"\",\"device\":\"\",\"alias\":\"\"}\n";
    // char *dbuf = "{\"did\":\"%s\",\"device\":\"%s\",\"alias\":\"%s\"}\n";

    // 计算长度
#if 0
    if (size > MAX_SCENE_DEVICE_SIZE)
        data_len = (strlen(sbuf) + MAX_SCENE_NAME_LENGTH) * MAX_SCENE_DEVICE_SIZE;
    else
        data_len = (strlen(sbuf) + MAX_SCENE_NAME_LENGTH) * size;
#else
    data_len = (strlen(sbuf) + MAX_SCENE_NAME_LENGTH) * size;
#endif

    printf("jbuf len = %d\n", data_len);
    buf = data = (char *)calloc(data_len, sizeof(char));

    int remain = size;
    int sub = 0;
    int offset = 0;

#if 0
    while (remain > 0) {
        sub = (remain >= MAX_SCENE_DEVICE_SIZE ? MAX_SCENE_DEVICE_SIZE : remain);
        for (i = 0; i < sub; i++) {
            char buff[60] = {0};
            snprintf(buff, sizeof(buff), sbuf, scene[i + offset]);
            data += snprintf(data, sizeof(buff), "%s", buff);
        }
        printf("buf[%d] : %s", strlen(buf), buf);
        listen_create_upload_aiui((const uint8_t *)buf, strlen(buf), 1);
        memset(buf, 0, data_len);
        data = buf;
        remain -= sub;
        offset += sub;
    }
#else
    for (i = 0; i < size; i++) {
        char buff[80] = {0};
        snprintf(buff, sizeof(buff), sbuf, scene[i]);
        data += snprintf(data, sizeof(buff), "%s", buff);
    }
    printf("buf[%d] : %s", strlen(buf), buf);
    listen_create_upload_aiui((const uint8_t *)buf, strlen(buf), 1);
#endif

    free(buf);
}

static uint16_t get_devices_size(btActionMsg *data_action, uint16_t size)
{
    uint16_t i = 0;
    uint16_t sum = 0;

    if (size == 0)
        return 0;

    for (i = 0; i < size; i++) {
        btActionMsg *action = &data_action[i];
        if (action->dp_size == 0 || action->dp_size == 1) {
            sum += 1;
        }
        else {
            sum += action->dp_size;
        }
    }
    return sum;
}

static void base64_test_data1(char *outdata)
{
    cJSON *root = NULL;
    char *buf = NULL;
    char *data = NULL;
    uint8_t *base64_data = NULL;
    // uint8_t *resName[32] = {0};
    uint32_t data_len = 0;
    uint8_t size = 4;
    int i = 0, j = 0;
    uint16_t devices_size = 0;

    // char *sbuf = "{\"scene\":\"%s\",\"device\":\"group\"}\n";
    // char *dbuf = "{\"did\":\"\",\"device\":\"\",\"alias\":\"\"}\n";
    char *dbuf = "{\"did\":\"%s\",\"device\":\"%s\",\"alias\":\"%s\"}\n";

    btActionMsg *data_action = fill_data_test();
    uint32_t one_length = strlen(dbuf) + MAX_DEVICE_NAME_LENGTH + MAX_DEVICE_ID_LENGTH + MAX_DEVICE_TYPE_LENGTH;

    // 计算长度
#if 0
    if (size > MAX_SCENE_DEVICE_SIZE1)
        data_len = one_length * MAX_SCENE_DEVICE_SIZE1 * 4;
    else
        data_len = one_length * size * 4;
#else
    devices_size = get_devices_size(data_action, size);
    printf("devs size = %d\n", devices_size);
    data_len = one_length * devices_size;
#endif

    printf("jbuf len = %d\n", data_len);
    buf = data = (char *)calloc(data_len, sizeof(char));

    int remain = size;
    int sub = 0;
    int offset = 0;
#if 0
    while (remain > 0) {
        sub = (remain >= MAX_SCENE_DEVICE_SIZE1 ? MAX_SCENE_DEVICE_SIZE1 : remain);
        for (i = 0; i < sub; i++) {
            char buff[112] = {0};
            btActionMsg *action = &data_action[i + offset];
            if (action->dp_size > 0) {
                for (j = 0; j < action->dp_size; j++) {
                    snprintf(buff, sizeof(buff), dbuf, action->deviceId, "switch", action->dp[j].v_string);
                    data += snprintf(data, sizeof(buff), "%s", buff);
                    memset(buff, 0, sizeof(buff));
                }
            }
            else {
                if (strncmp(action->devicePid, "h536ubqx1tui1jsj", strlen("h536ubqx1tui1jsj")) == 0)
                    snprintf(buff, sizeof(buff), dbuf, action->deviceId, "curtain", action->deviceName);
                else
                    snprintf(buff, sizeof(buff), dbuf, action->deviceId, "light", action->deviceName);
                data += snprintf(data, sizeof(buff), "%s", buff);
            }
        }

        printf("buf[%d] : %s", strlen(buf), buf);
        listen_create_upload_aiui((const uint8_t *)buf, strlen(buf), 0);
        memset(buf, 0, data_len);
        data = buf;
        remain -= sub;
        offset += sub;
    }
#else
    for (i = 0; i < 4; i++) {
        char buff[112] = {0};
        btActionMsg *action = &data_action[i];
        if (action->dp_size > 0) {
            for (j = 0; j < action->dp_size; j++) {
                printf("id: %s, name: %s\n",  action->deviceId, action->dp[j].v_string);
                snprintf(buff, sizeof(buff), dbuf, action->deviceId, "switch", action->dp[j].v_string);
                data += snprintf(data, sizeof(buff), "%s", buff);
                memset(buff, 0, sizeof(buff));
            }
        }
        else {
            printf("id: %s, name: %s\n",  action->deviceId, action->deviceName);
            if (strncmp(action->devicePid, "h536ubqx1tui1jsj", strlen("h536ubqx1tui1jsj")) == 0)
                snprintf(buff, sizeof(buff), dbuf, action->deviceId, "curtain", action->deviceName);
            else
                snprintf(buff, sizeof(buff), dbuf, action->deviceId, "light", action->deviceName);
            data += snprintf(data, sizeof(buff), "%s", buff);
        }
    }
    printf("buf[%d] : %s", strlen(buf), buf);
    listen_create_upload_aiui((const uint8_t *)buf, strlen(buf), 0);
#endif

    if (buf)
        free(buf);
    if (data_action)
        free(data_action);
}

int main(int argc, char const *argv[])
{
#if 0
    cJSON *root = NULL;
    char *json_buf = NULL;
    // cJSON *root = NULL;
    cJSON *param = NULL;
    char *buffer = "appid=bc85460d&res_name=IFLYTEK.telephone_contact&pers_param=%7B%22auth_id%22%3A";

    root = cJSON_CreateObject();
    param = cJSON_CreateObject();

    cJSON_AddStringToObject(param, "res_name", "resName");
    cJSON_AddStringToObject(param, "id_name", "uid");
    cJSON_AddStringToObject(param, "id_name", "");
    cJSON_AddItemToObject(root, "param", param);
    cJSON_AddStringToObject(root, "data", "data");
    // char *json_buf = NULL;
    // root = cJSON_CreateObject();

    // cJSON_AddStringToObject(root, "gid", "01");
    // cJSON *cids_item = cJSON_AddArrayToObject(root, "cids");
    // // cJSON_AddStringToObject(cids_item, "cids", "haj122");
    json_buf = cJSON_PrintUnformatted(root);
    printf("json_buf: %s\n", json_buf);

    char *test = NULL;
    cJSON *rname = cJSON_GetObjectItem(param, "res_name");\
    test = rname->valuestring;
    printf("test : %s\n", test);
    // char buf1[3] = {0x7b, 0x22};
    // printf("buffer: %s\n", buf1);

    if (json_buf)
        free(json_buf);
    if (root)
        cJSON_Delete(root);
#endif

    // base64_test_data(NULL);
    // base64_test_data1(NULL);

    // char str[] = "9aset";
    // int data = atoi(str);
    // char tts_buf[512] = {0};
    // printf("str = %s, %d\n", str, isdigit(str[0]));
    // char *devName = "廊灯";
    // char *action_v = "打开";
    // sprintf(tts_buf, "正在为您将%s%s", devName, action_v);
    // printf("tts_buf = %s\n", tts_buf);
    // test_idx(19);
    // char *zhongwen = "无线ssid中文测试";
    // printf("size = %d\n", sizeof(about_info) / sizeof(about_info[0]));
    // for(int i = 0; i < sizeof(about_info) / sizeof(about_info[0]); i++)
    //     printf("%s\n", about_info[i]);
    // char utf8[] = "路由";
    // for (size_t i = 0; i < strlen(utf8); i++)
    // {
    // printf("%d\n", strlen(zhongwen));
    // }

    // uint32_t val = 0xffffff70;

    // printf("val = 0x%08x\n", val);
    // val &= ~(1 << 6);
    // printf("val = 0x%08x\n", val);
#if 0
    uint32_t total_size = 530;
    uint16_t packets = 0, i = 0;
    uint16_t tr_size = 256;
    uint32_t last_packet_size = 0;

    packets = total_size / tr_size;
    last_packet_size = total_size % tr_size;

    if(last_packet_size > 0)
        packets += 1;
    printf("total_size = %d, packets = %d\n", total_size, packets);

    // printf("send first packet [%d]\n", i++);
    printf("send data packet [%d], offset = %d\n", i, i * tr_size);
    i++;

    while (i < packets) {
        // i++;
        // if(i == 0)
        //     printf("send data packet [%d], offset = %d\n", i, i * tr_size);
        // else {
            // if(i == (packets -1 ))
            //     printf("send data packet end [%d], offset = %d\n", i, (i - 1) * tr_size + last_packet_size );
            // else
                printf("send data packet [%d], offset = %d\n", i, i * tr_size );
        // }

        i += 1;
    }
    printf("send data packet [%d], offset = %d\n", i, (i - 1) * tr_size + last_packet_size);
#endif

#if 0
    uint8_t ctrl_data[10] = {0};
    uint8_t *cdata = (uint8_t *)ctrl_data;
    int i = 0;
    uint8_t num = 0;
    int len = 0;

    // memset(cdata, 0x55, sizeof(cdata));
    int test = 100;
    // memcpy(cdata, &test, sizeof(test));
    len = 4;
    for (i = len - 1; i >= 0; i--) {
        num = BREAK_UINT32(test, i);
        printf("cdata = 0x%02x\n", num);
        // cdata++;
    }

    len = 2;
    printf("===========\n");
    while(len-- != 0) {
        // len--;
        num = BREAK_UINT32(test, len);
        printf("cdata = 0x%02x\n", num);
    }

    float tmp = 3.4 * 73;
    printf("tmp = %d\n", (int)tmp);
#endif
    // _leshi_dp_t ctrl_dp;
    // uint8_t *data = NULL;

    // memset(&ctrl_dp, 0, sizeof(ctrl_dp));

    // ctrl_dp.value = calloc(12, sizeof(uint8_t));
    // ctrl_dp.value[0] = 0;

    // // printf("data = %d\n", (uint32_t)ctrl_dp.value);
    // _Bool key =  *ctrl_dp.value;
    // printf("data = %d\n", key);

    // free(ctrl_dp.value);
    // int i = 0;
    // for(i = 0; i < 7; i++)
    //     printf("%s\n", deviceIds[i]);

    // for (i = len - 1; i >= 0; i--) {
    //     num = BREAK_UINT32(test, i);
    //     printf("cdata = 0x%02x\n", num);
    //     // cdata++;
    // }
    // len = 1;
    // printf("===========\n");
    // for (i = len - 1; i >= 0; i--) {
    //     num = BREAK_UINT32(test, i);
    //     printf("cdata = 0x%02x\n", num);
    //     // cdata++;
    // }

    // num = BREAK_UINT32(test, 0);
    // printf("cdata = 0x%02x\n", num);
    // num = (uint8_t) test;
    // printf("cdata = 0x%02x\n", num);

    // for (i = 0; i < 10; i++) {
    //     printf("0x%02x\n", ctrl_data[i]);
    // }
    // char buffer[256] = {0};
    // _cjson_test2();

    // sprintf(buffer, "%d", 12);
    // printf("%s, %d\n", buffer, strlen(buffer));

    // _cjson_test4();
    // char buffer[] = "43981";

    // printf("%s, %d\n", buffer, strlen(buffer));
    // for(int i = 0; i < strlen(buffer); i++) {
    //     printf("0x%02x\n", buffer[i]);
    // }
#if 1
    cJSON *root = cJSON_Parse(json_buf);
    if(root == NULL)
    {
        printf("json format err\n");
    }else{
        printf("json parse successful\n");
        // cJSON *list_js = cJSON_GetObjectItem(root, "list");
        // char *strll = cJSON_PrintUnformatted(list_js);
        cJSON *name_js = cJSON_GetObjectItem(root, "name");
        printf("%s, %d\n", name_js->valuestring, strlen(name_js->valuestring));
        // free(strll);
    }

    cJSON_Delete(root);
#endif

    // cJSON *root = cJSON_CreateArray();

    // cJSON_AddItemToArray(root, cJSON_CreateString("hello"));
    // cJSON_AddItemToArray(root, cJSON_CreateString("hello"));
    // cJSON_AddItemToArray(root, cJSON_CreateString("hello"));

    // char *str = cJSON_PrintUnformatted(root);
    // printf("%s\n", str);
    // free(str);

    // cJSON_Delete(root);
    
    return 0;
}
