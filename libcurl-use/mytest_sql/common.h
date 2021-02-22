#ifndef __COMMON_H__
#define __COMMON_H__

#include <sqlite3.h>

#define MAX_BUFFER_SIZE	2048

#define PRINT_MCD_ENABLE  1
#define PRINT_LOG_ENABLE  0

#if PRINT_MCD_ENABLE == 1
    #define print_mcd(format, arg...)   do { printf("\033[31m[----mcd----]\033[0m:%s,%d--- "format"\n",__FILE__,__LINE__, ## arg);}\
                                        while (0)
#else
    #define print_mcd(format, arg...)   do {} while (0)
#endif

#if PRINT_LOG_ENABLE == 1
    #define print_log(format, arg...)   do { printf("\033[31m[----log----]\033[0m:%s,%d--- "format"\n",__FILE__,__LINE__, ## arg);}\
                                        while (0)
#else
    #define print_log(format, arg...)   do {} while (0)
#endif

#define REQUIRE(in,tag)                     do{if(in){print_log("%s %s", #tag, #in); goto tag;}}while(0)
#define REQUIRE_NOLOG(in,tag)              	do{if(in){print_log("%s %s", #tag, #in); goto tag;}}while(0)
#define REQ_JSON_OBJ(des,item,tag)        	cJSON_GetObjectItem(des,#item); \
                                                           REQUIRE((item==NULL),tag)
#define REQ_JSON_PARSE(str,item,tag)     	cJSON_Parse(str); \
                                                            REQUIRE((item==NULL),tag)

typedef struct
{
	sqlite3 *db;
} dataBaseManager_t;

#endif // !__COMMON_H__