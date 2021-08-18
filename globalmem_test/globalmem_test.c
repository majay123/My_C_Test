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
 * @Date         : 2021-08-13 11:01:48
 * @LastEditors  : MCD
 * @LastEditTime : 2021-08-17 13:34:06
 * @FilePath     : /My_C_Test/globalmem_test/globalmem_test.c
 * @Description  : 
 * 
 * ******************************************
 */
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/uaccess.h>

// #include <stdio.h>

#define GLOBALMEM_SIZE  (0x1000)
#define MEM_CLEAR       (0x01)
#define GLOBALMEM_MAJOR (230)
#define DEVICE_NUM      (10)

static int globalmem_major = GLOBALMEM_MAJOR;
module_param(globalmem_major, int, S_IRUGO);

struct globalmem_dev {
    struct cdev cdev;
    unsigned char mem[GLOBALMEM_SIZE];
    struct mutex mutex;
};

struct globalmem_dev *globalmem_devp;

// #define container_of(ptr, type, member) ({			\
// 	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
// 	(type *)( (char *)__mptr - offsetof(type,member) );})

static int
globalmem_open(struct inode *inode, struct file *filp)
{
    struct globalmem_dev *dev = container_of(inode->i_cdev, struct globalmem_dev, cdev);
    filp->private_data = dev;
    return 0;
}

static int
globalmem_release(struct inode *inode, struct file *filp)
{
    return 0;
}

static long
globalmem_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    struct globalmem_dev *dev = filp->private_data;
    switch (cmd) {
    case MEM_CLEAR:
        mutex_lock(&dev->mutex);
        memset(dev->mem, 0, GLOBALMEM_SIZE);
        mutex_unlock(&dev->mutex);
        printk(KERN_ALERT "globalmem is set to zero\n");
        break;

    default:
        return -EINVAL;
        break;
    }
    return 0;
}

static long int
globalmem_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos)
{
    unsigned long p = *ppos;
    unsigned int count = size;
    int ret = 0;
    struct globalmem_dev *dev = filp->private_data;

    if (p >= GLOBALMEM_SIZE)
        return 0;

    if (count > GLOBALMEM_SIZE - p)
        count = GLOBALMEM_SIZE - p;

    mutex_lock(&dev->mutex);

    if (copy_to_user(buf, dev->mem + p, count))
        ret = -EFAULT;
    else {
        *ppos += count;
        ret = count;
        printk(KERN_ALERT "read %u bytes(s) from %lu\n", count, p);
    }

    mutex_unlock(&dev->mutex);

    return ret;
}

static long int
globalmem_write(struct file *filp, const char __user *buf, size_t size, loff_t *ppos)
{
    unsigned long p = *ppos;
    unsigned int count = size;
    int ret = 0;
    struct globalmem_dev *dev = filp->private_data;

    if (p >= GLOBALMEM_SIZE)
        return 0;

    if (count > GLOBALMEM_SIZE - p)
        count = GLOBALMEM_SIZE - p;
    
    mutex_lock(&dev->mutex);

    if (copy_from_user(dev->mem + p, buf, count))
        ret = -EFAULT;
    else {
        *ppos += count;
        ret = count;
        printk(KERN_ALERT, "written %u bytes(s) from %lu\n", count, p);
    }
    mutex_unlock(&dev->mutex);

    return ret;
}

static loff_t
globalmem_llseek(struct file *filp, loff_t offset, int oring)
{
    loff_t ret = 0;
    switch (oring) {
    case 0:
        if (offset < 0) {
            ret = -EINVAL;
            break;
        }
        if ((unsigned int)offset > GLOBALMEM_SIZE) {
            ret = -EINVAL;
            break;
        }
        filp->f_op = (unsigned int)offset;
        ret = filp->f_op;
        break;
    case 1:
        if ((filp->f_op + offset) > GLOBALMEM_SIZE) {
            ret = -EINVAL;
            break;
        }
        if ((filp->f_op + offset) < 0) {
            ret = -EINVAL;
            break;
        }
        filp->f_op += offset;
        ret = filp->f_op;
        break;
    default:
        ret = -EINVAL;
        break;
    }

    return ret;
}

static const struct file_operations globalmem_fops = {
    .owner = THIS_MODULE,
    .llseek = globalmem_llseek,
    .read = globalmem_read,
    .write = globalmem_write,
    .unlocked_ioctl = globalmem_ioctl,
    .open = globalmem_open,
    .release = globalmem_release,
};

static void
globalmem_setup_cdev(struct globalmem_dev *dev, int index)
{
    int err;
    dev_t devno = MKDEV(globalmem_major, index);

    cdev_init(&dev->cdev, &globalmem_fops);
    dev->cdev.owner = THIS_MODULE;
    err = cdev_add(&dev->cdev, devno, 1);
    if (err) {
        printk(KERN_ALERT, "Error %d adding globalmem %d", err, index);
    }
}

static int __init
globalmem_init(void)
{
    int ret;
    int i;
    dev_t devno = MKDEV(globalmem_major, 0);

    if (globalmem_major)
        ret = register_chrdev_region(devno, DEVICE_NUM, "globalmem");
    else {
        ret = alloc_chrdev_region(&devno, 0, DEVICE_NUM, "globalmem");
        globalmem_major = MAJOR(devno);
    }
    
        
    if (ret < 0)
        return ret;
    globalmem_devp = kzalloc(sizeof(struct globalmem_dev) * DEVICE_NUM, GFP_KERNEL);
    if (!globalmem_devp) {
        ret = -ENOMEM;
        goto fail_malloc;
    }

    for (i = 0; i < DEVICE_NUM; i++) {
        /* code */
        mutex_init(&(globalmem_devp + i)->mutex);
        globalmem_setup_cdev(globalmem_devp + i, i);
    }

    return 0;

fail_malloc:
    unregister_chrdev_region(devno, DEVICE_NUM);
    return ret;
}
module_init(globalmem_init);

static void __exit
globalmem_exit(void)
{
    int i;
    for (i = 0; i < DEVICE_NUM; i++) {
        /* code */
        cdev_del(&(globalmem_devp + i)->cdev);
    }

    kfree(globalmem_devp);
    unregister_chrdev_region(MKDEV(globalmem_major, 0), DEVICE_NUM);
}
module_exit(globalmem_exit);

MODULE_AUTHOR("Majay <machendong12@gmail.com>");
MODULE_LICENSE("GPL v2");