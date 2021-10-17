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
 * ************Copyright 2021 MCD************
 * 
 * @version      : 
 * @Company      : HOPE
 * @Author       : MCD
 * @Date         : 2021-09-06 13:08:36
 * @LastEditors  : MCD
 * @LastEditTime : 2021-09-28 17:02:14
 * @FilePath     : /My_C_Test/protobuff/main.c
 * @Description  : 
 * 
 * ******************************************
 */

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "common.h"
#include "test.pb-c.h"
#include "cJSON.h"
// {"action":"OFF","device":"开关二","inputText":"关闭开关二","attribute":"STATE","catalog":"smart"}

#define CONDITION_PATH      "test.json"

static void test_json(SearchResponse__Result *s)
{
    char *buffer = NULL;
	long length;
	FILE *fp;
	int i = 0;
	int size = 0;
    char *result = NULL;
	
	fp = fopen(CONDITION_PATH, "rb");

	fseek(fp, 0, SEEK_END);
	length = ftell(fp);
	// print_mcd("get length = %d", length);

	buffer = (char *)calloc(1, length + 1);
	fseek(fp, 0, SEEK_SET);
	fread(buffer, 1, length, fp);
	close(fp);
	// print_mcd("get data : %s", buffer);

	// cJSON *root = cJSON_Parse(buffer);
    cJSON *root = REQ_JSON_PARSE(buffer, root, Error);
	free(buffer);
    cJSON *device = REQ_JSON_OBJ(root, device, Error);

    char temp2[128]={0};
    snprintf(temp2,sizeof(temp2),"%s",device->valuestring);
    // snippets[j] = strdup(temp2);
    // print_mcd("%s", temp2);
    s->url = strdup(device->valuestring);
Error:
    if(root != NULL)
    {
        cJSON_Delete(root);
        root = NULL;
    }
}

static void _student_proto_test(void)
{
    StudentMsg pack_st = STUDENT_MSG__INIT;
    StudentMsg *unpack_st = NULL;

    pack_st.address = "zhejiang ningbo";
    pack_st.age = 19;
    pack_st.class_num = 2;
    pack_st.name = "ma xiao dong";

    int len = student_msg__get_packed_size(&pack_st);
    print_mcd("get len = %d", len);

    uint8_t *out = calloc(len, sizeof(uint8_t));
    if (out) {
        student_msg__pack(&pack_st, out);
        // print_mcd("out  = %s", out);

        unpack_st = student_msg__unpack(NULL, len, out);
        print_mcd("name = %s, age = %d, class_num = %d, address = %s", unpack_st->name, unpack_st->age, unpack_st->class_num, unpack_st->address);

        student_msg__free_unpacked(unpack_st, NULL);
        free(out);
    }
}

