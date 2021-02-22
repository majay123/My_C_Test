#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

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

#define MULTI_CURL_NUM      3
#define TIME_OUT_MS         1000 * 2
#define URL                 "http://192.168.2.9:8080/hopeApi/music/listRank"

/* resizable buffer */
typedef struct {
  char *buf;
  size_t size;
} memory;

typedef struct {
    char Rsp[4096];
    CURL *Curl_handle;
}curl_Rsp;

#if 0
static char *_get_json_prase(void)
{
    print_mcd("this func is get json prase");
    cJSON *root = NULL;
    char *test;
    root = cJSON_CreateObject();
    if(root == NULL)
        return NULL;
    cJSON_AddStringToObject(root, "test", "hello");

    
    test = cJSON_Print(root);
    print_mcd("this is curl test!!");
    // print_mcd("get test = %s\r\n", test);
    if(root)
        cJSON_Delete(root);

    return test;
}
#endif

static size_t curl_writer(void *buffer, size_t size, size_t count, void *stream)
{
    print_mcd("get %s", (char *)buffer);
    cJSON *root = cJSON_Parse(buffer);
    if(root == NULL) print_mcd("parse json failed");
    return size * count;
}

/**
* @details  	create curl handler 
* @param[in]	char *sUrl, char *sProxy, char * sRsp, unsigned int uiTimeout
* @param[out]	CURL *handler
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 202011181108 V1.0.0				MCD				create
*/
CURL *curl_easy_handler(char *sUrl, char *sProxy, char * sRsp, unsigned int uiTimeout, char *date)
{
    CURL *curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, sUrl);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

    if(uiTimeout > 0)
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, uiTimeout);

    if(sProxy)
        curl_easy_setopt(curl, CURLOPT_PROXY, sProxy);

    //write function
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, date);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &sRsp);

    return curl;
}

/**
* @details  	使用select函数监听multi curl文件描述符的状态，监听成功返回0，监听失败返回-1
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 202011181109 V1.0.0				MCD				create
*/
int curl_multi_select(CURLM * curl_m)
{
    CURLcode res;

    struct timeval timeout_tv;
    fd_set fd_read;
    fd_set fd_write;
    fd_set fd_except;
    int max_fd = -1;

    if(curl_m == NULL)
    {
        print_mcd("curl_m is null");
        return -1;
    }
    /* 注意这里一定要清空fdset， curl_multi_fdset不会执行fdset的清空操作 */
    FD_ZERO(&fd_read);
    FD_ZERO(&fd_write);
    FD_ZERO(&fd_except);

    /* 设置select超时时间 */
    timeout_tv.tv_sec = 1;
    timeout_tv.tv_usec = 0;

    /* 获取multi curl需要监听的文件描述符集合 fd_set */
    res = curl_multi_fdset(curl_m, &fd_read, &fd_write, &fd_except, &max_fd);
    print_mcd("fdset curl code  = %d, max_fd = %d", res, max_fd);
    /** 
     * When max_fd returns with -1, 
     * you need to wait a while and then proceed and call curl_multi_perform anyway. 
     * How long to wait? I would suggest 100 milliseconds at least, 
     * but you may want to test it out in your own particular conditions to find a suitable value. 
     */ 
    // print_mcd("fd_read = %d, fd_write = %d, fd_except = %d, max_fd = %d", fd_read, fd_write, fd_except, max_fd);
    if(-1 == max_fd)
    {
        print_mcd("get max_fd = %d", max_fd);
        struct timeval wait = { 0, TIME_OUT_MS }; /* 2ms */
        select(0, NULL, NULL, NULL, &wait);
        // return -1;
    }

    /** 
     * 执行监听，当文件描述符状态发生变化的时候返回
     * 返回0，持续调用curl_mutli_perform通过curl执行相应的操作 
     * 返回-1，表示select错误
     * 注意：即使select超时也需要返回0，具体可以看官网文档说明
     */ 
    int ret_code = select(max_fd + 1, &fd_read, &fd_write, &fd_except, &timeout_tv);
    if(-1 == ret_code)
    {
        print_mcd("select error ");
        return -1;
    }
    else if(0 ==  ret_code)
    {
        print_mcd("select success ");
    }
    
    return 0;

}


