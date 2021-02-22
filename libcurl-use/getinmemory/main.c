#include "main.h"

#include <curl/curl.h>
#include <curl/easy.h>
#include "cJSON.h"

#define PRINT_MCD_ENABLE  1

#if PRINT_MCD_ENABLE == 1
    #define print_mcd(format, arg...)   do { printf("\033[31m[----mcd----]\033[0m:%s,%d--- "format"\n",__FILE__,__LINE__, ## arg);}\
                                        while (0)
#else
    #define print_mcd(format, arg...)   do {} while (0)
#endif


#define URL     "http://192.168.2.9:8080/hopeApi/music/listRank"
#define URL1    "http://192.168.2.9:8080//hopeApi/device/register"

/* resizable buffer */
typedef struct {
  char *memory;
  size_t size;
} MemoryStruct;

char JsonDate[1024] = "ver=1.0&des=06&cid=750064224428658688&sid=750837261197414400&key=A716A953593940D2BD78E1A02CD3C070&len=58&dat=%7B%22tokenId%22%3A%22ZGE3M2xlbGlCMEM0bk9sN1BYS0ZYRWR3cVRBVzRxU0E%3D%22%7D";


char JsonDate1[1024] = "ver=1.0&des=73&cid=750064224428658688&sid=750837261197414400&key=A716A953593940D2BD78E1A02CD3C070&len=354&dat=%7B%22comName%22%3A%22HOPE%22%2C%22deviceSN%22%3A%22710038190600555%22%2C%22deviceName%22%3A%22HOPE-Z4%22%2C%22deviceCata%22%3A%22Z4-MIGU%22%2C%22firmVersion%22%3A%22ats3609-debug-0.1%22%2C%22softVersion%22%3A%220.0.1%22%2C%22macAddress%22%3A%2230%3A4A%3A26%3AB2%3AE6%3AF4%22%2C%22playerType%22%3A%22_test%22%2C%22playerVersion%22%3A%221.0%22%2C%22bgType%22%3A%22master%22%2C%22bgVersion%22%3A%221.2.3%22%2C%22upgradeChannel%22%3A%22release-HOPE-Z4%22%2C%22deviceDrive%22%3A%22ats3609%22%2C%22parentId%22%3A%22753396045774098432%22%7D";




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


static size_t WriteMemoryCallback(void *contents, size_t size, size_t number, void * userp)
{
    size_t realsize = size * number;
    MemoryStruct *mem = (MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL)
    {
        print_mcd("not enough memory (realloc returned NULL)");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    print_mcd("realsize = %ld\n", realsize);
    return realsize;
}


int main(int argc, char *argv[])
{
    CURLcode res;
    CURL *curl_handle;

    MemoryStruct chunk;

    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_ALL);

    curl_handle = curl_easy_init();

    curl_easy_setopt(curl_handle, CURLOPT_URL, URL);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, JsonDate);
    curl_easy_setopt(curl_handle, CURLOPT_POST, 1);

    res = curl_easy_perform(curl_handle);

    if(res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }
    else
    {
        print_mcd("%lu bytes retrieved", (unsigned long)chunk.size);
        print_mcd("get date: %s", chunk.memory);
        cJSON *root = cJSON_Parse(chunk.memory);
        if(root == NULL) print_mcd("parse json failed");
        else print_mcd("parse json success");
    }
        

    curl_easy_cleanup(curl_handle);
    free(chunk.memory);
    curl_global_cleanup();

    return 0;
}