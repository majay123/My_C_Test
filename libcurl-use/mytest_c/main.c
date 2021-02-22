
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
#include <sqlite3.h>

#include <sys/sysinfo.h>

#include "common.h"
#include "cJSON.h"

dataBaseManager_t dataBaseManager;

struct control_json_map_t {
	uint8_t code;
    uint8_t param_type;
	const char *param_name;
	const char *param_content_string;
    uint8_t param_content_int;
};

static struct control_json_map_t control_json_table[] = {
    {MQTT_MSGCODE_CHECK_BIND_STATUS_REQ, cJSON_String, "bind_status", "check"},
    {MQTT_MSGCODE_BIND_ACTION_REQ, cJSON_String, "bind_action", NULL},
    {MQTT_MSGCODE_UNBIND_ACTION_REQ, cJSON_String, "unbind_action", "unbind"},
    {MQTT_MSGCODE_JOIN_ACTION_REQ, cJSON_Number, "pmtjn", NULL, 0},
    {MQTT_MSGCODE_LEAVE_ACTION_REQ, cJSON_String, "unbind_action", "unbind"},
    {MQTT_MSGCODE_PUSH_DEV_ACTION_REQ, cJSON_String, "push_action", "update dev"},
    {MQTT_MSGCODE_GET_ALL_DEV_REQ, cJSON_String, "get_alldev", "check"},
    {MQTT_MSGCODE_GET_ALL_SCENE_REQ, cJSON_String, "get_allscene", "check"},
    {MQTT_MSGCODE_PUSH_SCENE_ACTION_REQ, cJSON_String, "push_action", "update scene"},
    {MQTT_MSGCODE_TOTAL_DEV_ACTION_REQ, cJSON_String, "get_action", "total dev"},
    {MQTT_MSGCODE_TOTAL_SCENE_ACTION_REQ, cJSON_String, "get_action", "total scene"},
    {MQTT_MSGCODE_POP_ALL_ACTION_REQ, cJSON_String, "pop_action", "pop all"},
};

static unsigned int g_mqtt_serial = 0;

static int write_music_info(char *id, char *name, char *url, char *sname);

uint64_t get_now_us(void)
{
    struct timeval tv;

    //clock_gettime(CLOCK_REALTIME, &tv);
    gettimeofday(&tv, NULL);
    return (uint64_t)tv.tv_sec * 1000000ll + tv.tv_usec;
}

uint64_t get_now_ms(void)
{
    return (get_now_us() / 1000);
}

uint64_t get_now_s(void)
{
    return (get_now_ms() / 1000);
}



static int get_devices_name(char *driver_name, char *device_name)
{
	int fd;
	int ret;
	char buffer[MAX_BUFFER_SIZE];
	char key[100];
	char *p1, *p2, *p3;

	fd = open("devices", O_RDONLY);

	if(fd < 0)
	{
		print_mcd("open file fail: devices");
		return -1;
	}

	memset(buffer, 0, sizeof(buffer));
	ret = read(fd, buffer, MAX_BUFFER_SIZE - 1);
	if(ret < 0)
	{
		print_mcd("read file fail: devices");
		return -1;
	}

	buffer[ret] = 0;
	close(fd);

	sprintf(key, "N: Name=\"%s", driver_name);
	p1 = strstr(buffer, key);
	print_mcd("get p1 = %s", p1);
	if(NULL == p1)	return -1;

	p3 = strstr(p1, "H: Handlers=kbd ");
	print_mcd("get p3 = %s", p3);
	if(NULL == p3)
	{
		p3 = strstr(p1, "H: Handlers=");
		if(NULL == p3)	return -1;
		p1 = p3;
		p1 += strlen("H: Handlers=");
		while(*p1 == ' ') p1++;
	}
	else
	{
		p1 = p3;
		p1 += strlen("H: Handlers=kdb ");
		while(*p1 == ' ') p1++;
	}
	
	p2 = p1;
	print_mcd("p2 get = %s", p2);
	while((*p2 != ' ') && (*p2 != '\n') && (*p2 != 0)) p2++;

	*p2 = 0;
	sprintf(device_name, "/dev/input/%s", p1);
	print_mcd("find %s device: %s", driver_name, device_name);

	return 0;
}


