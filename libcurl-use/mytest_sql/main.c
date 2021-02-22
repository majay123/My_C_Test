
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* somewhat unix-specific */
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sqlite3.h>

#include "common.h"
#include "cJSON.h"

dataBaseManager_t dataBaseManager;

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