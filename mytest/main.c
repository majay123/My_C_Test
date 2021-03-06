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
 * @Date         : 2021-05-20 14:28:57
 * @LastEditors  : MCD
 * @LastEditTime : 2021-06-17 14:24:15
 * @FilePath     : /My_C_Test/mytest/main.c
 * @Description  : 
 * 
 * ******************************************
 */


#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

/* somewhat unix-specific */
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
// #include <sqlite3.h>
#include <sys/sysinfo.h>

#include "common.h"
#include "cJSON.h"
// #include "cJSON.h"
#define NAME_MAX_LEN        (32)
#define INFO_MAX_NUM        (80)
#define SRV_CONTENT_SIZE   	(4*1024)
// typedef struct
// {
//     char num;
// 	char type;
//     char info[];
// }all_db_info_t;

typedef struct 
{
    char dev_type;
    char name[NAME_MAX_LEN];
}db_info_t;

typedef struct
{
    char num;
    db_info_t info[INFO_MAX_NUM];
}all_db_info_t;

typedef struct
{
    /* srv msg value when send msg, or msg_reply_type_e when reply from service */
    int type;
    /* 0: async msg, 1: sync msg */
    int sync;
    /* content real size */
    int c_size;
    char content[SRV_CONTENT_SIZE];
}srv_msg_t;


static char *_print_time(void)
{
    struct timeval tv;
    struct tm *t;
	static tbuf[32] = {0};
    gettimeofday(&tv, NULL);
    t = localtime(&tv.tv_sec);

    // printf("[ %d-%d-%d %d:%d:%d.%ld ] \n", 1900+t->tm_year, 1+t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, tv.tv_usec / 1000);
	sprintf(tbuf, "[ %d-%d-%d %d:%d:%d.%ld ]", 1900+t->tm_year, 1+t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, tv.tv_usec / 1000);
	// printf("%s\n", tbuf);
	return tbuf;
}
void _tmp_test(all_db_info_t *infos)
{
	#if 0
	// all_db_info_t *all_db_info;
	infos->num = 10;
	print_mcd("111");
	infos->info = (char **)calloc(1, infos->num * sizeof(char *));
	int i;
	print_mcd("111");
	for(i = 0; i < infos->num; i++)
	{
		infos->info[i] = "test098090 jkjkljiojoijkl";
		// print_mcd("set info");
		// snprintf(infos->info[i], NAME_MAX_LEN, "%s", "test");
	}
	print_mcd("get memory = %p", infos->info);
	#endif
	// all_db_info_t *malloc_devs = (all_db_info_t *)calloc(1, sizeof(all_db_info_t));
}

#define CONDITION_PATH      "test.json"

int _get_file_size()
{
	char *buffer = NULL;
	long length;
	FILE *fp;
	int i = 0;
	int size = 0;
	
	fp = fopen(CONDITION_PATH, "rb");

	fseek(fp, 0, SEEK_END);
	length = ftell(fp);
	print_mcd("get length = %d", length);

	buffer = (char *)calloc(1, length + 1);
	fseek(fp, 0, SEEK_SET);
	fread(buffer, 1, length, fp);
	close(fp);
	// print_mcd("get data : %s", buffer);

	cJSON *root = cJSON_Parse(buffer);
	free(buffer);
	cJSON *params_js = cJSON_GetObjectItem(root, "params");
	cJSON *scene_js = cJSON_GetObjectItem(params_js, "sce");
	cJSON *dev_js = cJSON_GetObjectItem(params_js, "dev");
	
	size = cJSON_GetArraySize(scene_js);
	print_mcd("%d", size);
	for(i= 0 ; i < size; i++)
	{
		cJSON *item_js = cJSON_GetArrayItem(scene_js, i);
		print_mcd("name  = %s", item_js->string);
	}

	size = cJSON_GetArraySize(dev_js);
	print_mcd("%d", size);
	for(i= 0 ; i < size; i++)
	{
		cJSON *item_js = cJSON_GetArrayItem(dev_js, i);
		print_mcd("name  = %s", item_js->string);
	}
	
	

	if(root == NULL)
		return;
	char *out = cJSON_PrintUnformatted(root);
	// print_mcd("out : %s", out);
	
}