static int sqlite3_insert_musicinfo(char *filename)
{
	cJSON *root = NULL;
	int fd = -1;
	char buffer[MAX_BUFFER_SIZE * 50] = { 0 };

	print_mcd("do insert info");

	fd = open(filename, O_RDONLY);
	if(fd < 0)
	{
		print_mcd("open fd failed!!");
		return -1;
	}
	
	int size = read(fd, buffer, MAX_BUFFER_SIZE*50 - 1);
	if(size < 0)
	{
		print_mcd("read fd failed!!");
		return -1;
	}
	print_mcd("get size = %d", size);
	buffer[size] = 0;
	close(fd);

	// print_mcd("%s", buffer);
	root = cJSON_Parse(buffer);
	if(root == NULL)
	{
		print_mcd("parse json failed!!");
		return -1;
	}

	// REQ_JSON_OBJ(root, "rows", Error);
	cJSON *code_js = cJSON_GetObjectItem(root, "code");
	REQUIRE(code_js == NULL, Error);

	if(code_js->valueint == 100000)
	{
		cJSON *rows_js = cJSON_GetObjectItem(root, "rows");
		REQUIRE(rows_js == NULL, Error);
		size = cJSON_GetArraySize(rows_js);
		if(size > 0)
		{
			for (int i = 0; i < size; i++)
			{
				cJSON *music_js = cJSON_GetArrayItem(rows_js, i);
				if(music_js == NULL)	continue;
				cJSON *musicId_js = cJSON_GetObjectItem(music_js, "musicId");
				cJSON *musicName_js = cJSON_GetObjectItem(music_js, "musicName");
				cJSON *picUrl_js = cJSON_GetObjectItem(music_js, "picUrl");
				cJSON *singerName_js = cJSON_GetObjectItem(music_js, "singerName");

				if(musicId_js != NULL && musicName_js != NULL && singerName_js != NULL && picUrl_js != NULL)
				{
					// printf("i = %d, name = %s\n", i, musicName_js->valuestring);
					write_music_info(musicId_js->valuestring, musicName_js->valuestring, picUrl_js->valuestring, singerName_js->valuestring);
				}
			}
			
		}
	}
	
Error:
	if(root != NULL)
	{
		cJSON_Delete(root);
		root = NULL;
	}
	
	return 0;
}

static int write_music_info(char *id, char *name, char *url, char *sname)
{
	int rc = 0;
	char *zErrMsg = NULL;
	char *sql = NULL;
	int ret = 0;

	sql = sqlite3_mprintf("INSERT INTO test_music_info(musicId, musicName, picUrl, singerName) VALUES(\"%s\", \"%s\", \"%s\", \"%s\");", id, name, url, sname);
	// print_mcd("sql = %s", sql);
	rc = sqlite3_exec(dataBaseManager.db, sql, 0, 0, &zErrMsg);
	if(sql != NULL)
	{
		sqlite3_free(sql);
		sql = NULL;
		ret = -1;
	}
	
	if(rc != SQLITE_OK)
	{
		print_mcd("%s", zErrMsg);
		if(zErrMsg != NULL)
		{
			sqlite3_free(zErrMsg);
			zErrMsg = NULL;
			ret = -1;
		}
	}

	return ret;

}

