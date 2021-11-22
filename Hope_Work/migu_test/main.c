#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "migu.h"
#include "local_common.h"
#include "cJSON.h"
#include "common.h"

#define KEY	"d98ec9e4cb95c945e50b8b1abbfdc3de"
#define  MALLOC_SIZE			(1024 * 256)
#define  MALLOC_DEFULT_SIZE		(1024 * 50)


static int _try_to_active(void)
{
	cJSON *root = NULL;
	cJSON *root1 = NULL;
	int size = 0;
	char * result = calloc(MALLOC_SIZE, sizeof(char));

	if(!result) {
		printf("calloc failed!\n");
		return -1;
	}

	get_music_newsong(result);
	if(!result) {
		printf("get music new song failed!\n");
		return -1;
	}

	root = cJSON_Parse(result);
	if(!root) {
		printf("cjson parse failed\n");
		return -1;
	}
	size = cJSON_GetArraySize(root);
	if(size <= 0) {
		printf("get json array size = %d failed\n", size);
		return -1;
	}
	printf("size = %d\n", size);
	cJSON *one_item = cJSON_GetArrayItem(root, 0);
	if(!one_item) {
		printf("get one item failed!\n");
		return -1;
	}
	cJSON *songIds =  cJSON_GetObjectItem(one_item, "songIds");
	if(!songIds) {
		printf("get song ids failed\n");
		return -1;
	}

	size = cJSON_GetArraySize(songIds);
	if(size <= 0) {
		printf("get song ids size = %d failed\n", size);
		return -1;
	}
	printf("size = %d\n", size);
	cJSON *id = cJSON_GetArrayItem(songIds, 0);
	if(!id) {
		printf("get id failed!\n");
		return -1;
	}
	printf("id = %s\n", id->valuestring);
	
	memset(result, 0, MALLOC_SIZE);
	get_musicInfo(id->valuestring, NULL, result);
	if(!result) {
		printf("get music info failed!\n");
		return -1;
	}
	printf("result: %s\n",result);
	root1 = cJSON_Parse(result);
	// "resCode": "000000"
	cJSON *resCode = cJSON_GetObjectItem(root1, "resCode");
	if(strcmp(resCode->valuestring, "000000") != 0) {
		printf("get music info error!\n");
		return -1;
	}

	if(root1) {
		cJSON_Delete(root1);
	}
	if(root) {
		cJSON_Delete(root);
	}
	free(result);

	return 0;
}


//对应乐库的分类
void *_ger_and_parse_radio_info(void *arg)
{
	char * result = malloc(MALLOC_SIZE);
	int ret = 0;
	cJSON *root = NULL;
	int i = 0;
	int j = 0;

	if(!result) {
		print_mcd("error result!");
		return NULL;
	}
	ret = get_radio_info(result);	//对应乐库的分类
	if(ret != 0) {
		print_mcd("error get_radio_info!");
		goto Error;
	}

	root = cJSON_Parse(result);
	if(root == NULL) {
		print_mcd("error cjosn parse");
		goto Error;
	}
	int size = cJSON_GetArraySize(root);
	print_mcd("get array size = %d", size);
	for(i = 0; i < 1; i++) {
		cJSON *item = cJSON_GetArrayItem(root, i);
		cJSON *columnId;
		cJSON *columnName;
		cJSON *radios;
		// print_mcd("get item!");
		columnId = REQ_JSON_OBJ(item, columnId, Error);
		columnName = REQ_JSON_OBJ(item, columnName, Error);
		radios = REQ_JSON_OBJ(item, radios, Error);

		if(columnId->type == cJSON_String)
			print_mcd("id = %s", columnId->valuestring);
		else {
			print_mcd("type is not a string %d", columnId->type);
		}
		if(columnName->type == cJSON_String)
			print_mcd("name = %s", columnName->valuestring);
		else {
			print_mcd("type is not a string %d", columnName->type);
		}
		int size1 = cJSON_GetArraySize(radios);
		print_mcd("get array size = %d", size1);
		for(j = 0; j < 1; j++) {
			cJSON *item1 = cJSON_GetArrayItem(radios, j);
			cJSON *radioId = REQ_JSON_OBJ(item1, radioId, Error);
			cJSON *radioName = REQ_JSON_OBJ(item1, radioName, Error);
			cJSON *songIds = REQ_JSON_OBJ(item1, songIds, Error);
			print_mcd("rId = %s", radioId->valuestring);
			print_mcd("rname = %s", radioName->valuestring);
			int size2 = cJSON_GetArraySize(songIds);
			print_mcd("size2 = %d", size2);
			// int k= 0;
			// size_t offset = 0;
			// char songids[1024 *10] = {0};
			// char *out = calloc(1024, sizeof(char));
			// out = cJSON_Print(songIds);
			// print_mcd("songids = %s",out);
			// free(out);
		#if 0
			for ( k = 0; k < 1; k++) {
				cJSON *songid = cJSON_GetArrayItem(songIds, k);
				// print_mcd("songid = %s", songid->valuestring);
				// offset += snprintf(songids + offset, sizeof(songids), "%s,", songid->valuestring);
				// print_mcd("songids = %s", songids);
				char *result2 = calloc(1024*50, 1);
				get_musicInfo("6005750X5DM", "S", result2);
				print_mcd("result2 = %s", result2);
				free(result2);
			}
		#else
			char *result2 = calloc(1024*50, 1);
			get_musicInfo("6005750X5DM", "S", result2);
			print_mcd("result2 = %s", result2);
			free(result2);
		#endif

		}

		printf("=============================================\n");
		// print_mcd("columnId = %s, columnName = %s", columnId->valuestring, columnName->valuestring);
	}

Error:
	if(result) free(result);
	if(root) cJSON_Delete(root);

	return NULL;
}