void ctrl_db_device_scene1(void)
{
    cJSON *root = NULL;
    cJSON *command = NULL;
    cJSON *param = NULL;
    // char *out = NULL;
	
    REQUIRE((root = cJSON_CreateObject()) == NULL, Error);
    REQUIRE((command = cJSON_CreateObject()) == NULL, Error);
    cJSON_AddItemToObject(root, "command", command);
    REQUIRE((param = cJSON_CreateObject()) == NULL, Error);
    cJSON_AddItemToObject(command, "param", param);
    cJSON_AddStringToObject(root, "api", "dev_ctrl");

	cJSON_AddStringToObject(param, "设备名称", "廊灯");


	char *out = cJSON_PrintUnformatted(root);
	print_mcd("out = %s", out);

	free(out);
Error:
    if(root != NULL)
    {
        cJSON_Delete(root);
        root = NULL;
    }
}

// {"command":{"param":{"操作":"打开","设备名称":"廊灯", "name":"廊灯"},"api":"dev_ctrl"}}
_db_rs485_dis_t db_ctrl_lc[] = 
{
    {HOPE_DB_ACTION_NULL  , {0},   {0},   },
    {HOPE_DB_ACTION_CLOSE , "操作", "关闭"},
    {HOPE_DB_ACTION_OPEN  , "操作", "打开"},
    {HOPE_DB_ACTION_STOP  , "操作", "暂停"},
    {HOPE_DB_ACTION_MODE  , "模式",  ""  },
    {HOPE_DB_ACTION_SPEED , "风速",  ""  },
    {HOPE_DB_ACTION_ADD   , "温度",  "+"  },
    {HOPE_DB_ACTION_REDUCE, "温度",  "-"  },
};

_db_rs485_ms_t db_ctrl_ac_mode[] = {
    {MODE_COOL, "制冷"},
    {MODE_HEAT, "制热"},
    {MODE_AIR , "通风"},
    {MODE_DRY , "除湿"},
};
_db_rs485_ms_t db_ctrl_ac_speed[] = {
    {VALUE_NULL  , ""},
    {VALUE_LOW   , "低"},
    {VALUE_MIDDLE, "中"},
    {VALUE_HIGH  , "高"},
    {VALUE_AUTO  , "自动"},
};

void ctrl_db_device_scene(db_ctrl_t *db_ctrl)
{
    cJSON *root = NULL;
    cJSON *command = NULL;
    cJSON *param = NULL;
    char *out = NULL;

    REQUIRE((root = cJSON_CreateObject()) == NULL, Error);
    REQUIRE((command = cJSON_CreateObject()) == NULL, Error);
    cJSON_AddItemToObject(root, "command", command);
    REQUIRE((param = cJSON_CreateObject()) == NULL, Error);
    cJSON_AddItemToObject(command, "param", param);
    cJSON_AddStringToObject(root, "api", "dev_ctrl");
    

    switch (db_ctrl->dev_type)
    {
        case HOPE_DB_LIGHT_TYPE:
        case HOPE_DB_CURTAIN_TYPE:
        case HOPE_DB_AIR_CONDITIONER_TYPE:
        {
            cJSON_AddStringToObject(param, "设备名称", db_ctrl->name);
            if(db_ctrl->action == HOPE_DB_ACTION_MODE)
            {
                cJSON_AddStringToObject(param, db_ctrl_lc[db_ctrl->action].name_action, db_ctrl_ac_mode[db_ctrl->attribute].m_name); 
            }   
            else if(db_ctrl->action == HOPE_DB_ACTION_SPEED)   
            {
                cJSON_AddStringToObject(param, db_ctrl_lc[db_ctrl->action].name_action, db_ctrl_ac_speed[db_ctrl->attribute].m_name); 
            }
            else if(db_ctrl->action > 0)
                cJSON_AddStringToObject(param, db_ctrl_lc[db_ctrl->action].name_action, db_ctrl_lc[db_ctrl->action].do_action); 
        }
            break;
        case HOPE_DB_SCENE_TYPE:
        {
            cJSON_AddStringToObject(param, "模式", db_ctrl->name);
            if((db_ctrl->action == HOPE_DB_ACTION_CLOSE) || (db_ctrl->action == HOPE_DB_ACTION_OPEN))
                cJSON_AddStringToObject(param, db_ctrl_lc[db_ctrl->action].name_action, db_ctrl_lc[db_ctrl->action].do_action); 
        }
            break;
        
        default:
        goto Error;
            break;
    }

    out = cJSON_PrintUnformatted(root);
	print_mcd("get data %s", out);
    if(out != NULL)
    {
        // parse_speech_cmd(out);
        free(out);
        out = NULL;
    }
    
Error:
    if(root != NULL)
    {
        cJSON_Delete(root);
        root = NULL;
    }
}