static void _search_respone_proto_test(void)
{
    int i, j;
    SearchResponse__Result **results;
    SearchResponse pack_sr = SEARCH_RESPONSE__INIT;
    SearchResponse *unpack_sr;
    char **snippets;
    SearchResponse__Result res = SEARCH_RESPONSE__RESULT__INIT;

#if 0
    pack_sr.results = calloc(sizeof(SearchResponse__Result *), sizeof(char));
    pack_sr.n_results = 1;
    res.page_number = 100;
    pack_sr.results[0] = &res;
#endif
    pack_sr.n_results = 10;
#if 0 //my func
    results = malloc(sizeof(SearchResponse__Result *) * pack_sr.n_results);

    for (i = 0; i < pack_sr.n_results; i++) {
        results[i] = malloc(sizeof(SearchResponse__Result));
        search_response__result__init(results[i]);
        results[i]->n_snippet = 5;
        snippets = malloc(sizeof(char *) * results[i]->n_snippet);
        for (j = 0; j < results[i]->n_snippet; j++) {
            // snippets[j] = malloc(256);
            snippets[j] = "test snippet";
        }
        results[i]->snippet = snippets;
        results[i]->url = "test url";
        results[i]->title = "test title";
    }

    pack_sr.results = results;
#else   //li gong func
    #if 1
    pack_sr.results = calloc(sizeof(SearchResponse__Result *) * pack_sr.n_results, sizeof(char));
    SearchResponse__Result *result = calloc(sizeof(SearchResponse__Result) * pack_sr.n_results, sizeof(char));

    for (i = 0; i < pack_sr.n_results; i++) {
        search_response__result__init(&result[i]);
        result[i].page_number = i;
        result[i].n_snippet = 5;
        snippets = malloc(sizeof(char *) * result[i].n_snippet);
        for (j = 0; j < result[i].n_snippet; j++) {
            char temp2[128]={0};
            snprintf(temp2,sizeof(temp2),"snippets,msg%zu",i);
            snippets[j] = strdup(temp2);
            // test_json(&result[i]);
        }
        result[i].snippet = snippets;
        char temp[128]={0};
        snprintf(temp,sizeof(temp),"test url,msg%zu",i);
        char temp1[128]={0};
        snprintf(temp1,sizeof(temp1),"test title,msg%zu",i);
        // result[i].url = strdup(temp);
        test_json(&result[i]);
        result[i].title = strdup(temp1);
        // result[i].title = test_json(&result[i]);
        pack_sr.results[i]=&result[i];
    }
    #endif
#endif
    print_mcd("start");
    int size = search_response__get_packed_size(&pack_sr);
    print_mcd("get size = %d", size);
    void *buf = malloc(size);
    if (buf) {
        search_response__pack(&pack_sr, buf);
        print_mcd("pack ok");
    #if 0 // my func
        for (i = 0; i < pack_sr.n_results; i++) {
            // for (j = 0; j < pack_sr.results[i]->n_snippet; j++) {
            //     free(pack_sr.results[i]->snippet[j]);
            // }
            free(pack_sr.results[i]->snippet);
            free(pack_sr.results[i]);
        }
        free(pack_sr.results);
    #else
        #if 1
        for (i = 0; i < pack_sr.n_results; i++) {
            free(pack_sr.results[i]->title);
            free(pack_sr.results[i]->url);
            for (j = 0; j < pack_sr.results[i]->n_snippet; j++)
                free(pack_sr.results[i]->snippet[j]);
        }
        free(pack_sr.results);
        free(result);
        #endif
    #endif

        print_mcd("free memory ok");
        unpack_sr = search_response__unpack(NULL, size, buf);
    #if 0
        for (i = 0; i < unpack_sr->n_results; i++) {
            print_mcd("url = %s %d", unpack_sr->results[i]->url, i);
            print_mcd("title = %s %d", unpack_sr->results[i]->title, i);
            print_mcd("page_number = %d %d", unpack_sr->results[i]->page_number, i);
            for (j = 0; j < unpack_sr->results[i]->n_snippet; j++) {
                print_mcd("snippet = %s %d", unpack_sr->results[i]->snippet[j], j);
            }
        }
    #else   
        SearchResponse__Result **result_test;
        char tmp[64] = {0};
        size_t num = unpack_sr->n_results;
        
        result_test = (SearchResponse__Result **)calloc(sizeof(SearchResponse__Result *) * unpack_sr->n_results, sizeof(char));
        print_mcd("test1");
        for ( i = 0; i < num; i++)
        {
            SearchResponse__Result *result_tmp = (SearchResponse__Result *)calloc(sizeof(SearchResponse__Result), sizeof(char));
            print_mcd("test2");
            // memcpy(result_tmp, unpack_sr->results[i], sizeof(SearchResponse__Result));
            result_tmp->page_number = unpack_sr->results[i]->page_number;
            snprintf(tmp, sizeof(tmp), "%s", unpack_sr->results[i]->title);
            result_tmp->title = strdup(tmp);
            print_mcd("%s", result_tmp->title);
            memset(tmp, 0, sizeof(tmp));
            snprintf(tmp, sizeof(tmp), "%s", unpack_sr->results[i]->url);
            result_tmp->url = strdup(tmp);
            result_test[i] = result_tmp ;
        }

        search_response__free_unpacked(unpack_sr, NULL);
        free(buf);

        for (i = 0; i < num; i++) {
            print_mcd("url = %s %d", result_test[i]->url, i);
            print_mcd("title = %s %d", result_test[i]->title, i);
            print_mcd("page_number = %d %d", result_test[i]->page_number, i);
            for (j = 0; j < result_test[i]->n_snippet; j++) {
                print_mcd("snippet = %s %d", result_test[i]->snippet[j], j);
            }
        }
    #endif
        for (i = 0; i < num; i++) {
            if(result_test[i]->url)
                free(result_test[i]->url);
            if(result_test[i]->title)
                free(result_test[i]->title);
            if(result_test[i])
                free(result_test[i]);
        }
        free(result_test);
        // search_response__free_unpacked(unpack_sr, NULL);
        // free(buf);
    }
}



