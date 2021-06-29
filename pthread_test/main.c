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
 * @Date         : 2021-06-22 16:28:18
 * @LastEditors  : MCD
 * @LastEditTime : 2021-06-25 16:20:05
 * @FilePath     : /My_C_Test/pthread_test/main.c
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
#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <signal.h>

#define PRINT_MCD_ENABLE 1


#if PRINT_MCD_ENABLE == 1
#define print_mcd(format, arg...)                                                          \
    do                                                                                     \
    {                                                                                      \
        printf("\033[31m[-mcd-]\033[0m:%s,%d--- " format "\n", __FILE__, __LINE__, ##arg); \
    } while (0)
#else
#define print_zizy(format, arg...) \
    do                             \
    {                              \
    } while (0)
#endif

sem_t sem;

static void *_test_cb(void *data)
{   
    int i = 0;
    while (1)
    {
        pid_t exit_pid = -1;
        int status = 0;

        exit_pid = waitpid(-1, &status, 0);
        if(exit_pid != -1)
        {
            
            print_mcd("test exit_pid");
        }
        else{
            print_mcd("get zi pid = %d",getpid());
            print_mcd("test hello world!");
            sleep(1);
            int ret = sem_trywait(&sem);
            print_mcd("get ret = %d", ret);
            if(ret == 0)
                break;
        }
        
        // i++;
        // if(i > 5)
        //     // pthread_exit((void*)getpid());
        //     break;
        // pthread_exit((void*)getpid());
    }
    print_mcd("out child pthread!");
    return NULL;
    
}

const char page_name[][32] = {
    "PAGE_MUSIC", 
    "PAGE_SCENE", 
    "PAGE_LIGHT", 
    "PAGE_AC",
    "PAGE_CURTAIN"
};
int a = 10;
#if 1
int g_test = 88;
int main(int argc, const char *argv[])
{
    pthread_t tid;
    pthread_attr_t t_attr;
    int i = 0;
    pid_t pid;
    int b = 20;
    pid_t cpid, fpid;
    int test = 98;

    // for(i = 0 ; i < 5; i++)
    //     print_mcd("%s", page_name[i]);
    // i=0;
    // if(!i)
    //     print_mcd("stest   11");


    pid = vfork();
    if(pid < 0)
        perror("vfork failed");
        
    if(pid == 0)   //child
    {
        // pid_t cpid = getppid();
        // print_mcd("child pid = %d, pid = %d", cpid, pid);
        g_test++;
        test++;
        // _exit(0);
    }
    else if(pid > 0)
    {
        // int fpid = getpid();
        // print_mcd("this is father = %d", fpid);
        sleep(2);
    }
    else
    {
        print_mcd("vfork failed");
    }  // sleep(1);
    
    if(1)
        // print_mcd("pid = %d, %d, %d", pid, getpid(),getppid());

    print_mcd("pid = %ld, g_test = %d, test = %d", (long)getpid(), g_test, test);
    exit(0);
#if 0   
    pid = vfork();	// 创建进程
	if(pid < 0){ // 出错
		perror("vfork");
	}
	
	if(0 == pid){ // 子进程
	
		a = 100, b = 200;
		printf("son: a = %d, b = %d\n", a, b);
		
		_exit(0); // 退出子进程，必须
	}else if(pid > 0){ // 父进程
		
		printf("father: a = %d, b = %d\n", a, b);
		
	}
// #if 0   
    sem_init(&sem, 0, 0);
    pthread_attr_init(&t_attr);
    pthread_attr_setinheritsched(&t_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setscope(&t_attr, PTHREAD_SCOPE_SYSTEM);

    pthread_create(&tid, &t_attr, _test_cb, NULL);
    // print_mcd("test destroy");
    pthread_attr_destroy(&t_attr);
    while(1){
        print_mcd("get pid = %d",getppid());
        sleep(1);
        i++;
        if(i > 10)
            sem_post(&sem);
        // for(i = 0; i < 5; i++)   
        // return;
    }

    sem_destroy(&sem);
#endif
    // pthread_join(tid, NULL);

}
#endif
#if 0
int a = 10;
 
int main(int argc, char *argv[])
{
	pid_t pid;
	
	int b = 20;
	
	pid = vfork();	// 创建进程
	if(pid < 0){ // 出错
		perror("vfork");
	}
	
	if(0 == pid){ // 子进程
	
		a = 100, b = 200;
		printf("son: a = %d, b = %d\n", a, b);
		
		_exit(0); // 退出子进程，必须
	}else if(pid > 0){ // 父进程
		
		printf("father: a = %d, b = %d\n", a, b);
		
	}
	
	return 0;
}
#endif