static int _my_sort(int *a, int l)
{
	int i , j;
	int v;

	for(i = 0; i < l -1; i++)
	{
		for(j = i+1; j < l ; j++)
		{
			if(a[j] < a[i])
			{
				v = a[i];
				a[i] = a[j];
				a[j]  = v;
			}
		}
	}
}


int main(int argc, const char *argv[])
{
	char utf8_tmp_buf[128] = "}12123";
	int len = strlen(utf8_tmp_buf);
	int i = 10;
	all_db_info_t *temp;
	srv_msg_t *msg;
	db_ctrl_t db_ctrl;
	
	char buf[1024] = {0};
	int a[10] = {2,4,1,5,5,3,7,4,1,5};//乱序的数组。

	_my_sort(a, 10);
	for(i = 0; i < 10; i++)
		printf("%d ", a[i]);

	return 0;

	snprintf(buf, sizeof(buf), "%d", i);
	print_mcd("%s", buf);
	return 0;
	int k = 1;
	int kk = 2;
	while(1)
	{

		if(k == 1)
		{
			if(kk == 2)
				break;
		}
	}

	print_mcd("out");
return 0;
	// _get_file_size();

	// ctrl_db_device_scene();
	db_ctrl.action = HOPE_DB_ACTION_OPEN;
	db_ctrl.dev_type = HOPE_DB_LIGHT_TYPE;
	// db_ctrl.name = "廊灯"; 
	memcpy(db_ctrl.name, "廊灯", NAME_MAX_LEN);
	db_ctrl.attribute = 0;
	ctrl_db_device_scene(&db_ctrl);

	db_ctrl.action = 0;
	db_ctrl.dev_type = HOPE_DB_SCENE_TYPE;
	// db_ctrl.name = "廊灯"; 
	memcpy(db_ctrl.name, "回家", NAME_MAX_LEN);
	db_ctrl.attribute = 0;
	ctrl_db_device_scene(&db_ctrl);

	db_ctrl.action = HOPE_DB_ACTION_ADD;
	db_ctrl.dev_type = HOPE_DB_AIR_CONDITIONER_TYPE;
	// db_ctrl.name = "廊灯"; 
	memcpy(db_ctrl.name, "空调", NAME_MAX_LEN);
	db_ctrl.attribute = 0;
	ctrl_db_device_scene(&db_ctrl);

	db_ctrl.action = HOPE_DB_ACTION_MODE;
	db_ctrl.dev_type = HOPE_DB_AIR_CONDITIONER_TYPE;
	// db_ctrl.name = "廊灯"; 
	memcpy(db_ctrl.name, "空调", NAME_MAX_LEN);
	db_ctrl.attribute = MODE_COOL;
	ctrl_db_device_scene(&db_ctrl);

	db_ctrl.action = HOPE_DB_ACTION_SPEED;
	db_ctrl.dev_type = HOPE_DB_AIR_CONDITIONER_TYPE;
	// db_ctrl.name = "廊灯"; 
	memcpy(db_ctrl.name, "空调", NAME_MAX_LEN);
	db_ctrl.attribute = VALUE_HIGH;
	ctrl_db_device_scene(&db_ctrl);
#if 0
	all_db_info_t *info = (all_db_info_t *)msg->content;

	print_mcd("size = %d", sizeof(all_db_info_t *));
	// _get_file_size();
	_tmp_test(temp);
	srv_msg_t msg

	if(temp->info != NULL)
	{
		print_mcd("get memory = %p", temp->info);
		print_mcd("get num = %d", temp->num);
		// all_db_info_t tett;
		// memcpy(&tett, temp, sizeof(all_db_info_t));
		// print_mcd("get buf = %p", tett.info);
	}
		
	else
		print_mcd("get null memory");
	for(i = 0; i < temp->num; i++)
	{
		print_mcd("name is  = %s", temp->info[i]);
	}

	
	free(temp->info);
#endif
#if 0
	print_mcd("int time = %s", _print_time());
	int16_t test = 0xffff;
	if(test == 0xffff)
	{
		print_mcd("ok");
	}
	else
		print_mcd("error");
	for ( i = 0; i < strlen(utf8_tmp_buf); i++)
	{
		/* code */
		printf("%d ", utf8_tmp_buf[i]);
	}
	
	print_mcd("0x%4X", test);
	print_mcd("%d",strlen(utf8_tmp_buf));
	if(strspn(utf8_tmp_buf, "0123456789") != len)
	{
		print_mcd("number is wrong!!");
	}
	else
		print_mcd("number is right!!");

	print_mcd("end time = %s", _print_time());
	// print_mcd("ret = %d", ret);
	#endif
}