static void _search_respone_proto_one_test(void)
{
    int i, j;
    // SearchResponse__Result **results;
    SearchResponse pack_sr = SEARCH_RESPONSE__INIT;
    SearchResponse *unpack_sr;
    char **snippets;
    SearchResponse__Result res = SEARCH_RESPONSE__RESULT__INIT;

    
    #if 1   // use Heap(堆) memory
    pack_sr.results = (SearchResponse__Result **)calloc(sizeof(SearchResponse__Result *), sizeof(char));
    pack_sr.n_results = 1;
    res.page_number = 100;
    pack_sr.results[0] = &res;
    #else   // use Stack(栈) memory
    SearchResponse__Result *d_res = &res;
    pack_sr.n_results = 1;
    res.page_number = 100;
    pack_sr.results = &d_res;
    #endif
    print_mcd("start");
    int size = search_response__get_packed_size(&pack_sr);
    print_mcd("get size = %d", size);
    void *buf = malloc(size);
    if (buf) {
        search_response__pack(&pack_sr, buf);
        print_mcd("pack ok");


        print_mcd("free memory ok");
        unpack_sr = search_response__unpack(NULL, size, buf);
        for (i = 0; i < unpack_sr->n_results; i++) {
            print_mcd("url = %s %d", unpack_sr->results[i]->url, i);
            print_mcd("title = %s %d", unpack_sr->results[i]->title, i);
            print_mcd("page_number = %d %d", unpack_sr->results[i]->page_number, i);
            for (j = 0; j < unpack_sr->results[i]->n_snippet; j++) {
                print_mcd("snippet = %s %d", unpack_sr->results[i]->snippet[j], j);
            }
        }
        search_response__free_unpacked(unpack_sr, NULL);
        free(buf);
    }
}


static void _search_respone_proto_point_test(void * arg) 
{
    int i;
    SearchResponse__Result **results = (SearchResponse__Result **)arg;
    SearchResponse__Result *result = (SearchResponse__Result *)calloc(10, sizeof(SearchResponse__Result));
    for ( i = 0; i < 10; i++)
    {
        search_response__result__init(&result[i]);
        memcpy(&result[i], results[i], sizeof(SearchResponse__Result));
        print_mcd("test = %s", result[i].title);
        print_mcd("test = %s", result[i].url);
    }
    free(result);
}


static void _proto_point_test(void)
{
    int i;
    SearchResponse__Result **results = (SearchResponse__Result **)calloc(10, sizeof(SearchResponse__Result *));
    SearchResponse__Result *result = (SearchResponse__Result *)calloc(10, sizeof(SearchResponse__Result));
    for ( i = 0; i < 10; i++)
    {
        uint8_t tmp[64] = {0};
        uint8_t tmp1[64] = {0};
        snprintf(tmp, sizeof(tmp), "title test %d", i);
        snprintf(tmp1, sizeof(tmp), "url test %d", i);
        result[i].title = strdup(tmp);
        result[i].url = strdup(tmp1);
        results[i] = &result[i];
    }
    _search_respone_proto_point_test(results);
    for ( i = 0; i < 10; i++) {
        free(result[i].title);
        free(result[i].url);
    }
    free(results);
    free(result);
}

static void _search_respone_proto_point_oneitem_test(void * arg) 
{
    int i = 0;
    SearchResponse__Result **results = (SearchResponse__Result **)arg;
    print_mcd("test = %s", results[i]->title);
    print_mcd("test = %s", results[i]->url);
    SearchResponse__Result *result = (SearchResponse__Result *)calloc(1, sizeof(SearchResponse__Result));
    for ( i = 0; i < 1; i++)
    {
        search_response__result__init(&result[i]);
        memcpy(&result[i], results[i], sizeof(SearchResponse__Result));
        print_mcd("test = %s", result[i].title);
        print_mcd("test = %s", result[i].url);
    }
    free(result);
}
static void _proto_oneitem_test(void)
{
    SearchResponse__Result result = SEARCH_RESPONSE__RESULT__INIT;
    SearchResponse__Result *d_res = &result;
    SearchResponse__Result **d_res1  = NULL;
    int i = 0;
    uint8_t tmp[64] = {0};
    uint8_t tmp1[64] = {0};
    snprintf(tmp, sizeof(tmp), "title test %d", i);
    snprintf(tmp1, sizeof(tmp), "url test %d", i);
    result.title = strdup(tmp);
    result.url = strdup(tmp1);
    d_res1 = &d_res;
    _search_respone_proto_point_oneitem_test(d_res1);

    free(result.title);
    free(result.url);
}


int main(int argc, char const *argv[])
{
    int i = 0;

    // student_msg__init(&pack_st);
    char tmp[64] = "testsetast";
    char *tmp2 = tmp;
    print_mcd("argc = %d", argc);
    for (i = 1; i < argc; i++)
        print_mcd("argv[%d] = %d", i, atoi(argv[i]));
    print_mcd("%s",tmp2);
    _proto_oneitem_test();
    // _student_proto_test();
    // _proto_point_test();
    // _search_respone_proto_test();
    // _search_respone_proto_one_test();

    return 0;
}
