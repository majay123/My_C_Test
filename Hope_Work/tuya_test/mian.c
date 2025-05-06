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
 * ************Copyright 2022 MCD************
 * 
 * @version      : 
 * @Company      : HOPE
 * @Author       : MCD
 * @Date         : 2022-02-21 10:01:18
 * @LastEditors  : MCD
 * @LastEditTime : 2024-12-06 11:28:39
 * @FilePath     : /My_C_Test/Hope_Work/tuya_test/mian.c
 * @Description  : 
 * 
 * ******************************************
 */

#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <time.h>
#include <mbedtls/md.h>
#include <mbedtls/platform.h>

#include "MQTTClient.h"
#include "cJSON.h"

#define TUYA_MQTT_CLIENTID_MAXLEN  (128U)
#define TUYA_MQTT_USERNAME_MAXLEN  (128U)
#define TUYA_MQTT_PASSWORD_MAXLEN  (96U)
#define TUYA_MQTT_DEVICE_ID_MAXLEN (32U)
#define TUYA_MQTT_TOPIC_MAXLEN     (64U)
#define TUYA_MQTT_TOPIC_MAXLEN     (64U)

#define BROKER_ADDRESS  "ssl://m1.tuyacn.com:8883"
#define TOPIC_SUBSCRIBE "tylink/%s/thing/property/set"
#define TIMEOUT         10000L  // 10秒超时
#define QOS             1

#define CA_CERTIFICATE_FILE "/home/mcd/MyWorkSpace/MyGithub_Project/My_C_Test/Hope_Work/tuya_test/cacert.pem"

typedef struct {
    char clientid[TUYA_MQTT_CLIENTID_MAXLEN];
    char username[TUYA_MQTT_USERNAME_MAXLEN];
    char password[TUYA_MQTT_PASSWORD_MAXLEN];
} tuya_mqtt_auth_t;

// {"code":100000,"desc":"success!","message":"成功！","object":{"bindCode":"2VqbSFA9","broker":"ssl://m1.tuyacn.com:8883","createTime":1705398124903,"createUser":765016147543318528,"delState":false,"deviceId":"265a27655eec45c673glam","deviceRegion":"CN","deviceSN":"50140406070131","deviceSecret":"qgLi8FTRqG77hmxb","productId":"o7y47gcwsddctawy","qrCode":"https://m.smart321.com/AYWx9Y0bDniFiYTH","refrenceId":1573157668024791040,"registerId":"ZVe5RYqbY7Tx6UJcXcrT","updateTime":1733215075106,"updateUser":765016147543318528}}


