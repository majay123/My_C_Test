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
 * @Date         : 2021-11-19 10:55:47
 * @LastEditors  : MCD
 * @LastEditTime : 2021-11-22 13:59:07
 * @FilePath     : /My_C_Test/Hope_Work/migu_test/local_common.c
 * @Description  : 
 * 
 * ******************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <net/if.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <errno.h>
#include <syslog.h>
#include <pthread.h>
#include <sys/time.h>
#include <fcntl.h>
#include <termios.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdarg.h>
#include "cJSON.h"
#include "migu.h"
#include "local_common.h"
#include "common.h"

#define HPIO_MIGU     "hpio.migu"

struct _st_lua_arg
{
    lua_State *l;
    void (*callback)(char *, char *);
    char *fmt;
    char *filename;
    char *func;
    cJSON *input;
    cJSON *root;
    int narg, nres;
};

//======================c to lua======================

static void _clean_cb_(void *arg)
{
    struct _st_lua_arg *a = (struct _st_lua_arg*)arg;
    if(a) {
        if(a->filename) {
            free(a->filename);
        }
        if(a->func) {
            free(a->func);
        }
        if(a->input) {
            cJSON_Delete(a->input);
        }
        if(a->fmt) {
            free(a->fmt);
        }
        if(a->l) {
            lua_close(a->l);
        }
        if(a->root) {
            cJSON_Delete(a->root);
        }
        free(a);
    }
}

static void *_lua_proc_(void *arg)
{
    pthread_cleanup_push(_clean_cb_, arg);
    struct _st_lua_arg *a = (struct _st_lua_arg *)arg;
#if 1
    lua_State *L = lua_open();
    a->l = L;
    luaL_openlibs(L);
    luaopen_hpio(L);

    if(luaL_loadfile(L, a->filename) || lua_pcall(L, 0, 0, 0)) {
        printf("lua load %s file error\n", lua_tostring(L, -1));
        goto End;
    }
    // va_list v1;
    lua_getglobal(L, a->func);
    int len = cJSON_GetArraySize(a->input);
    if(len == 0 && a->callback == NULL) {
        lua_pcall(L, 0, 0, 0);
        print_mcd("lua call without args");
        goto End;
    }

    int i;
    
	for(i = 0;i < len; i++) {
		cJSON *node = cJSON_GetArrayItem(a->input, i);
        if(!node) {
            continue;
        }
        switch(node->type) {
            case cJSON_String: {
                lua_pushstring(L, node->valuestring);
                break;
            }
            case cJSON_Number: {
                lua_pushnumber(L, node->valuedouble);
                break;
            }
            default: {
                break;
            }
        }
        luaL_checkstack(L, 1, "toomany arguments");
	}
    a->narg = len;
    // print_mcd("lua async push ok");
#endif

    a->root = cJSON_CreateArray();
    int lua_err = lua_pcall(a->l, a->narg, a->nres, 0);
    if(lua_err < 0) {
        print_mcd("call lua error! %d %s\n", lua_err, lua_tostring(L, -1));
        lua_pop(L, 1);
        goto End;
    }
    char *format = a->fmt;
    // print_mcd("format %s\n", format);
    
    int nres = 0;
    while(*format) {
        ++nres;
        if(nres > a->nres) {
            break;
        }
        switch(*format++) {
            case 'd':
            case 'i': {
                if(!lua_isnumber(a->l, nres)){
                    printf("wrong type d i!\n");
                    // return  NULL;
                    break;
                }
                double n = lua_tonumber(a->l, nres);
                cJSON_AddItemToArray(a->root,cJSON_CreateNumber(n));
                break;
            }
            case 's': {
                if(!lua_isstring(a->l, nres)) {
                    printf("wrong type s!\n");
                    // return NULL;
                    break;
                }
                char *str = (char *)lua_tostring(a->l, nres);
                cJSON_AddItemToArray(a->root, cJSON_CreateString(str));
                break;
            }
            default: {
                printf("invalid option (%c)\n", *(format-1));
                // return NULL;
                break;
            }
        }
    }
    // a->nres = nres;
    lua_pop(a->l, a->nres);
    a->nres = 0;
    
    if(a->callback) {
        print_mcd("lua thread callback is going to run...");
        char *out = cJSON_PrintUnformatted(a->root);
        if(out){
            a->callback(a->fmt, out);
            free(out);
        } else {
            print_mcd("cjson print error");
        }
    }
End:
    pthread_cleanup_pop(1);
}

