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
 * @Date         : 2023-03-30 14:56:37
 * @LastEditors  : MCD
 * @LastEditTime : 2023-03-30 16:31:23
 * @FilePath     : /My_C_Test/ip_weather_test/test.c
 * @Description  : 
 * 
 * ******************************************
 */
#include <curl/curl.h>
#include <json-c/json.h>
#include <stdio.h>
#include <stdlib.h>

/* Your OpenWeatherMap API key */
#define API_KEY "YOUR_API_KEY"

/* The URL for the IP-API */
#define IP_API_URL "http://ip-api.com/json"

/* The URL for the OpenWeatherMap API */
#define WEATHER_API_URL "http://api.openweathermap.org/data/2.5/weather?lat=%.2f&lon=%.2f&appid=%s"

/* The size of the response buffer */
#define BUFFER_SIZE 1024 * 10

#define MY_RELLOC(data, num, type)            \
    do {                                      \
        if (data) {                           \
            free(data);                       \
            data = NULL;                      \
        }                                     \
        if (num > 0) {                        \
            data = calloc(num, sizeof(type)); \
        }                                     \
    } while (0)

#define MY_RELLOC_REQUIRE(data, num, type, tag)            \
    do {                                                   \
        MY_RELLOC(data, num, type);                        \
        if (!data) {                                       \
            printf("%s %s malloc is null\n", #tag, #data); \
            goto tag;                                      \
        }                                                  \
    } while (0)

/* The callback function for writing the response */
static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    char **buffer_ptr = (char **)userdata;
    size_t realsize = size * nmemb;
    char **tmp_buf_ptr = buffer_ptr;

    if(NULL == *buffer_ptr)
        exit(EXIT_FAILURE);

    printf("size = %d, %d\n", strlen(*buffer_ptr), strlen(*buffer_ptr) + realsize + 1);
    *buffer_ptr = (char *)realloc(*buffer_ptr, strlen(*buffer_ptr) + realsize + 1);

    if (NULL == *buffer_ptr) {
        fprintf(stderr, "Error: out of memory\n");
        if (NULL != *tmp_buf_ptr) {
            free(*tmp_buf_ptr);
        }
        exit(EXIT_FAILURE);
    }

    memcpy(*buffer_ptr + strlen(*buffer_ptr), ptr, realsize);
    return realsize;
}

int main(int argc, char **argv)
{
    CURL *curl_handle = curl_easy_init();
    char *buffer = (char *)calloc(BUFFER_SIZE, sizeof(char));
    char ip_api_url[BUFFER_SIZE];
    char weather_api_url[BUFFER_SIZE];
    CURLcode res;
    struct json_object *ip_json;
    struct json_object *latitude_json;
    struct json_object *longitude_json;
    double latitude;
    double longitude;

    if (curl_handle == NULL || buffer == NULL) {
        fprintf(stderr, "Error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    /* Get the IP address location */
    snprintf(ip_api_url, BUFFER_SIZE, IP_API_URL);
    curl_easy_setopt(curl_handle, CURLOPT_URL, ip_api_url);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&buffer);

    res = curl_easy_perform(curl_handle);

    if (res != CURLE_OK) {
        fprintf(stderr, "Error: %s\n", curl_easy_strerror(res));
        exit(EXIT_FAILURE);
    }

    printf("buffer: %s\n", buffer);
    ip_json = json_tokener_parse(buffer);
    json_object_object_get_ex(ip_json, "lat", &latitude_json);
    json_object_object_get_ex(ip_json, "lon", &longitude_json);
    latitude = json_object_get_double(latitude_json);
    longitude = json_object_get_double(longitude_json);

#if 0
    /* Get the weather data */
    snprintf(weather_api_url, BUFFER_SIZE, WEATHER_API_URL, latitude, longitude, API_KEY);
    curl_easy_setopt(curl_handle, CURLOPT_URL, weather_api_url);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&buffer);

    res = curl_easy_perform(curl_handle);

    if (res != CURLE_OK) {
        fprintf(stderr, "Error: %s\n", curl_easy_strerror(res));
        exit(EXIT_FAILURE);
    }
#endif

    printf("%s\n", buffer);

    json_object_put(ip_json);
    curl_easy_cleanup(curl_handle);
    free(buffer);

    return 0;
}