const char tuya_cacert_pem[] = {\
"-----BEGIN CERTIFICATE-----\n"\
"MIIDxTCCAq2gAwIBAgIBADANBgkqhkiG9w0BAQsFADCBgzELMAkGA1UEBhMCVVMx\n"\
"EDAOBgNVBAgTB0FyaXpvbmExEzARBgNVBAcTClNjb3R0c2RhbGUxGjAYBgNVBAoT\n"\
"EUdvRGFkZHkuY29tLCBJbmMuMTEwLwYDVQQDEyhHbyBEYWRkeSBSb290IENlcnRp\n"\
"ZmljYXRlIEF1dGhvcml0eSAtIEcyMB4XDTA5MDkwMTAwMDAwMFoXDTM3MTIzMTIz\n"\
"NTk1OVowgYMxCzAJBgNVBAYTAlVTMRAwDgYDVQQIEwdBcml6b25hMRMwEQYDVQQH\n"\
"EwpTY290dHNkYWxlMRowGAYDVQQKExFHb0RhZGR5LmNvbSwgSW5jLjExMC8GA1UE\n"\
"AxMoR28gRGFkZHkgUm9vdCBDZXJ0aWZpY2F0ZSBBdXRob3JpdHkgLSBHMjCCASIw\n"\
"DQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAL9xYgjx+lk09xvJGKP3gElY6SKD\n"\
"E6bFIEMBO4Tx5oVJnyfq9oQbTqC023CYxzIBsQU+B07u9PpPL1kwIuerGVZr4oAH\n"\
"/PMWdYA5UXvl+TW2dE6pjYIT5LY/qQOD+qK+ihVqf94Lw7YZFAXK6sOoBJQ7Rnwy\n"\
"DfMAZiLIjWltNowRGLfTshxgtDj6AozO091GB94KPutdfMh8+7ArU6SSYmlRJQVh\n"\
"GkSBjCypQ5Yj36w6gZoOKcUcqeldHraenjAKOc7xiID7S13MMuyFYkMlNAJWJwGR\n"\
"tDtwKj9useiciAF9n9T521NtYJ2/LOdYq7hfRvzOxBsDPAnrSTFcaUaz4EcCAwEA\n"\
"AaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAQYwHQYDVR0OBBYE\n"\
"FDqahQcQZyi27/a9BUFuIMGU2g/eMA0GCSqGSIb3DQEBCwUAA4IBAQCZ21151fmX\n"\
"WWcDYfF+OwYxdS2hII5PZYe096acvNjpL9DbWu7PdIxztDhC2gV7+AJ1uP2lsdeu\n"\
"9tfeE8tTEH6KRtGX+rcuKxGrkLAngPnon1rpN5+r5N9ss4UXnT3ZJE95kTXWXwTr\n"\
"gIOrmgIttRD02JDHBHNA7XIloKmf7J6raBKZV8aPEjoJpL1E/QYVN8Gb5DKj7Tjo\n"\
"2GTzLH4U/ALqn83/B2gX2yKQOC16jdFU8WnjXzPKej17CuPKf1855eJ1usV2GDPO\n"\
"LPAvTK33sefOT6jEm0pUBsV/fdUID+Ic/n4XuKxe9tQWskMJDE32p2u0mYRlynqI\n"\
"4uJEvlz36hz1\n"\
"-----END CERTIFICATE-----\n"};

/**
* @author  		MCD
* @date  		2024-12-03-14:24
* @details		计算hmac sha256
*/
void compute_hmac_sha256(const unsigned char *key, int key_len, const unsigned char *data, int data_len, unsigned char *out)
{
    // 使用 OpenSSL 提供的 HMAC 计算函数
    HMAC_CTX ctx;
    HMAC_CTX_init(&ctx); ;                       // 创建 HMAC 上下文
    HMAC_Init_ex(&ctx, key, key_len, EVP_sha256(), NULL);  // 初始化 HMAC
    HMAC_Update(&ctx, data, data_len);                     // 更新数据
    unsigned int len = SHA256_DIGEST_LENGTH;
    HMAC_Final(&ctx, out, &len);  // 获取最终结果
    HMAC_CTX_cleanup(&ctx);          // 释放 HMAC 上下文
}

int mbedtls_message_digest_hmac(mbedtls_md_type_t md_type,
                                const uint8_t* key, size_t keylen,
                                const uint8_t* input, size_t ilen, 
                                uint8_t* digest)
    {
    if (key == NULL || keylen == 0 || input == NULL || ilen == 0 || digest == NULL)
        return -1;

    mbedtls_md_context_t md_ctx;
    mbedtls_md_init(&md_ctx);
    int ret = mbedtls_md_setup(&md_ctx, mbedtls_md_info_from_type(md_type), 1);
    if (ret != 0) {
        printf("mbedtls_md_setup() returned -0x%04x\n", -ret);
        goto exit;
    }

    mbedtls_md_hmac_starts(&md_ctx, key, keylen);
    mbedtls_md_hmac_update(&md_ctx, input, ilen);
    mbedtls_md_hmac_finish(&md_ctx, digest);

exit:
    mbedtls_md_free(&md_ctx);
    return ret;
}

static int hmac_sha256_once(const uint8_t* key, const uint8_t* input, size_t ilen, uint8_t* digest)
{
    return mbedtls_message_digest_hmac(MBEDTLS_MD_SHA256, key, 16, input, ilen, digest);
}

