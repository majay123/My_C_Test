
#include <stdio.h>
#include <string.h>

/* somewhat unix-specific */
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sqlite3.h>

#define MAX_BUFFER_SIZE	2048

#define PRINT_MCD_ENABLE  1

typedef struct
{
	sqlite3 *db;
} dataBaseManager_t;

dataBaseManager_t dataBaseManager;
// static sqlite3 *dataBasedb;

#if PRINT_MCD_ENABLE == 1
    #define print_mcd(format, arg...)   do { printf("\033[31m[----mcd----]\033[0m:%s,%d--- "format"\n",__FILE__,__LINE__, ## arg);}\
                                        while (0)
#else
    #define print_mcd(format, arg...)   do {} while (0)
#endif

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


static int get_json_music_info()
{
	return 0;
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
	char *zErrMsg = 0;
	// int rc = 0;

	if(db == NULL)
	{
		print_mcd("sqlite3 db is null!!");
		return -1;
	}
#if 1
	/* 创建一个表，如果该表存在，则不创建，并给出提示，存储在zErrMsg中 */
	char *sql = " CREATE TABLE SensorData(ID INTEGER PRIMARY KEY, SensorID INTEGER, SiteNum INTEGER, Time VARCHAR(12), SensorParameter REAL );";
	sqlite3_exec(db, sql, 0, 0, &zErrMsg);
	print_mcd("%s", zErrMsg);

	//插入数据
	sql = "INSERT INTO \"SensorData\" VALUES(NULL , 1 , 1 , '200605011206', 18.9 );" ;  
 	sqlite3_exec( db , sql , 0 , 0 , &zErrMsg );  
   
	sql = "INSERT INTO \"SensorData\" VALUES(NULL , 23 , 45 , '200605011306', 16.4 );" ;  
	sqlite3_exec( db , sql , 0 , 0 , &zErrMsg );  
	
	sql = "INSERT INTO \"SensorData\" VALUES(NULL , 34 , 45 , '200605011306', 15.4 );" ;  
	sqlite3_exec( db , sql , 0 , 0 , &zErrMsg );  
	print_mcd("%s", zErrMsg);

	// sqlite3_close(db);
#endif
	return 0;
}

/**
* @details  	sqlite3 query 查询数据库中的数据
* @param[in]	
* @param[out]	
* @retval  		
* @brief		
* <date>       <version>         <author>          <brief>
*--------------------------------------------------------------------
* 202012091344 V1.0.0				MCD				create
*/
static int sqlite3_query(sqlite3 *db)
{
	char *zErrMsg = 0;
	// int rc = 0;
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
	char *sql = "SELECT * FROM SensorData ";
	sqlite3_get_table(db, sql, &azResult, &nrow, &ncol, &zErrMsg);

	int i = 0;
	print_mcd("nrow: %d, ncol: %d", nrow, ncol);
	print_mcd("The result of querying is : ");

	for(i = 0; i < (nrow + 1) * ncol; i++)
		printf("azResult[%d] = %s\n", i, azResult[i]);

	/* 释放 azResult 的内存空间 */
	sqlite3_free_table(azResult);

	print_mcd("zErrMsg  = %s", zErrMsg);
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
	// int rc;
	int nrow = 0, ncol = 0;
	char **azResult;	/* 二维数组存放结果 */
	
	if(db == NULL)
	{
		print_mcd("sqlite3 db is null!!");
		return -1;
	}	
	/* 删除数据 */
	char *sql = "DELETE FROM SensorData WHERE SensorID = 1 ;";
	sqlite3_exec(db, sql, 0, 0, &zErrMsg);
	print_mcd("zErrMsg = %s", zErrMsg);

	sql = "SELECT * FROM SensorData;";
	sqlite3_get_table(db, sql, &azResult, &nrow, &ncol, &zErrMsg);
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
	ret = sqlite3_create("mcdtest.db");
	if(ret != SQLITE_OK)
	{
		print_mcd("ret is not %d", ret);
		goto Error;
	}
	print_mcd("db = %p", dataBaseManager.db);
	if(dataBaseManager.db != NULL)
	{
		sqlite3_insert(dataBaseManager.db);
		sqlite3_query(dataBaseManager.db);
		sqlite3_delete(dataBaseManager.db);
	}
Error:
	sqlite3_myfree(dataBaseManager.db);
	return 0;
}