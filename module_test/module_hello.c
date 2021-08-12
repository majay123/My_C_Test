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
 * @Date         : 2021-08-10 15:56:21
 * @LastEditors  : MCD
 * @LastEditTime : 2021-08-11 16:21:25
 * @FilePath     : /My_C_Test/module_test/module_hello.c
 * @Description  : 
 * 
 * ******************************************
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>

static int hello_data __initdata = 18;

static char* test_name = "dissecting Linux Device Driver";
module_param(test_name, charp, S_IRUGO);
static int test_num = 1024;
module_param(test_num, int, S_IRUGO);

int add_integar(int a, int b)
{
    return a + b;
}
EXPORT_SYMBOL_GPL(add_integar);

int sub_integar(int a, int b)
{
    return a - b;
}
EXPORT_SYMBOL_GPL(sub_integar);

static int __init 
hello_init(void)
{
    printk(KERN_EMERG "Hello World enter, hello_data = %d\n", hello_data);
    // fprintf(stdout,  "Hello World enter\n");
    return 0;
}
module_init(hello_init);

static void __exit
hello_exit(void)
{
    printk(KERN_EMERG "Hello World exit\n");
    // fprintf(stdout,  "Hello World exit\n");
}
module_exit(hello_exit);

MODULE_AUTHOR("Majay <machendong12@gmail.com>");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("A Simple Hello World Module");
MODULE_ALIAS("a simplest module");