void hmac_sha256(const char *key, const char *data, unsigned char *output) {
    mbedtls_md_context_t ctx;
    const mbedtls_md_info_t *info;
    
    mbedtls_md_init(&ctx);
    
    // 获取SHA-256的消息摘要算法信息
    info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);
    
    // 初始化HMAC
    mbedtls_md_setup(&ctx, info, 1);
    mbedtls_md_hmac_starts(&ctx, (unsigned char *)key, strlen(key));
    
    // 输入数据
    mbedtls_md_hmac_update(&ctx, (unsigned char *)data, strlen(data));
    
    // 计算HMAC值
    mbedtls_md_hmac_finish(&ctx, output);
    
    // 清理上下文
    mbedtls_md_free(&ctx);
}


/**
* @author  		MCD
* @date  		2024-12-03-14:24
* @details		获取tuya授权 clientId username password
*/
static int tuya_mqtt_auth_signature_calculate(const char *deviceId, const char *deviceSecret, char *clientID, char *username, char *password)
{
    if (NULL == deviceId || NULL == deviceSecret ||
        NULL == clientID || NULL == username || NULL == password) {
        return -1;
    }
    // 获取当前的时间戳
    time_t timestamp = time(NULL);

    /* client ID */
    sprintf(username, "%s|signMethod=hmacSha256,timestamp=%ld,secureMode=1,accessType=1", deviceId, timestamp);
    printf("username:%s\n", username);

    /* username */
    sprintf(clientID, "tuyalink_%s", deviceId);
    printf("clientID:%s\n", clientID);

    /* password */
    int i = 0;
    uint8_t password_stuff[255] = {0};
    uint8_t digest[32];
    size_t slen = sprintf((char *)password_stuff, "deviceId=%s,timestamp=%ld,secureMode=1,accessType=1", deviceId, timestamp);
    // hmac_sha256_once(deviceSecret, password_stuff, slen, digest);
    compute_hmac_sha256((const uint8_t *)deviceSecret, strlen(deviceSecret), password_stuff, slen, digest);
    for (i = 0; i < 32; i++) {
        sprintf(password + 2 * i, "%02x", digest[i]);
    }
    printf("password:%s\n", password);

    // printf("password_stuff: %s\n", password_stuff);
    // memset(digest, 0, sizeof(digest));
    // hmac_sha256_once((const uint8_t *)deviceSecret, password_stuff, slen, digest);
    // for (i = 0; i < 32; i++) {
    //     sprintf(password + 2 * i, "%02x", digest[i]);
    // }
    // printf("password:%s\n", password);
    // memset(digest, 0, sizeof(digest));
    // hmac_sha256(deviceSecret, password_stuff, digest);
    // for (i = 0; i < 32; i++) {
    //     sprintf(password + 2 * i, "%02x", digest[i]);
    // }
    // printf("password:%s\n", password);

    return 0;
}

