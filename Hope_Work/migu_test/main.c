#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "migu.h"
#include "cJSON.h"

#define KEY	"d98ec9e4cb95c945e50b8b1abbfdc3de"
#define  MALLOC_SIZE	(1024 * 256)


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
int main(void)
{
	// char device_info [] = "{\"uid\":\"9ac8ccbe-40f2-430b-bf19-2eaea389143d\",\"mac\":\"9C:41:7C:A0:1B:88\",\"smartDeviceId\":\"44401007240347\"}";	//old sn
	char device_info [] = "{\"uid\":\"9ac8ccbe-40f2-430b-bf19-2eaea389143d\",\"mac\":\"9C:41:7C:A0:1B:88\",\"smartDeviceId\":\"44405107074789\"}";	//new sn
	migusdk_init(device_info);
	char version[128];
	char * result = malloc(MALLOC_SIZE);

	if(!result)
	{
		printf("malloc failure\n");
		exit(-1);	
	}

	memset(version,0,sizeof(version));
	migusdk_init(device_info);
	// set_CAfile("/etc/ssl/certs/cacert.pem");
	getSdkVersion(version);
	printf("%s\n",version);

	_try_to_active();

	memset(result, 0, MALLOC_SIZE);
	get_device_info(KEY, result);
	printf("device info: %s\n",result);


	free(result);

	migusdk_cleanup();
	return 0;
}

