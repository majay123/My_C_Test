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
 * @Date         : 2021-11-19 10:55:40
 * @LastEditors  : MCD
 * @LastEditTime : 2021-11-22 13:59:00
 * @FilePath     : /My_C_Test/Hope_Work/migu_test/local_common.h
 * @Description  : 
 * 
 * ******************************************
 */
#ifndef __LOCAL_COMMON_H__
#define __LOCAL_COMMON_H__
#include <stdint.h>
#include <pthread.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#define MAX_BODY_LEN    (1024)

/** 
 * @brief  dump all the stack of L
 * @note   ok the word 'stack' is spelled by mistake
 * @param  *L: the obj of lua
 * @retval None
 */
void lua_stachDump(lua_State *L);

/** 
 * @brief  common lua call function
 * @note   null
 * @param  *filename: target lua file
 * @param  *func: target lua function name
 * @param  *format: format of args 'xxx(input)>xxx(output)'
 *          i->int, d->double, s->char *;
 *       exp:   if ssid>isd : 
 *              char *input1, char *input2, int input3, double input4,
 *              int *output1, char *output2, double *output3;
 * @param  ...: args
 * @retval None
 */
void lua_call_fun(const char *filename,
                    const char *func,
                    const char *format,
                    ...);

/**
 * @brief  
 * @note   
 * @param  *filename: 
 * @param  *func: 
 * @param  callback: 
 * @retval None
 */
void lua_call_fun_async(const char *filename,
                        const char *func,
                        void (*callback)(char *, char *),
                        const char *format,
                        ...);


int luaopen_hpio(lua_State *L);
#endif // __LOCAL_COMMON_H__
