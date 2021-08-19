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
 * @Date         : 2021-08-19 13:19:21
 * @LastEditors  : MCD
 * @LastEditTime : 2021-08-19 14:32:25
 * @FilePath     : /My_C_Test/second_test/second.c
 * @Description  : 
 * 
 * ******************************************
 */

#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/timer.h>

#define SECOND_MAJOR (228)
static int second_major = SECOND_MAJOR;
module_param(second_major, int, S_IRUGO);

struct second_dev {
    struct cdev cdev;
    atomic_t counter;
    struct timer_list s_timer;
};

static struct second_dev *second_devp;

static void
second_timer_handler(struct timer_list *t)
{
    mod_timer(&second_devp->s_timer, jiffies + HZ); //触发下一次定时
    atomic_inc(&second_devp->counter);              //增加秒计时
    printk(KERN_INFO "current jiffies is %ld\n", jiffies);
}

static int
second_open(struct inode *inode, struct file *filp)
{
    // 在4.15内核里，init_timer被移除了。需要换用新的timer_setup接口
    // init_timer(&second_devp->s_timer);
    // second_devp->s_timer.function = second_timer_handler;
    timer_setup(&second_devp->s_timer, second_timer_handler, 0);
    second_devp->s_timer.expires = jiffies + HZ;


    add_timer(&second_devp->s_timer);
    atomic_add(0, &second_devp->counter); //初始化秒计数为0

    return 0;
}

static int
second_release(struct inode *inode, struct file *filp)
{
    del_timer(&second_devp->s_timer);
    return 0;
}

static long int
second_read(struct file *filp, char __user *buf, size_t count, loff_t *ppos)
{
    int counter;

    counter = atomic_read(&second_devp->counter);
    if (put_user(counter, (int *)buf)) // 复制counter到userspace
        return -EFAULT;
    else
        return sizeof(unsigned int);
}

static const struct file_operations second_fops = {
    .owner = THIS_MODULE,
    .open = second_open,
    .read = second_read,
    .release = second_release,
};

static void
second_setup_cdev(struct second_dev *dev, int index)
{
    int err, devno = MKDEV(second_major, index);

    cdev_init(&dev->cdev, &second_fops);
    dev->cdev.owner = THIS_MODULE;

    err = cdev_add(&dev->cdev, devno, 1);
    if (err)
        printk(KERN_INFO "Failed to add second devices\n");
}

static int __init
second_init(void)
{
    int ret = 0;
    dev_t devno = MKDEV(second_major, 0);

    if (second_major)
        ret = register_chrdev_region(devno, 1, "second");
    else {
        ret = alloc_chrdev_region(&devno, 0, 1, "second");
        second_major = MAJOR(devno);
    }
    if (ret < 0)
        return ret;

    second_devp = kzalloc(sizeof(*second_devp), GFP_KERNEL);
    if (!second_devp) {
        ret = -ENOMEM;
        goto fail_malloc;
    }

    second_setup_cdev(second_devp, 0);
    return 0;

fail_malloc:
    unregister_chrdev_region(devno, 1);
    return ret;
}
module_init(second_init);

static void __exit
second_exit(void)
{
    cdev_del(&second_devp->cdev);
    kfree(second_devp);
    unregister_chrdev_region(MKDEV(second_major, 0), 1);
}
module_exit(second_exit);

MODULE_AUTHOR("Majay <machendong12@gmail.com>");
MODULE_LICENSE("GPL v2");