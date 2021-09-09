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
 * @LastEditTime : 2021-09-09 16:15:33
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
    pack_sr.results = calloc(sizeof(SearchResponse__Result *) * pack_sr.n_results, sizeof(char));
    SearchResponse__Result *result = calloc(sizeof(SearchResponse__Result) * pack_sr.n_results, sizeof(char));

    for (i = 0; i < pack_sr.n_results; i++) {
        search_response__result__init(&result[i]);
        result[i].n_snippet = 5;
        snippets = malloc(sizeof(char *) * result[i].n_snippet);
        for (j = 0; j < result[i].n_snippet; j++) {
            char temp2[128]={0};
            snprintf(temp2,sizeof(temp2),"snippets,msg%zu",i);
            snippets[j] = strdup(temp2);
        }
        result[i].snippet = snippets;
        char temp[128]={0};
        snprintf(temp,sizeof(temp),"test url,msg%zu",i);
        char temp1[128]={0};
        snprintf(temp1,sizeof(temp1),"test title,msg%zu",i);
        result[i].url = strdup(temp);
        result[i].title = strdup(temp1);
        pack_sr.results[i]=&result[i];
    }
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
        for (i = 0; i < pack_sr.n_results; i++) {
            free(pack_sr.results[i]->title);
            free(pack_sr.results[i]->url);
            for (j = 0; j < pack_sr.results[i]->n_snippet; j++)
                free(pack_sr.results[i]->snippet[j]);
        }
        free(pack_sr.results);
        free(result);
    #endif

        print_mcd("free memory ok");

        unpack_sr = search_response__unpack(NULL, size, buf);

        for (i = 0; i < unpack_sr->n_results; i++) {
            print_mcd("url = %s %d", unpack_sr->results[i]->url, i);
            print_mcd("title = %s %d", unpack_sr->results[i]->title, i);
            for (j = 0; j < unpack_sr->results[i]->n_snippet; j++) {
                print_mcd("snippet = %s %d", unpack_sr->results[i]->snippet[j], j);
            }
        }
        search_response__free_unpacked(unpack_sr, NULL);
        free(buf);
    }
}

int main(int argc, char const *argv[])
{

    // student_msg__init(&pack_st);
    _student_proto_test();
    _search_respone_proto_test();

    return 0;
}
