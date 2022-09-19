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
 * @Date         : 2021-08-12 09:36:52
 * @LastEditors  : MCD
 * @LastEditTime : 2022-08-11 10:43:35
 * @FilePath     : /My_C_Test/file_test/file_test.c
 * @Description  : 
 * 
 * ******************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include <inttypes.h>

#define PRINT_MCD_ENABLE 1


#if PRINT_MCD_ENABLE == 1
    #define print_mcd(format, arg...)                                                          \
    do                                                                                     			\
    {                                                                                      			\
        char ctime[30] = { 0 };																		\
        char ctime1[30] = { 0 };																	\
        struct tm tm1 = { 0 };																		\
        struct timespec ts; 																		\
        clock_gettime(CLOCK_REALTIME, &ts); 														\
        localtime_r(&ts.tv_sec,&tm1);																\
        strftime(ctime,sizeof(ctime),"%Y-%m-%d %H:%M:%S",&tm1); 									\
        snprintf(ctime1,sizeof(ctime),"%s.%.3ld",ctime,ts.tv_nsec/1000/1000);	                	\
        printf("\033[31m[--mcd--][%s]\033[0m:%s,%s,%d--- " format "\n", ctime1,__FILE__,__func__,__LINE__, ##arg); 	\
    } while (0)
#else
    #define print_mcd(format, arg...)   do {} while (0)
#endif  

#define MAX_BUF_SIZE        (1024)
static int test_count = 10;
static int fexit = 0;

typedef struct {
     void* data;
     int exit;
     unsigned char onoff;
} thread_param_t;

thread_param_t test_param = {0};

typedef void* (ThreadWrapper)(void*);
pthread_t _loop_thread_creat(ThreadWrapper func, thread_param_t *arg)
{
    pthread_t t_id;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    int ret = pthread_create(&t_id, &attr, func, (void *)arg);
    pthread_attr_destroy(&attr);

    if (ret || !t_id)
        return -1;
    return t_id;
}

int _loop_thread_exit(pthread_t tid, thread_param_t* param)
{   
    param->exit = 1;
    if(tid != 0) {
        pthread_join(tid, NULL);
        tid = 0;
    }

    return 0;
}

void test_func(void *arg)
{
    thread_param_t* paramter = (thread_param_t*) arg;
    while(paramter->exit != 1) {
        printf("test %d!\n", test_count);
        sleep(1);
        test_count--;
    }
}

int get_music_name(const char *uri, char *name, int maxlen)
{
    if(uri == NULL || name == NULL) {
        return -1;
    }
    char *ptr = (char*)uri;
    char *tail;
    ptr = strrchr(uri, '/');
    if(ptr == NULL) {
        snprintf(name, maxlen, "Unknown");
        // memset(name, 0, maxlen);
        return 0;
    }
    ++ptr;
    tail = strrchr(ptr, '.');
    if(ptr && tail && tail-ptr) {
        strncpy(name, ptr, (tail-ptr > maxlen) ? (maxlen - 1) : tail-ptr);
        // print_zizy("get name %s[end]",name);
    }
    return 0;
}


	
struct du562_reverb_ratio_t {
	uint16_t echo_delay;
	uint16_t echo_max_delay;
	uint16_t wetdrymix;
};

static int du562_set_Reverb(uint8_t level)
{	
	int i = 0;
	int ret = 0;
	struct du562_reverb_ratio_t reverb_ratio[] = {
		{0,			0,			0},
		{100,		300,		40},
		{400, 		1000, 		100},
	};
	
	uint8_t param_buf[][8] = {
		{
			0xA5, 0x5A,/*sync head*/
			0x8d,/* */
			0x03,/*length*/
			0x03,/*parameter*/
			0x00, 0x00,/* */
			0x16,/*sync tail*/
		},
		{
			0xA5, 0x5A,/*sync head*/
			0x8d,/* */
			0x03,/*length*/
			0x05,/*parameter*/
			0x00, 0x00,/* */
			0x16,/*sync tail*/
		},
		{
			0xA5, 0x5A,/*sync head*/
			0x91,/* */
			0x03,/*length*/
			0x03,/*parameter*/
			0x00, 0x00,/* */
			0x16,/*sync tail*/
		},
	};

    printf("%d, %d, %d\n", reverb_ratio[level].echo_delay, reverb_ratio[level].echo_max_delay, reverb_ratio[level].wetdrymix);
    // param_buf[0][5] = reverb_ratio[level].echo_delay & 0xff;
	// param_buf[0][6] = (reverb_ratio[level].echo_delay>>8) & 0xff;/*vol L*/

    // param_buf[1][5] = reverb_ratio[level].echo_max_delay & 0xff;
	// param_buf[1][6] = (reverb_ratio[level].echo_max_delay>>8) & 0xff;/*vol L*/

    // param_buf[2][5] = reverb_ratio[level].wetdrymix & 0xff;
	// param_buf[2][6] = (reverb_ratio[level].wetdrymix>>8) & 0xff;/*vol L*/

	memcpy(&param_buf[0][5], (uint8_t *)&reverb_ratio[level].echo_delay, 2);
	memcpy(&param_buf[1][5], (uint8_t *)&reverb_ratio[level].echo_max_delay, 2);
	memcpy(&param_buf[2][5], (uint8_t *)&reverb_ratio[level].wetdrymix, 2);

    printf("%02x, %02x\n", param_buf[0][5], param_buf[0][6]);
    printf("%02x, %02x\n", param_buf[1][5], param_buf[1][6]);
    printf("%02x, %02x\n", param_buf[2][5], param_buf[2][6]);

}



int main(int argc, char const *argv[])
{
    int fd;
    int i = 0;
    size_t len = 0;
    char buff[MAX_BUF_SIZE] = "Hello linux, this is file test!";
    char read_buff[MAX_BUF_SIZE] = {0};
    unsigned int data = 0x00002255;
    unsigned int tmp = data;
    unsigned char ch = 0;
    unsigned char ba = 4;
    unsigned char gb = 0;

    // data &= ~(0xF << (ch * 4));
    // data |= ba << (ch * 4);
    // printf("%08x\n", data);
    for ( i = 0; i < 4; i++)
    {
        /* code */
        tmp = data;
        tmp &= 0xF << (i * 4);
        gb = tmp >> (i * 4);
        printf("%0x\n", gb);
    }
    
    

#if 0
int kk
    fd = open("hello.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if(fd){
        len = write(fd, buff, strlen(buff));
        close(fd);
        
        if(len <= 0){
            return -1;
        }
    }
    
    fd = open("hello.txt", O_RDONLY);
    len = read(fd, read_buff, MAX_BUF_SIZE);
    close(fd);
    if(len <= 0)
        return -1;
    read_buff[len] = '\0';
    print_mcd("%s", read_buff);

    // thread_param_t test_param;
    pthread_t thread_id;
    // memset(&test_param, 0, sizeof(test_param));

    thread_id = _loop_thread_creat((ThreadWrapper *)test_func, &test_param);
    printf("thread_id = %ld\n", thread_id);
    sleep(5);
    _loop_thread_exit(thread_id, &test_param);
    char title[256] = {0};
    get_music_name("/mnt/test/music/siakkenafa_afajsdifaskl_+aklfjkaj$dkajsklfjakekdlsdjklafsjekljkldasjlfasdlfefeajfkldsjaklfjklsafjeisdkjhsagiaeshjagkldshjgfhmnvcmxnvjagfasdjk助攻发丝房东叫阿护额将阿斯顿规划思恩姐呢福啊了看不能打微机课老师大话顾问.mp3", title, sizeof(title) - 1);
    printf("title = %s\n", title);
#endif
     
    //     printf("same !!!");
    // }
    // else 
    //     printf("not same !!!");
    
    while (i < 10) {
        printf("test i = %d\n", i++);
    }

    char temp[100] = "3.0.10.22";
    char temp1[3] = {0};
    sscanf(temp, "%2"SCNu8".%2"SCNu8".%2"SCNu8, &temp1[0], &temp1[1], &temp1[2]);
    printf("!!!  %2"SCNu8".%2"SCNu8".%2hhu\n", temp1[0], temp1[1], temp1[2]);

    char level = 0x1b;
    if(level <= 60) {
		level = 0;
	} else if(level <= 80) {
		level = 1;
	} else if(level <= 100) {
		level = 2;
	} else {
		return -1;  
	}
    printf("level: %d\n", level);
    du562_set_Reverb(level);


    return 0;
}

