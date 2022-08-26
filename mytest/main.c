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
 * @LastEditTime : 2022-08-26 16:30:20
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


#define KEYWORD_PATH       ".speech_keyword" 
static int32_t _get_tuya_keyword_to_speech(void) 
{
    int ret = 0;
    // char sv_name[128] = "HOPE情景";

    FILE *file = fopen(KEYWORD_PATH, "r");
    if(file == NULL) {
        print_mcd("open file failed: %s", KEYWORD_PATH);
        return -1;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    char *buffer = (char *)calloc(size + 1, sizeof(char));
    if(buffer == NULL) {
        print_mcd("calloc memeory failed");
        ret = -1;
        goto error;
    }
	rewind(file);
    size_t rsize = fread(buffer, 1, size, file);
    print_mcd("get size: %ld, rsize: %ld\n", size, rsize);
    if(rsize != (size_t)size) {
        print_mcd("read file failed");
        ret = -1;
        goto error;
    }
    // speech set_cinfo
    print_mcd("get speech keywords : %s", buffer);
    // ai_engine_set_cinfo("HOPE情景", buffer, 1);
error:
    if(buffer != NULL) {
        free(buffer);
    }
    if(file != NULL) {
        fclose(file);
    }
    return ret;

}

static _json_test(void)
{
	char buffer[1024] = "{\"comName\":\"顺舟科技\",\"deviceCata\":\"Host3\",\"deviceName\":\"Host3\",\"parentId\":\"820930244371648512\",\"deviceSN\":\"44415106030037\",\"deviceVO\":true}\"}";
	char buffer1[1024] = "{\"comName\":\"顺舟科技\",\"deviceCata\":\"Host3\",\"deviceName\":\"Host3\",\"parentId\":\"820930244371648512\",\"deviceSN\":\"44415106030037\",\"deviceVO\":1}\"}";

	cJSON *root = cJSON_Parse(buffer);
	if(root != NULL) {
		cJSON *comName = cJSON_GetObjectItem(root, "comName");
		print_mcd("comName: %s\n", comName->valuestring);
		cJSON *deviceVO = cJSON_GetObjectItem(root, "deviceVO");
		print_mcd("deviceVO: %d\n", deviceVO->valueint);
	}
}

char* itoa(int num, char* str, int radix)
{
    char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";//索引表
    unsigned unum;//存放要转换的整数的绝对值,转换的整数可能是负数
    int i=0,j,k;//i用来指示设置字符串相应位，转换之后i其实就是字符串的长度；转换后顺序是逆序的，有正负的情况，k用来指示调整顺序的开始位置;j用来指示调整顺序时的交换。
 
    //获取要转换的整数的绝对值
    if(radix==10&&num<0)//要转换成十进制数并且是负数
    {
        unum=(unsigned)-num;//将num的绝对值赋给unum
        str[i++]='-';//在字符串最前面设置为'-'号，并且索引加1
    }
    else unum=(unsigned)num;//若是num为正，直接赋值给unum
 
    //转换部分，注意转换后是逆序的
    do
    {
        str[i++]=index[unum%(unsigned)radix];//取unum的最后一位，并设置为str对应位，指示索引加1
        unum/=radix;//unum去掉最后一位
 
    }while(unum);//直至unum为0退出循环
 
    str[i]='\0';//在字符串最后添加'\0'字符，c语言字符串以'\0'结束。
 
    //将顺序调整过来
    if(str[0]=='-') k=1;//如果是负数，符号不用调整，从符号后面开始调整
    else k=0;//不是负数，全部都要调整
 
    char temp;//临时变量，交换两个值时用到
    for(j=k;j<=(i-1)/2;j++)//头尾一一对称交换，i其实就是字符串的长度，索引最大值比长度少1
    {
        temp=str[j];//头部赋值给临时变量
        str[j]=str[i-1+k-j];//尾部赋值给头部
        str[i-1+k-j]=temp;//将临时变量的值(其实就是之前的头部值)赋给尾部
    }
 
    return str;//返回转换后的字符串
 
}

char aaa[] = "[{\"value1\":\"自定义话术1\"},{\"value2\":\"\"},{\"value3\":\"回家模式的自定义话术\"},{\"title\":\"回家模式\"}]";
#define DSP_MAX_DATA_SIZE		(32 - 1)
int main(int argc, const char *argv[])
{
	char utf8_tmp_buf[128] = "}12123";
	int len = strlen(utf8_tmp_buf);
	int i = 10;
	all_db_info_t *temp;
	srv_msg_t *msg;
	db_ctrl_t db_ctrl;
	int64_t userid = 1005039295;
	char *user = NULL;

	char str[32] = {0};
	char str1[32] = {0};

	// itoa(userid, str, 10);
	// printf("userid = %d, %s\n", userid, str);
	// snprintf(str1, sizeof(str1), "%d", userid);
	// printf("userid = %d, %s\n", userid, str1);
	// printf("user = %d\n", strlen(user));

	// printf("get addr %d\n", INDOOR_MACH_WORK_MODE_INP_REG(1, 14));
	// printf("get addr %d\n", INDOOR_MACH_REF_TMP_UPPER_AND_LOWER_LIMIT_INP_REG(1, 14));
	// printf("get addr %d\n", INDOOR_MACH_HEAT_TMP_UPPER_AND_LOWER_LIMIT_INP_REG(1, 14));
	// printf("get addr %d\n", INDOOR_MACH_OPERATION_DETAILS_HOLD_REG(1, 14));

	// _json_test(	);

	// srv_msg_t *mtest = (srv_msg_t *)calloc(10, sizeof(srv_msg_t));
	// for ( i = 0; i < 10; i++)
	// {
	// 	mtest[i].type = i;
	// 	// printf("%p\n", mtest[i]);
	// }
	// for ( i = 0; i < 10; i++)
	// {
	// 	// mtest[i].type = i;
	// 	printf("%d\n", mtest[i].type);
	// }
#if 0
	char buf[1024] = "播放周杰伦的歌";
	printf("size = %d\n", strlen(buf));
	// for ( i = 0; i < strlen(buf); i++ )
	// {
	// 	printf("0x%02x\n", buf[i]);
	// }
	printf("%s\n", buf);
	i = 1;
	switch (i)
	{
	case 1:
		printf("%d\n", i);
	case 2:
		printf("%d\n", i);
	default:
		break;
	}
	
#endif
	// int ret = strcmp("9.0.2.20211101", "9.0.2.20200101");
	// print_mcd("ret = %d", ret);
#if 0
	int *size = 1;
	// _get_tuya_keyword_to_speech();
	// print_mcd("size = %d", size);
	char test_buf[64] = "test";
	print_mcd("test_buf = %s", test_buf);
	if(!strcmp(test_buf, "test")) { 
		print_mcd("good!!");
	}
	char data[1024] = "";
	int a = 0;
	int fd = open("test.txt", O_CREAT | O_RDWR, 0776);
	memset(test_buf, 0, sizeof(test_buf));
	if(fd > 0) {
		// lseek(fd, 0, SEEK_END);
		// for ( i = 0; i < 5; i++)
		// {
		// 	sprintf(test_buf, "%s %d\n", "test", i+10);
		// 	write(fd, test_buf, strlen(test_buf));
		// }
		while(1) {
			write(fd, data, sizeof(data));
			a++;
			if(a >= 1024*3049)
				break;
		}
	}

	close(fd);
	print_mcd("write over");
#endif
#if 0
	char *auth_str = "jvkl78XHnUTHH+VZvRN2YTqeHSpuJ/BiRDoVEizGoqSg3y66y756GdGKuhxnCQeTIbbcRw4MdFU=";
	
	int s_name_size = 0;
	
	int size = strlen(auth_str);
	char *test_mem = malloc(size);
	char mac_empower[DSP_MAX_DATA_SIZE + 1] = {0};
	sprintf(test_mem, "%s", auth_str);
	char *test1 = test_mem;
	
	while(size > 0) {
		memset(mac_empower, 0, sizeof(mac_empower));
		int block = size - DSP_MAX_DATA_SIZE;
		// print_mcd("print data = %d", block > 0 ? DSP_MAX_DATA_SIZE : size);
		memcpy(mac_empower, test1, (block > 0 ? DSP_MAX_DATA_SIZE : size));
		print_mcd("data = %d %s ", block > 0 ? DSP_MAX_DATA_SIZE : size, mac_empower);
		test1 += block > 0 ? DSP_MAX_DATA_SIZE : size;
		size -= DSP_MAX_DATA_SIZE;
	}
	free(test_mem);
#endif

#if 0
	 {

		cJSON *root = cJSON_CreateObject();
		cJSON *name = cJSON_CreateObject();
		cJSON_AddItemToObject(root, "name", name);

		// char *out = calloc(1024, sizeof(char));
		char *out = cJSON_PrintUnformatted(root);
		print_mcd("%p, out = %s", out, out);
		// print_mcd("%p, out = %s", out, out);
		// print_mcd("%p, out1 = %s", out1, out1);

		cJSON_Delete(root);
		free(out);
		sleep(2);
	}
#endif
	// print_mcd("%p, out = %s", out, out);

	
#if 0
	cJSON *root = cJSON_Parse(aaa);
	if(root){
		print_mcd("root is not null");
		int size = cJSON_GetArraySize(root);
		print_mcd("%d",size);
		cJSON *item = NULL;
		cJSON *iitem  = NULL;
		char s_name[64] = {0};
		char *s_name1 = NULL, *s_name2 = NULL, *s_name3 = NULL;
		cJSON *s_root = cJSON_CreateArray();
		char temp[128] = {0};
            // 
		for(i = 0; i < size; i++) {
			item = cJSON_GetArrayItem(root, i);
			iitem = cJSON_GetObjectItem(item, "title");
			if(iitem){
				// s_name = iitem->valuestring;
				memcpy(s_name, iitem->valuestring, strlen(iitem->valuestring));
				print_mcd("%s", s_name);
				if(s_root) {
					cJSON *name_s = cJSON_CreateString(s_name);
					cJSON_AddItemToArray(s_root, name_s);
				}

			}
		}
		for(i = 0; i < size; i++) {
			item = cJSON_GetArrayItem(root, i);
			iitem = cJSON_GetObjectItem(item, "value1");
			if(iitem){
				if(strlen(iitem->valuestring) > 0) {
					s_name_size++;
					s_name1 = iitem->valuestring;
					print_mcd("%s %d", s_name1, strlen(s_name1));
					if(s_root) {
						memset(temp, 0, sizeof(temp));
						snprintf(temp, sizeof(temp),"%s:%s", s_name1, s_name);
						print_mcd("%s", temp);
						cJSON *name_s1 = cJSON_CreateString(temp);
						cJSON_AddItemToArray(s_root, name_s1);
					}
					
				}
			}
			iitem = cJSON_GetObjectItem(item, "value2");
			if(iitem){
				if(strlen(iitem->valuestring) > 0) {
					s_name_size++;
					s_name2 = iitem->valuestring;
					print_mcd("%s %d", iitem->valuestring, strlen(iitem->valuestring));
					if(s_root) {
						memset(temp, 0, sizeof(temp));
						snprintf(temp, sizeof(temp),"%s:%s", s_name2, s_name);
						print_mcd("%s", temp);
						cJSON *name_s2 = cJSON_CreateString(temp);
						cJSON_AddItemToArray(s_root, name_s2);
					}
				}
			}
			iitem = cJSON_GetObjectItem(item, "value3");
			if(iitem){
				if(strlen(iitem->valuestring) > 0) {
					s_name_size++;
					s_name3 = iitem->valuestring;
					print_mcd("%s %d", iitem->valuestring, strlen(iitem->valuestring));
					if(s_root) {
						memset(temp, 0, sizeof(temp));
						snprintf(temp, sizeof(temp),"%s:%s", s_name3, s_name);
						print_mcd("%s", temp);
						cJSON *name_s3 = cJSON_CreateString(temp);
						cJSON_AddItemToArray(s_root, name_s3);
					}
				}
			}
		}
		print_mcd("s_name size = %d", s_name_size);
		if(s_name_size > 0) {
			char *out = cJSON_PrintUnformatted(s_root);
			char *out1 = calloc(1024, sizeof(char));

			print_mcd("out = %p, %s", out, out);
			print_mcd("out1 = %p, %s", out1, out1);
			out1 = out;
			print_mcd("out1 = %p, %s", out1, out1);
			// free(out);
			free(out1);
			out1 = NULL;
			print_mcd("out = %p, %s", out, out);

		}
		cJSON_Delete(s_root);
		// cJSON *a = cJSON_GetObjectItem(root, "title");
		// print_mcd("title = %s", a->valuestring);
	}
#endif

#if 0	
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
#endif

return 0;
	// _get_file_size();

#if 0
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
#endif
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