// 消息回调函数
int messageArrived(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    printf("Message received -Topic: %s\n", topicName);
    printf("Message content: %s\n", (char *)message->payload);

    cJSON *root = NULL;
    root = cJSON_Parse((const char *)message->payload);

    if(root == NULL) {
        printf("Error parse message\n");
        goto Error;
    }
    
    char *test = cJSON_PrintUnformatted(root);
    printf("test: %s\n", test);
    


Error:
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

// 连接丢失回调
void connectionLost(void *context, char *cause) {
    printf("Connection lost, reason: %s\n", cause);
}


int main() {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc= 0;
    tuya_mqtt_auth_t tuya_mqtt_auth;
    uint8_t dev_id[64] = "265a27655eec45c673glam";
    uint8_t dev_secret[64] = "qgLi8FTRqG77hmxb";

    memset(&tuya_mqtt_auth, 0, sizeof(tuya_mqtt_auth));
    rc = tuya_mqtt_auth_signature_calculate((const char *)dev_id, (const char *)dev_secret, tuya_mqtt_auth.clientid, tuya_mqtt_auth.username, tuya_mqtt_auth.password);
    if (rc != 0) {
        printf("get tuya mqtt auth failed\n");
        return -1;
    }

    // return 0;

    
    // 配置连接参数
    conn_opts.keepAliveInterval = 60;
    conn_opts.cleansession = 1;

    // 设置用户名和密码认证
    conn_opts.username = tuya_mqtt_auth.username;
    conn_opts.password = tuya_mqtt_auth.password;
    // conn_opts.username = "265a27655eec45c673glam|signMethod=hmacSha256,timestamp=1733293816,secureMode=1,accessType=1";
    // deviceId=265a27655eec45c673glam,timestamp=1733293816,secureMode=1,accessType=1
    // conn_opts.password = "56f868abe4630abae96ae5aea0f5bc482f48c9dbe03dd39c9addbd88e0828e7d";

    // MQTTClient_SSLOptions ssl_opts = MQTTClient_SSLOptions_initializer;
    // ssl_opts.enableServerCertAuth = 1;
    // conn_opts.ssl = &ssl_opts;

    // printf("username:%s\n", tuya_mqtt_auth.username);
    // printf("password:%s\n", tuya_mqtt_auth.password);

    MQTTClient_SSLOptions ssl_opts = MQTTClient_SSLOptions_initializer;
    // ssl_opts.enableServerCertAuth = 1;
    // ssl_opts.trustStore = CA_CERTIFICATE_FILE;
    // conn_opts.ssl->trustStore = "/home/mcd/MyWorkSpace/MyGithub_Project/My_C_Test/Hope_Work/tuya_test/cacert.pem";  // 不使用证书文件路径，而是直接通过 SSL_CTX 加载证书
    conn_opts.ssl = &ssl_opts;

    // 创建MQTT客户端
    rc = MQTTClient_create(&client, BROKER_ADDRESS, tuya_mqtt_auth.clientid,
                           MQTTCLIENT_PERSISTENCE_NONE, NULL);
    if (rc != MQTTCLIENT_SUCCESS) {
        printf("Failed to create client, error code: %d\n", rc);
        return -1;
    }

    // 设置回调
    rc = MQTTClient_setCallbacks(client, NULL, connectionLost, messageArrived, NULL);
    if (rc != MQTTCLIENT_SUCCESS) {
        printf("Failed to set callback\n");
        MQTTClient_destroy(&client);
        return -1;
    }
    

    rc = MQTTClient_connect(client, &conn_opts);
    if (rc != MQTTCLIENT_SUCCESS) {
        printf("Connection failed, error code: %d\n", rc);
        switch(rc) {
            case 1: printf("Connection refused: Unacceptable protocol version\n"); break;
            case 2: printf("Connection refused: Reject client ID\n"); break;
            case 3: printf("Connection refused: Server is unavailable\n"); break;
            case 4: printf("Connection refused: Wrong username or password\n"); break;
            case 5: printf("Connection refused: Unauthorized\n"); break;
            default: printf("unknown error\n"); break;
        }
        MQTTClient_destroy(&client);
        return -1;
    }

    printf("Successfully connected to mqtt server\n");

    // 订阅主题
    uint8_t topic_subscribe[256] = {0};

    sprintf((char *)topic_subscribe, TOPIC_SUBSCRIBE, dev_id);
    printf("topic_subscribe:%s\n", topic_subscribe);
    rc = MQTTClient_subscribe(client, (const char *)topic_subscribe, QOS);
    if (rc != MQTTCLIENT_SUCCESS) {
        printf("Subscription failed\n");
        MQTTClient_disconnect(client, TIMEOUT);
        MQTTClient_destroy(&client);
        return -1;
    }

    // 保持连接并处理消息
    while(1) {
        // 在实际应用中，可以添加其他业务逻辑
        MQTTClient_yield();
    }

    // 断开连接（实际上不会执行到这里）
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    
    return 0;
}