char test_date[1024] = "ver=1.0&des=06&cid=750064224428658688&sid=750837261197414400&key=A716A953593940D2BD78E1A02CD3C070&len=58&dat=%7B%22tokenId%22%3A%22ZGE3M2xlbGlCMEM0bk9sN1BYS0ZYRWR3cVRBVzRxU0E%3D%22%7D";
/* 这里设置自己说需要访问的url */
int curl_multi_demo(int num)
{
    /* 初始化一个multi curl对象 */
    CURLM *curl_m = curl_multi_init();
    curl_Rsp curl_ml[num];

    /* 设置easy curl对象并添加multi curl对象中 */
    for(int i = 0; i < num; ++i)
    {
        memset(&curl_ml[i], 0, sizeof(curl_Rsp));
        curl_ml[i].Curl_handle = curl_easy_handler(URL, NULL, (char *)curl_ml[i].Rsp, TIME_OUT_MS, test_date);
        if(curl_ml[i].Curl_handle == NULL)
        {
            print_mcd("get curl handle error");
            return -1;
        }
        curl_multi_add_handle(curl_m, curl_ml[i].Curl_handle);
    }

    /* 
     * 调用curl_multi_perform函数执行curl请求 
     * url_multi_perform返回CURLM_CALL_MULTI_PERFORM时，表示需要继续调用该函数直到返回值不是CURLM_CALL_MULTI_PERFORM为止 
     * running_handles变量返回正在处理的easy curl数量，running_handles为0表示当前没有正在执行的curl请求 
     */  
    int running_handles = 0;
    /* 执行并发请求，非阻塞，立即返回 */
    while(CURLM_CALL_MULTI_PERFORM == curl_multi_perform(curl_m, &running_handles))
        print_mcd("running handle = %d",running_handles);

    /* 为了避免循环调用curl_multi_perform产生的cpu持续占用的问题，采用select来监听文件描述符 */
    // print_mcd("running handle = %d",running_handles);
    while(running_handles)
    {
        if(-1 == curl_multi_select(curl_m))
        {
            print_mcd("select error");
            break;
        }
        else
        {
            /* select监听到事件，调用curl_multi_perform通知curl执行相应的操作 */
            while (CURLM_CALL_MULTI_PERFORM == curl_multi_perform(curl_m, &running_handles))
            {
                print_mcd("select running handles = %d", running_handles);
            }
        }
        // print_mcd("select running handles = %d", running_handles); 
    }

    /* 输出执行结果 */
    int msgs_left = 0;
    CURLMsg *msg;

    while((msg = curl_multi_info_read(curl_m, &msgs_left)))
    {
        if(CURLMSG_DONE == msg->msg)
        {
            int idx = 0;
            for(idx = 0; idx < num; ++idx)
            {
                if(msg->easy_handle == curl_ml[idx].Curl_handle)    break;
            }

            if(idx == num)
            {
                print_mcd("curl not found");
            }
            else
            {
                print_mcd("curl [ %d ] complete with status %d", idx, msg->data.result);
                print_mcd("rsp: %s\n", curl_ml[idx].Rsp);
            }
        }
    }

    /* 这里要注意cleanup的顺序 */
    for(int i = 0; i < num; ++i)    
        curl_multi_remove_handle(curl_m, curl_ml[i].Curl_handle);
    
    for(int i = 0; i < num; ++i)    
        curl_easy_cleanup(curl_ml[i].Curl_handle);

    curl_multi_cleanup(curl_m);
    
    return 0;

}

/**
* @details  	curl easy dem0
* @param[in]	int num
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 202011181658 V1.0.0				MCD				create
*/  
int curl_easy_demo(int num)
{
    char rsp_buffer[1024] = {0};

    for(int i = 0; i < num; ++i)
    {
        memset(rsp_buffer, 0, sizeof(rsp_buffer));
        CURL *curl = curl_easy_handler(URL, NULL, (char *)rsp_buffer, TIME_OUT_MS, test_date);
        CURLcode code = curl_easy_perform(curl);
        print_mcd("curl [ %d ] complete with status code = %s", code, rsp_buffer);
        print_mcd("curl rsp = %s", rsp_buffer);
        curl_easy_cleanup(curl);
    }
    return 0;
}


#define USE_MULTI_CURL

struct timeval begin_tv, end_tv;

int main(int argc, char const *argv[])
{
    if(argc < 2)
    {
        print_mcd("Usage: argc = %d", argc);
        return -1;
    }
    int num = atoi(argv[1]);
    print_mcd("get num: %d", num);
    /* 获取开始时间 */
    gettimeofday(&begin_tv, NULL);

#ifdef USE_MULTI_CURL
    curl_multi_demo(num);
#else   
    curl_esey_demo(num);
#endif // USE_MULTI_CURL

    /* 获取结束时间 */
    gettimeofday(&end_tv, NULL);

    /* 计算执行延时并输出，用于比较 */
    int eclapsed = (end_tv.tv_sec - begin_tv.tv_sec) * 1000 + (end_tv.tv_usec - begin_tv.tv_usec) / 1000;

    print_mcd("eclapsed time: %d ms", eclapsed);

    return 0;
}

