#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <curl/curl.h>
// #include <curl/types.h>
#include <curl/easy.h>
#include "cJSON.h"

/* resizable buffer */
typedef struct {
  char *buf;
  size_t size;
} memory;


static char *_get_json_prase(void)
{
    printf("this func is get json prase\r\n");
    cJSON *root = NULL;
    char *test;
    root = cJSON_CreateObject();
    if(root == NULL)
        return NULL;
    cJSON_AddStringToObject(root, "test", "hello");

    
    test = cJSON_Print(root);
    printf("this is curl test!!\r\n");
    // printf("get test = %s\r\n", test);
    if(root)
        cJSON_Delete(root);

    return test;
}

static size_t write_data1(void *ptr, size_t size, size_t nmemb, void *stream)  
{
    // int written = fwrite(ptr, size, nmemb, (FILE *)fp);
    int written = nmemb;
    // printf("get data  = %s\n", ptr);
    return written;
}

static size_t grew_buffer(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t realsize = nmemb * size;
    char *mem = (char *)stream;
    char *tmp_ptr = realloc(mem, realsize);
    if(!tmp_ptr)
    {
        printf("not enough memory (realloc returned  null!!)\n");
        return 0;
    }
    printf("size = %d, date = %s\n",realsize, ptr);
    memcpy(mem, ptr, realsize);

    return realsize;

}

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)  
{
    // int written = fwrite(ptr, size, nmemb, (FILE *)fp);
    int written = nmemb;
    CURL *curl;
    CURLcode code;
    char JsonDate[1024] = "ver=1.0&des=06&cid=750064224428658688&sid=750837261197414400&key=A716A953593940D2BD78E1A02CD3C070&len=58&dat=%7B%22tokenId%22%3A%22ZGE3M2xlbGlCMEM0bk9sN1BYS0ZYRWR3cVRBVzRxU0E%3D%22%7D";
    printf("size = %d\n",written);
    printf("get data  = %s\n", ptr);

    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.2.9:8080/hopeApi/music/listRank");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, JsonDate);
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    code = curl_easy_perform(curl);
    if(CURLE_OK == code)
    {
        char *ct;
        long res_code=0;
        printf("success!!!\n");
    }
    curl_easy_cleanup(curl);
    return written;
}


int main(int argc, char *argv[])
{
    char *test;

    CURLcode code;
    CURL *curl;
    // char JsonDate[1024] = "{\"cid\":750064224428658688,\"dat\":\"{\"tokenId\":\"ZGE3M2xlbGlCMEM0bk9sN1BYS0ZYRWR3cVRBVzRxU0E=\"}\",\"des\":\"06\",\"key\":\"A716A953593940D2BD78E1A02CD3C070\",\"len\":58,\"sid\":750837261197414400,\"ver\":1.0}";
    
    char JsonDate1[1024] = "ver=1.0&des=73&cid=750064224428658688&sid=750837261197414400&key=A716A953593940D2BD78E1A02CD3C070&len=354&dat=%7B%22comName%22%3A%22HOPE%22%2C%22deviceSN%22%3A%22710038190600555%22%2C%22deviceName%22%3A%22HOPE-Z4%22%2C%22deviceCata%22%3A%22Z4-MIGU%22%2C%22firmVersion%22%3A%22ats3609-debug-0.1%22%2C%22softVersion%22%3A%220.0.1%22%2C%22macAddress%22%3A%2230%3A4A%3A26%3AB2%3AE6%3AF4%22%2C%22playerType%22%3A%22_test%22%2C%22playerVersion%22%3A%221.0%22%2C%22bgType%22%3A%22master%22%2C%22bgVersion%22%3A%221.2.3%22%2C%22upgradeChannel%22%3A%22release-HOPE-Z4%22%2C%22deviceDrive%22%3A%22ats3609%22%2C%22parentId%22%3A%22753396045774098432%22%7D";
    char JsonDate[1024] = "ver=1.0&des=06&cid=750064224428658688&sid=750837261197414400&key=A716A953593940D2BD78E1A02CD3C070&len=58&dat=%7B%22tokenId%22%3A%22ZGE3M2xlbGlCMEM0bk9sN1BYS0ZYRWR3cVRBVzRxU0E%3D%22%7D";

    char writedate[1024] = { 0 };

    // memory *mem;
    memory *mem = malloc(sizeof(memory));
    mem->size = 0;
    mem->buf = malloc(1);

    test = calloc(1, 10240);
    if(test == NULL)
    {
        printf("calloc failed\r\n");
        return -1;
    }
    code = curl_global_init(CURL_GLOBAL_ALL);
    printf("get curl code  = %d\n", code);
    if(code != CURLE_OK)
    {
        printf("cur global init failed!!\n");
        return -1;
    }
    
    curl = curl_easy_init();
    if(curl)
    {
         // test = curl_version();
        printf("get curl version = %s\n", curl_version());
        
        // curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.2.9:8080//hopeApi/device/register");
        curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.2.9:8080/hopeApi/music/listRank");
        
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &grew_buffer);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, test);
        curl_easy_setopt(curl, CURLOPT_PRIVATE, test);
        
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, JsonDate);
        curl_easy_setopt(curl, CURLOPT_POST, 1);

        code = curl_easy_perform(curl);
        // printf("\nsuccess %d!!!\n", code);
        if(CURLE_OK == code)
        {
            char *ct;
            long res_code=0;
            // printf("success!!!\n");
            char *mem1;
            curl_easy_getinfo(curl, CURLINFO_PRIVATE, &mem1);
            // printf("get buf = %s\n", mem1);
            code = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &res_code);
            printf("\nreceivede Content-Type: %ld\n", res_code);
            if(200 == res_code)
            {
                // printf("receivede Content-Type: %s\n", ct);
                printf("get write date = %s\n", mem1);
                
            }
        }
        curl_easy_cleanup(curl);
        free(test);
    }
   
    curl_global_cleanup();
    
    return 0;
}