void lua_call_fun_async(const char *filename,
                        const char *func,
                        void (*callback)(char *, char *),
                        const char *format,
                        ...)
{
    int narg, nres;
    va_list v1;
    cJSON *root = cJSON_CreateArray();
    if(!root) {
        return;
    }
    va_start(v1, format);
    narg = 0;
    while(*format) {
        switch(*format++) {
            case 'd' : {
                cJSON_AddItemToArray(root, cJSON_CreateNumber(va_arg(v1, double)));
                break;
            }
            case 'i' : {
                cJSON_AddItemToArray(root, cJSON_CreateNumber(va_arg(v1, int)));
                break;
            }
            case 's' : {
                cJSON_AddItemToArray(root, cJSON_CreateString(va_arg(v1, char*)));
                break;
            }
            case '>' : {
                goto Endwhile;
            }
            default : {
                printf("invalid option!\n");
                break;
            }
        }
        ++narg;
    } Endwhile:
    nres = strlen(format);

    struct _st_lua_arg *a = (struct _st_lua_arg *)calloc(1, sizeof(struct _st_lua_arg));
    a->input = root;
    a->callback = callback;
    a->fmt = strdup(format);
    if(!a->fmt) {
        return;
    }
    a->root = NULL;
    a->filename = strdup(filename);
    if(!a->filename) {
        return;
    }
    a->func = strdup(func);
    if(!a->func) {
        return;
    }
    a->narg = narg;
    a->nres = nres;

    pthread_t pid;
    pthread_create(&pid, NULL, _lua_proc_, (void *)a);
    pthread_detach(pid);
    // pthread_join(pid, NULL);
}


void lua_call_fun(const char *filename,
                    const char *func,
                    const char *format,
                    ...)
{
    if(filename == NULL || func == NULL) return;
    
    uint64_t _ms = (uint64_t)get_now_ms();
    uint64_t _us = (uint64_t)get_now_us();
    print_mcd("call lua fun %s", func);

    lua_State *L = lua_open();
    luaL_openlibs(L);
    luaopen_hpio(L);

    if(luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0)) {
        printf("lua load %s file error\n", lua_tostring(L, -1));
        goto Exit;
    }
    va_list v1;
    int narg, nres;
    lua_getglobal(L, func);
    if(format == NULL || strlen(format) <= 0) {
        lua_pcall(L, 0, 0, 0);
        print_mcd("lua call without args");
        return;
    }
    va_start(v1, format);
    narg = 0;
    
    while(*format) {
        switch(*format++) {
            case 'd' : {
                lua_pushnumber(L, va_arg(v1, double));
                break;
            }
            case 'i' : {
                lua_pushnumber(L, va_arg(v1, int));
                break;
            }
            case 's' : {
                char *str = va_arg(v1, char *);
                lua_pushstring(L, str);
                break;
            }
            case '>' : {
                goto Endwhile;
            }
            default : {
                printf("invalid option!\n");
                break;
            }
        }
        ++narg;
        luaL_checkstack(L, 1, "toomany arguments");
    } Endwhile:
    nres = strlen(format);
    if(lua_pcall(L, narg, nres, 0)) {
        printf("call function error!\n");
        goto Exit;
    }
    nres = 0;
    while(*format) {
        ++nres;
        switch(*format++) {
            case 'd': {
                if(!lua_isnumber(L, nres)){
                    printf("wrong type d!\n");
                    goto Exit;
                }
                int n = lua_tonumber(L, nres);
                *va_arg(v1, double *) = n;
                break;
            }
            case 'i': {
                if(!lua_isnumber(L, nres)) {
                    printf("wrong type i!\n");
                    goto Exit;
                }
                int n = lua_tonumber(L, nres);
                *va_arg(v1, int *) = n;
                // *va_arg(v1, int *) = lua_tonumber(L, nres);
                break;
            }
            case 's': {
                if(!lua_isstring(L, nres)) {
                    printf("wrong type s!\n");
                    goto Exit;
                }
                char *str = (char *)lua_tostring(L, nres);
                strncpy(va_arg(v1, char *), str, MAX_BODY_LEN - 1);
                break;
            }
            default: {
                printf("invalid option (%c)\n", *(format-1));
                goto Exit;
            }
        }
    }

    va_end(v1);
    lua_pop(L,nres);
Exit:
    lua_close(L);
    print_err("\nlua call last %lld ms %lld ms\n\n", get_now_ms() - _ms, get_now_us() - _us);
}


//======================lua to c======================

static int _migu_get_musicInfos(lua_State* L)
{
    char *musicIds = (char *)luaL_checkstring(L, 1);
    char *result = (char *)calloc(1024*50, sizeof(char));
    get_musicInfos(musicIds, "S", result);
    if(result != NULL)
        lua_pushstring(L, result);
    return 1;
}

static const luaL_reg _migu_api_M[] =  {
    {"migu_get_musicinfos",         _migu_get_musicInfos},
    {NULL,      NULL},
};

static void hpio_migu_api(lua_State *L)
{
    luaL_newmetatable(L, HPIO_MIGU);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "_index");
    luaL_register(L, NULL, _migu_api_M);
    lua_setfield(L, -2, "migu");
}

int luaopen_hpio(lua_State *L)
{
    luaL_newmetatable(L, "hpio_root");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    // lua_pushvalue(L, -1);

    hpio_migu_api(L);

    lua_setglobal(L, "hpio");
    return 0;
}