static int sqlite3_create(const char *filename)
{
	// char *zErrMsg = 0;
	sqlite3 *db = NULL;
	int rc = 0;

	/* 打开指定的数据库文件，如果不存在将创建一个同名的数据文件 */
	rc = sqlite3_open(filename, &dataBaseManager.db);
	print_mcd("get rc = %d", rc);
	if(rc != SQLITE_OK)
	{
		fprintf(stderr, "can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return rc;
	}
	else
	{
		print_mcd("You have opened a sqlite3 database named zieckey.db successfully!");
		print_mcd("Congratulations! Have fun! ^-^ ");
	}
	return rc;
}

/**
* @details  	sqlite3 insert插入数据库
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 202012091307 V1.0.0				MCD				create
*/
static int sqlite3_insert(sqlite3 *db)
{
	// sqlite3 *db = NULL;
	char *zErrMsg = NULL;
	char *sql = NULL;
	int rc = 0;

	if(db == NULL)
	{
		print_mcd("sqlite3 db is null!!");
		return -1;
	}
#if 1
	/* 创建一个表，如果该表存在，则不创建，并给出提示，存储在zErrMsg中 */
	// char *sql = " CREATE TABLE SensorData(ID INTEGER PRIMARY KEY, SensorID INTEGER, SiteNum INTEGER, Time VARCHAR(12), SensorParameter REAL );";
	sql = sqlite3_mprintf("CREATE TABLE if not exists test_music_info("
							"musicId		TEXT NOT NULL,"
							"musicName		TEXT NOT NULL,"
							"picUrl			TEXT NOT NULL,"
							"singerName		TEXT NOT NULL);");
	rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
	if(sql != NULL)
	{
		sqlite3_free(sql);
		sql = NULL;
	}
	
	if(rc != SQLITE_OK)
	{
		print_mcd("%s", zErrMsg);
		if(zErrMsg != NULL)
		{
			sqlite3_free(zErrMsg);
			zErrMsg = NULL;
		}
	}

	// sqlite3_close(db);
#endif
	return 0;
}

/**
* @details  	sqlite3 query 查询数据库中的数据, 通过歌曲的名字，获取歌曲信息
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 202012091344 V1.0.0				MCD				create
*/
static int sqlite3_query(sqlite3 *db, char *name)
{
	char *zErrMsg = 0;
	int rc = 0;
	int nrow = 0, ncol = 0;
	char **azResult;	/* 二维数组存放结果 */
	
	if(db == NULL)
	{
		print_mcd("sqlite3 db is null!!");
		return -1;
	}
	// 查询数据
	/* int sqlite3_get_table(
	*	sqlite3 *db,           //An open database 
	*	const char *zSql,      //SQL to be evaluated *
	*	char ***pazResult,     //Results of the query 
	*	int *pnRow,            //Number of result rows written here 
	*	int *pnColumn,         //Number of result columns written here 
	*	char **pzErrmsg        //Error msg written here 
	*	) 
	*/
	/* result中是以数组的形式存放你所查询的数据，首先是表明，再是数据
	nrow，ncol分别为查询语句返回的结果集的行数，列数，没有查到结果时返回0 */
	char *sql = "SELECT * FROM test_music_info";	//获取说的歌曲信息
	// char *sql = "SELECT musicName FROM test_music_info";	//获取所有的歌曲名字

	// 通过歌曲名字获取的歌曲信息
	// char *sql = sqlite3_mprintf("SELECT musicId FROM test_music_info  WHERE musicName = \"%s\"", name);
	rc = sqlite3_get_table(db, sql, &azResult, &nrow, &ncol, &zErrMsg);
	if(rc != SQLITE_OK)
	{
		print_mcd("%s", zErrMsg);
		if(zErrMsg != NULL)
		{
			sqlite3_free(zErrMsg);
			zErrMsg = NULL;
		}
	}

	print_mcd("nrow: %d, ncol: %d", nrow, ncol);
	// print_mcd("The result of querying is : ");

	for(int i = 0; i < (nrow + 1) * ncol; i++)
		printf("azResult[%d] = %s\n", i, azResult[i]);

	/* 释放 azResult 的内存空间 */
	sqlite3_free_table(azResult);

	// print_mcd("zErrMsg  = %s", zErrMsg);
	// sqlite3_close(db);

	return 0;
}

/**
* @details  	sqlite3 delete 删除数据库中的特定的数据
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 202012091424 V1.0.0				MCD				create
*/
static int sqlite3_delete(sqlite3 *db)
{
	char *zErrMsg = 0;
	int rc;
	int nrow = 0, ncol = 0;
	char **azResult;	/* 二维数组存放结果 */
	
	if(db == NULL)
	{
		print_mcd("sqlite3 db is null!!");
		return -1;
	}	
	/* 删除数据 */
	char *sql = "DELETE FROM test_music_info WHERE musicId = 1 ;";
	rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
	if(rc != SQLITE_OK)
	{
		print_mcd("%s", zErrMsg);
		if(zErrMsg != NULL)
		{
			sqlite3_free(zErrMsg);
			zErrMsg = NULL;
		}
	}
	

	sql = "SELECT * FROM test_music_info;";
	rc = sqlite3_get_table(db, sql, &azResult, &nrow, &ncol, &zErrMsg);
	if(rc != SQLITE_OK)
	{
		print_mcd("%s", zErrMsg);
		if(zErrMsg != NULL)
		{
			sqlite3_free(zErrMsg);
			zErrMsg = NULL;
		}
	}

	int i = 0;
	print_mcd("nrow: %d, ncol: %d", nrow, ncol);
	print_mcd("After deleting , the result of querying is : ");

	for(i = 0; i < (nrow + 1) * ncol; i++)
		printf("azResult[%d] = %s\n", i, azResult[i]);

	sqlite3_free_table(azResult);
	// sqlite3_close(db);

	return 0;
	
}

static void sqlite3_myfree(sqlite3 *db)
{
	if(db != NULL)
	{
		sqlite3_close(db);
		db = NULL;
	}
	else
		db = NULL;
}

#if 0
int main(int argc, char *argv[])
{
	int ret = 0;
	// char deviceName[100] = { 0 };
	// char driveName[20] = "gpio-keys";

	// get_devices_name(driveName, deviceName);
	// print_mcd("get deviceName = %s", deviceName);
	
	// int ret = strcasecmp(argv[1], argv[2]);
	// print_mcd("ret = %d", ret);
	// get_timestamp()
	int64_t now = get_now_ms();
	ret = sqlite3_create("mcdtest.db");
	if(ret != SQLITE_OK)
	{
		print_mcd("ret is not %d", ret);
		goto Error;
	}
	// print_mcd("db = %p", dataBaseManager.db);
	if(dataBaseManager.db != NULL)
	{
		sqlite3_insert(dataBaseManager.db);
		sqlite3_insert_musicinfo("music1.json");
		// print_mcd("do time = %ld", get_now_ms() - now);
		sqlite3_query(dataBaseManager.db, "东石(“透南风” Live / Taipei Arena / 2018)");
		// sqlite3_delete(dataBaseManager.db);
	}
Error:
	print_mcd("end time = %ld", get_now_ms() - now);
	sqlite3_myfree(dataBaseManager.db);
	return 0;
}
#else

int my_itoa(int num, char *str, int radix)
{
    int tmp = num;
    int mod = 0;
    char *p = str;
	
    if((radix > 16 || radix < 2) || (num < 0 && radix != 10))
        return -1;

    while(tmp != 0)
    {
        mod = tmp % radix;
        if(mod >= 10)
            *p = (mod - 10) + 'a';
        else
            *p = '0' + mod;

        tmp /= radix;
        p++;
    }	
	*p = '\0';


    return 0;
}



static int _print_malloc(void)
{
	FILE   *fp;
	char   buf[1024];

	fp=popen("cat /proc/meminfo | grep MemAvailable:|sed -e 's/.*:[^0-9]//'","r");
	if(fp < 0)
	{
		printf("无法读取ram信息\n");
    	exit(1);
	}
	fread( buf, sizeof(char), sizeof(buf),  fp);	
	print_mcd("fp = %s", buf);
}


static char *_test_json_creat(device_t *devices_info)
{

    char *out;
    cJSON *root;
    cJSON *payload;
    cJSON *control;
    cJSON *allDeviceList;
    cJSON *item;
    int i;
    int count = 0;
    // device_t *devices_info = &scene->sys_get_devices;
    // if(devices_info != NULL) 
    //     count = devices_info->num;
	print_mcd("num = %d", devices_info->num);

    REQUIRE((root = cJSON_CreateObject()) == NULL, Error);
	REQUIRE((allDeviceList = cJSON_CreateArray()) == NULL, Error);
    cJSON_AddItemToObject(root, "allDeviceList", allDeviceList);
	for(i = 0; i < devices_info->num; i++)
	{
		cJSON *item = cJSON_CreateObject();
		if(item == NULL)    continue;
		if(devices_info->device_info[i].isDelete == 1)
		{
			// print_mcd("===");
			cJSON_AddStringToObject(item, "deviceName", devices_info->device_info[i].name);
			cJSON_AddNumberToObject(item, "eleCata", devices_info->device_info[i].eleCata);
			cJSON_AddStringToObject(item, "deviceId", devices_info->device_info[i].deviceId);
			cJSON_AddItemToArray(allDeviceList, item);
		}
		
	}

	out = cJSON_PrintUnformatted(root);
    REQUIRE(out == NULL, Error);

	print_mcd("out = %s", out);
	if(out != NULL)
	{
		free(out);
		out = NULL;
	}

    if(root !=  NULL) {
        cJSON_Delete(root);
        root = NULL;
    }
	return NULL;
Error:
	print_mcd("error");
	if(out != NULL)
	{
		free(out);
		out = NULL;
	}
	if(root != NULL)
	{
		cJSON_Delete(root);
		root = NULL;
	}
	return NULL;
}


static int _eq_device_get_info(device_t *device, int count)
{
	int num = 8;

	// device_info_t tmp[] ={{0}};
    device->num = 7;
    device->device_info[0].dev_type = 1;
    device->device_info[0].isDelete = 0;
    device->device_info[0].isHomepage = 1;
    memcpy(&device->device_info[0].name, "烟雾传感器", DEVICE_NAME_LEN);

    device->device_info[1].dev_type = 5;
    device->device_info[1].isDelete = 1;
    device->device_info[1].isHomepage = 0;
    memcpy(&device->device_info[1].name, "开关1", DEVICE_NAME_LEN);

    device->device_info[2].dev_type = 2;
    device->device_info[2].isDelete = 1;
    device->device_info[2].isHomepage = 0;
    memcpy(&device->device_info[2].name, "温度传感器", DEVICE_NAME_LEN);

    device->device_info[3].dev_type = 0;
    device->device_info[3].isDelete = 0;
    device->device_info[3].isHomepage = 1;
    memcpy(&device->device_info[3].name, "移动传感器", DEVICE_NAME_LEN);

    device->device_info[4].dev_type = 5;
    device->device_info[4].isDelete = 0;
    device->device_info[4].isHomepage = 0;
    memcpy(&device->device_info[4].name, "开关2", DEVICE_NAME_LEN);

    device->device_info[5].dev_type = 7;
    device->device_info[5].isDelete = 0;
    device->device_info[5].isHomepage = 0;
    memcpy(&device->device_info[5].name, "环境传感器", DEVICE_NAME_LEN);

    device->device_info[6].dev_type = 6;
    device->device_info[6].isDelete = 0;
    device->device_info[6].isHomepage = 1;
    memcpy(&device->device_info[6].name, "窗帘1", DEVICE_NAME_LEN);

    return 0;
}
static int test(device_t *devices)
{
	print_mcd("%p", &devices);
	print_mcd("%d, %d, %d, %s", devices->device_info[0].dev_type, devices->device_info[0].isHomepage, devices->device_info[0].isDelete, devices->device_info[0].name);
	devices->device_info[0].dev_type = 33;
	// free(devices);
	// devices = NULL;
}


static void _create_root_cjson(cJSON *root)
{
    cJSON_AddStringToObject(root, "application", MQTT_APPLICATUIN);
    cJSON_AddStringToObject(root, "port", MQTT_PORT);
    cJSON_AddStringToObject(root, "destination", MQTT_DESTINATION);
}

static void _creat_payload_cjson(cJSON *payload, int code)
{
    cJSON_AddNumberToObject(payload, "code", code);
    cJSON_AddNumberToObject(payload, "serial", g_mqtt_serial++);
}

static int _ceart_control_cjson(cJSON *control, mqtt_msg_t *mqtt_msg)
{
    int i = 0;
    cJSON *allDeviceList;

    for(i = 0; i < ARRAY_SIZE(control_json_table); i++)
    {
        if(control_json_table[i].code == mqtt_msg->code)
        {
            if(control_json_table[i].param_type == cJSON_String)
            {
                if(MQTT_MSGCODE_BIND_ACTION_REQ == control_json_table[i].code)
                {
                    cJSON_AddStringToObject(control, control_json_table[i].param_name, (mqtt_msg->aciton == true) ? "bind start" : "bind stop");
                }
                else
                {
                    cJSON_AddStringToObject(control, control_json_table[i].param_name, control_json_table[i].param_content_string);
                    if(MQTT_MSGCODE_GET_ALL_DEV_REQ == control_json_table[i].code || MQTT_MSGCODE_GET_ALL_SCENE_REQ == control_json_table[i].code)
                    {
                        cJSON_AddNumberToObject(control, "startIndex", mqtt_msg->startIndex);
                        cJSON_AddNumberToObject(control, "itemCount", mqtt_msg->itemCount);
                    }
                    // if(MQTT_MSGCODE_LEAVE_ACTION_REQ == control_json_table[i].code || \
                    //    MQTT_MSGCODE_PUSH_DEV_ACTION_REQ == control_json_table[i].code || \
                    //    MQTT_MSGCODE_PUSH_SCENE_ACTION_REQ == control_json_table[i].code)
                    // {
                    //     REQUIRE((allDeviceList = cJSON_CreateArray()) == NULL, Error);
                    //     cJSON_AddItemToObject(control, "allDeviceList", allDeviceList);
                    // }
                }
            }
            else if(control_json_table[i].param_type == cJSON_Number)
            {
                if(MQTT_MSGCODE_JOIN_ACTION_REQ == control_json_table[i].code)
                {
                    cJSON_AddNumberToObject(control, control_json_table[i].param_name, mqtt_msg->join_time);
                }
            }
            
            return 0;
        }
    }

Error:
    return -1;
}


char * mqtt_msg_control_create(void *param, mqtt_msg_t *mqtt_msg)
{
    char *out;
    cJSON *root;
    cJSON *payload;
    cJSON *control;
    cJSON *allDeviceList;
    cJSON *item;
    int i;
    int count = 0;
    // device_t *devices_info = &scene->sys_get_devices;
    // if(devices_info != NULL) 
    //     count = devices_info->num;
print_mcd("0-21");
    REQUIRE((root = cJSON_CreateObject()) == NULL, Error);
    _create_root_cjson(root);
print_mcd("0");
    REQUIRE((payload = cJSON_CreateObject()) == NULL, Error);
    cJSON_AddItemToObject(root, "payload", payload);
    _creat_payload_cjson(payload, mqtt_msg->code);
    print_mcd("1");
    REQUIRE((control = cJSON_CreateObject()) == NULL, Error);
    cJSON_AddItemToObject(payload, "control", control);
print_mcd("2");
    REQUIRE((_ceart_control_cjson(control, mqtt_msg)) == -1, Error);
print_mcd("3");
	out = cJSON_PrintUnformatted(root);
    REQUIRE(out == NULL, Error);
	print_mcd("%p out = %s", &out,out);
	// free(out);
	// out = NULL;
    if(root !=  NULL) {
        cJSON_Delete(root);
        root = NULL;
    }
    return out;

Error:
    if(root !=  NULL) {
        cJSON_Delete(root);
        root = NULL;
    }
    return NULL;
}

static int _fill_info(device_info_t *dev_info, device_info_t *to)
{
	memset(dev_info, 0, sizeof(device_info_t));
	snprintf(dev_info->name, DEVICE_NAME_LEN, "%s", to->name);
	snprintf(dev_info->deviceId, DEVICE_ID_LEN, "%s", to->deviceId);
	dev_info->dev_type = to->dev_type;
	dev_info->eleCata = to->eleCata;
}

device_info_t dev_test1;

// static int f(int i)
// {
// 	printf("get i = %d, %d, %d, %d\n", i, i++, i++, i++);
// }
// int main(int argc, char **argv)
// {
// 	f(1);
// 	return 0;
// }
#if 0
int main(int argc, char *argv[])
{
	int num = 781;
	char str[100] = {0};
	int i = 0;
	
	cJSON *root = NULL;
    cJSON *payload;
    cJSON *control;
    cJSON *allDeviceList;
    cJSON *item;
	int k = 1;
	unsigned char tmp = 255;
	uint8_t tmp1 = 255;
	uint8_t tmp2;
	mqtt_msg_apps_t msg;
	char *tmp3 = NULL;
	uint8_t tmp4 = 0;
	memset(&msg, 0, sizeof(mqtt_msg_apps_t));
	print_mcd("%p", msg.content);
	msg.code = 1;
	msg.size = sizeof(device_info_t);
	msg.content = (device_info_t *)calloc(1, sizeof(device_info_t) + 1);
	device_info_t *test_devinfo = (device_info_t *)calloc(1, sizeof(device_info_t) + 1);
	// tmp3 = (uint8_t *)calloc(1, sizeof(uint8_t));
	device_info_t dev_test;//= (device_info_t *)msg.content;
	memset(&dev_test, 0, sizeof(device_info_t));

	dev_test.dev_type = tmp1;
	dev_test.eleCata = 11111;
	snprintf(dev_test.name, DEVICE_NAME_LEN, "%s", "这是开关哦");
	snprintf(dev_test.deviceId, DEVICE_ID_LEN, "%s", "111232131243214123");
	print_mcd("%d, %d, %s, %s", dev_test.dev_type, dev_test.eleCata, dev_test.name, dev_test.deviceId);
	print_mcd("%p", msg.content);
	memcpy(msg.content, &dev_test, sizeof(device_info_t));

	device_info_t *get_devices =( device_info_t *) msg.content;
	print_mcd("%d, %d, %s, %s", get_devices->dev_type, get_devices->eleCata, get_devices->name, get_devices->deviceId);
	print_mcd("%p", get_devices);
	
	// _fill_info(&dev_test1, get_devices);
	memcpy(&dev_test1, msg.content, sizeof(device_info_t));
	print_mcd("%p", &dev_test1);
	print_mcd("%d, %d, %s, %s", dev_test1.dev_type, dev_test1.eleCata, dev_test1.name, dev_test1.deviceId);
	if(msg.content)
	{
		free(msg.content);
		msg.content = NULL;
	}
	print_mcd("%p", &dev_test1);
	print_mcd("%d, %d, %s, %s", dev_test1.dev_type, dev_test1.eleCata, dev_test1.name, dev_test1.deviceId);
	#if 0
	int kk = 1;
	switch (kk)
	{
		case 1:
		{
			print_mcd("test");
			if(1)	
			{
				print_mcd("aada");
				break;
			}
			print_mcd("kkkk");
		}
			break;
	}
#endif
	// memcpy(tmp3, &tmp1, sizeof(uint8_t));
	// memcpy(&tmp4, tmp3, sizeof(uint8_t));
	// print_mcd("%d, %p, %p, %d, %d", dev_test->dev_type, msg.content, dev_test, tmp2, tmp4);
	// char *out = (char *)malloc(1024);
	// device_t *sys_get_devices;
	// device_t* tmp_devs = (device_t *)malloc(sizeof(device_t));
	// memset(tmp_devs, 0, sizeof(device_t));
	// print_mcd("%p", &tmp_devs);
	// _eq_device_get_info(tmp_devs, 7);
	// mqtt_msg_t mqtt_msg;// = (mqtt_msg_t *)malloc(sizeof(mqtt_msg_t));
	// // memset(&mqtt_msg, 0, sizeof(mqtt_msg));
	// print_mcd("0-1");
	// mqtt_msg.code = 11;
	// print_mcd("111");
	// char *out = mqtt_msg_control_create(NULL, &mqtt_msg);
	// root = REQ_JSON_PARSE(out, root, Error);
	// cJSON *application = REQ_JSON_OBJ(root, application, Error);
	// int size = application->valueint;
	// print_mcd("%d, %s", application->valueint, application->valuestring);
#if 0
	int ttt = strcmp("uncheck", "check");
	print_mcd("ttt = %d", ttt);
	for (int i = 0; i < 20; i++)
	{	
		print_mcd("====");
		if(i > 2 && i < 11)
		{
			
			if(i == 3 || i == 10)
			{
				print_mcd("%d", i);
				goto Continue;
			}
				
		}
		else 
		{
		Continue:
			print_mcd("jump here");
			continue;
		}
		if(i > 4 && i < 8 )
		{
			
			if(i == 6 || i == 7)
			{
				print_mcd("%d", i);
				print_mcd("want jump");
				goto Continue1;
			}
				
			else
			{
			Continue1:
				print_mcd("jump here11");
				continue;
			}
		}
		
	}
	
Error:
print_mcd("root %p", root);
	if(root !=  NULL) {
		print_mcd("delete root");
        cJSON_Delete(root);
        root = NULL;
    }
#endif
	// print_mcd("%p = %s", &out, out);
	
	// memcpy(&sys_get_devices, tmp_devs, sizeof(device_t));
	
	// memset(sys_get_devices, 0, sizeof(sys_get_devices));
	// print_mcd("%d KB", sizeof(device_t) / 1024);
	// print_mcd("%d KB", sizeof(scene_t) / 1024);
	// print_mcd("%d", strlen("K6情景面板1"));
	// print_mcd("%d", strlen("厨房开关三"));
	// print_mcd("%d", strlen("主卧客厅名版有人正在"));
	// print_mcd("%p", &tmp_devs);
	// // sys_get_devices = tmp_devs;
	// test(tmp_devs);
	// print_mcd("%d, %d, %d, %s", tmp_devs->device_info[0].dev_type, tmp_devs->device_info[0].isHomepage, tmp_devs->device_info[0].isDelete, tmp_devs->device_info[0].name);
	// if(tmp_devs != NULL)
	// 	print_mcd("%p", &tmp_devs);
	// else
	// 	print_mcd("freed");
	// // _eq_device_get_info(&sys_get_devices);
	// _test_json_creat(&sys_get_devices);
#if 1
	printf("char size = %x, %d\n", tmp1, sizeof(tmp1));
	_print_malloc();

    REQUIRE((root = cJSON_CreateObject()) == NULL, Error);
    cJSON_AddStringToObject(root, "application", "gwui");
    cJSON_AddStringToObject(root, "port", "ff");
    cJSON_AddStringToObject(root, "destination", "gwservice");
	REQUIRE((payload = cJSON_CreateObject()) == NULL, Error);
	REQUIRE((control = cJSON_CreateObject()) == NULL, Error);
	cJSON_AddItemToObject(root, "payload", payload);
	cJSON_AddItemToObject(payload, "control", control);

	REQUIRE((allDeviceList = cJSON_CreateArray()) == NULL, Error);
	cJSON_AddItemToObject(control, "allDeviceList", allDeviceList);
	_print_malloc();
	
	for(i = 0; i< 1; i++)
	{
		REQUIRE((item = cJSON_CreateObject()) == NULL, Error);
		// print_mcd("===");
		cJSON_AddItemToArray(allDeviceList, item);
		cJSON_AddStringToObject(item, "deviceName", "K6情景面板");
		cJSON_AddNumberToObject(item, "eleCata", 114099);
		cJSON_AddStringToObject(item, "mainPage", (k == 1) ? "check" : "uncheck");
		cJSON_AddStringToObject(item, "deviceId", "010100124b0022313d5b");

		
	}
	

	

	_print_malloc();

	// itoa(num, str, 10);
	// my_itoa(num, str, 16);
	// print_mcd("str = %s", str);
	char *out = cJSON_Print(root); 
	print_mcd("%s", out);
	free(out);
Error:
	if(root !=  NULL) {
		print_mcd("delete root");
        cJSON_Delete(root);
        root = NULL;
    }
	_print_malloc();
#endif
	return 0;
}
#endif
#endif