void *_get_and_parse_music_rank(void *arg)
{
	char * result1 = malloc(MALLOC_DEFULT_SIZE);
	if(result1 == NULL)
		return NULL;
	get_music_rank(result1);	//对应乐库的榜单
	print_mcd("%s\n",result1);
	if(result1 != NULL)
		free(result1);
	return NULL;
}

void *_get_and_parse_music_newalbum(void *arg)
{
	char * result1 = malloc(MALLOC_DEFULT_SIZE);
	if(result1 == NULL)
		return NULL;
#if 1
	get_music_newalbum(result1);	//对应乐库的榜单
	get_albumInfo("1139069260", "1", "30", "S", result1);
	print_mcd("%s\n",result1);
	// memset(result1, 0, MALLOC_DEFULT_SIZE);
	// get_albumInfo("1139069260", "2", "30", "S", result1);
	// print_mcd("%s\n",result1);
#else
	get_music_playlistId("205882236", result1);
	print_mcd("%s\n",result1);
#endif
	if(result1 != NULL)
		free(result1);
	return NULL;
}

int main(int argc, char const *argv[])
{
	int ret = -1;
	// char device_info [] = "{\"uid\":\"9ac8ccbe-40f2-430b-bf19-2eaea389143d\",\"mac\":\"9C:41:7C:A0:1B:88\",\"smartDeviceId\":\"44401007240347\"}";	//old sn
	char device_info [] = "{\"uid\":\"9ac8ccbe-40f2-430b-bf19-2eaea389143d\",\"mac\":\"9C:41:7C:A0:1B:88\",\"smartDeviceId\":\"44405107074789\"}";	//new sn
	migusdk_init(device_info);
	char version[128];
	char * result = malloc(MALLOC_SIZE);
	char * result1 = malloc(MALLOC_DEFULT_SIZE);

	if(!result)
	{
		printf("malloc failure\n");
		exit(-1);	
	}

	memset(version,0,sizeof(version));
	migusdk_init(device_info);
	set_CAfile("/etc/ssl/certs/cacert.pem");
#if 0
	pthread_t tid;
	pthread_t tid1;
	pthread_t tid2;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	ret = pthread_create(&tid, &attr,(void*) _ger_and_parse_radio_info, NULL);
	
	ret = pthread_create(&tid1, &attr,(void*) _get_and_parse_music_rank, NULL);
	ret = pthread_create(&tid2, &attr,(void*) _get_and_parse_music_newalbum, NULL);
	pthread_attr_destroy(&attr);
#endif
	lua_call_fun_async("/tmp/lua/migu_radio.lua", "get_migu_radio", NULL, ">i", &ret);
	while (1)
	{
		/* code */
	}
	

	// pthread_join(tid, NULL);
	// pthread_join(tid1, NULL);
	// pthread_join(tid2, NULL);
	// sleep(1);
    free(result);
	free(result1);

	migusdk_cleanup();

	return 0;
}

