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
 * @Date         : 2021-06-29 10:38:35
 * @LastEditors  : MCD
 * @LastEditTime : 2021-07-09 15:42:48
 * @FilePath     : /My_C_Test/point_test_1/point.c
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
#include <malloc.h>
#include "common.h"

device_t g_dev_test;

#define CFG_PAGE_MUSIC              "PAGE_MUSIC"  
#define CFG_PAGE_SCENE              "PAGE_SCENE"
#define CFG_PAGE_LIGHT              "PAGE_LIGHT"
#define CFG_PAGE_AC                 "PAGE_AC"
#define CFG_PAGE_CURTAIN            "PAGE_CURTAIN"
#define CFG_PAGE_SETTING            "PAGE_SETTING"

typedef enum
{
    // SCENE_EXIT    = -1,
    SCENE_MUSIC =0,
    SCENE_SCENE ,
    SCENE_LIGHT,
    SCENE_AC,
    SCENE_CURTAIN,
    SCENE_SETTING,
    SCENE_MAX,
    
} launcher_scene_e;

char screen_page_cfg[SCENE_MAX][32] = {
    CFG_PAGE_MUSIC,
    CFG_PAGE_SCENE,
    CFG_PAGE_LIGHT,
    CFG_PAGE_AC,
    CFG_PAGE_CURTAIN,
    CFG_PAGE_SETTING,
};


typedef struct heap{
    char size;
    int ele[0];
}Heap;

static void bar(int *num)
{
    long int len = sizeof(num);
    print_mcd("%ld", len);
}
static void bar1(int num[])
{
    long int len = sizeof(num);
    print_mcd("%ld", len);
}

static void change(char *name)
{
    // 实际上传递的是指针的地址，而非内容
    print_mcd("%p", name);
    char *_name = name;
    print_mcd("%p",  _name);
    // 不要试图 在函数中更改指针的指向，我们只能更改指针指向的内容
    strcpy(name, "do some thing");

    // 所以 下面的操作是无法实现的
    // _name = "do some thing";
}

int main(int argc, char const *argv[])
{
    // int i = 0;
    
    // char *str[3] = {
    //     "my test1",
    //     "my test2",
    //     "my test3",
    // };
    // char  buf[32] = {0};
    // strcpy(buf, str[0]);
#if 0
    int a = 123, b;
    int *ptr = &a;
    char *str;
    b = (int)ptr;   //b变成了地址， ptr是地址 int* 类型
    str = (char *)b;
    int *c = (int *)b;
    print_mcd("%d, %p, %d", *c, str, *str);

    print_mcd("%d, %p", *ptr, ptr);
    ptr++;  
    print_mcd("%d, %p", *ptr, ptr);
    *(ptr++);
    print_mcd("%d, %p", *ptr, ptr);
#endif   
    char s = 'a';
    int *ptr;
    ptr = (int *)&s;
    // *ptr = 1298;
    print_mcd("%p", ptr);
    ptr++;
    // *ptr = 'c';

    print_mcd("%p", ptr);

#if 0 
    print_mcd("%ld, %ld", sizeof(str), sizeof(char *));
    for (i = 0; i < sizeof(str) /  sizeof(char *); i++)
    {
        print_mcd("%s, %d, %p", str[i], strlen(str[i]), &str[i]);
    }
    

    // print_mcd("%s, %p", *str, &str[0]);
    // print_mcd("%s, %p", *(str + 1), &str[1]);
    // print_mcd("%s, %p", *(str + 2), &str[2]);


    char *buf = str[0];

    print_mcd("%s, %p, %c", buf, &buf, str[0][1]);

    int num[] = {1, 2, 3};
    int *point = num;

    print_mcd("%d, %d", sizeof(num), sizeof(point));
    // 等价于
    print_mcd("%d, %d", sizeof(int) * 3, sizeof(point));

//作为参数传入，数组会退化成指针，sizeof(int *) 为 4
    bar(num);
    bar1(num);
    #endif
#if 0
    char name_buf[] = "name buf";
    print_mcd("%p", name_buf);
    change(name_buf);
    print_mcd("%s", name_buf);
    
    Heap heap = {0};

    print_mcd("%d, %p", sizeof(heap), &heap);
    print_mcd("%p, %p", &heap.size, &heap.ele);
#endif

#if 0
    char a[20] = "kkis is test!";
    int i = 0;
    char *test = NULL;
    int num = 44;//atoi(argv[1]);
    int *t_num ;

    t_num = &num;

    print_mcd("%d, %p, %p", *t_num, t_num, &num);

    if(!test)
        print_mcd("is null");
    print_mcd("size = %d", sizeof(launcher_scene_e));

    // for(; i < SCENE_MAX; i++)
    //     print_mcd("%s", screen_page_cfg[i]);

    // print_mcd("%d, %d", (num <= 0)? 1 : num, sizeof(char*)*((num <= 0)? 1 : num));
    // realloc(test, 1024);

    // test = (char**)malloc(sizeof(char*)*((num <= 0)? -1 : num));
    realloc(test, 0);
    test = malloc(num);
    print_mcd("test size = %d", malloc_usable_size(test));
    
    int num1 = 100;
    char *test1 = realloc(test, num1);
    if(!test1)
    {
        print_mcd("realloc failed");
        free(test);
        test = NULL;
    }
    test = test1;
    print_mcd("test realloc size = %d", malloc_usable_size(test));
    // print_mcd("test1 realloc size = %d", malloc_usable_size(test1));

    // memset(test, 0 , sizeof(char *));

    if(test != NULL)
        free(test);
    else
        print_mcd("null");
    // int *p = (int *)a;

    // print_mcd("p addr = %p, %s", p, p);
    // p+= 5;
    // print_mcd("p addr = %p, %s", p, p);
    return 0;
#endif

#if 0
    char *ptr= a;
    char **ptr1 = &ptr;
    print_mcd("a addr = %p", a);
    print_mcd("ptr addr = %p", ptr);
    print_mcd("*ptr addr = %c", *ptr);
    print_mcd("&ptr addr = %p", &ptr);
    print_mcd("&ptr1 addr = %p", &ptr1);
    print_mcd("ptr1 = %p",  ptr1);
    print_mcd("*ptr1 = %p",  *ptr1);
    print_mcd("**ptr1 = %c",  **ptr1);

    ptr1++;
    print_mcd("%c", **ptr1);
    print_mcd("&ptr1 addr = %p", &ptr1);
    print_mcd("ptr1 = %p",  ptr1);
    print_mcd("*ptr1 = %p",  *ptr1);
    print_mcd("**ptr1 = %c",  **ptr1);

    print_mcd("%d, %d", sizeof(char *), sizeof(char **));
#endif
#if 0
    char s[2][3] = {"ab", "cd"};
    char *p = s;
    print_mcd("%c", s[1][1]);
    print_mcd("%c",*++p+2);
    char *test;
    test = realloc(test, 100);
    // char *test = malloc(0);
    print_mcd("%p", test);
    // print_mcd("%c",++*p+2);
    return 0;
#endif
}


