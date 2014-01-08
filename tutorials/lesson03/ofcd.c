#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <asm-generic/uaccess.h>


#define DEVICE_NAME "ofcd_device" // The name of the device in /proc/devices
#define DEVICE_TYPE "chardrv"     // Device type
#define NODE_DEVICE_NAME "ofcd"   // The prefix for this device in /dev

static dev_t first;               // Global variable for the first device number
static struct cdev c_dev;         // Global variable for the character device structure
static struct class *cl;          // Global variable for the device class
static char c;

static int my_open(struct inode *i, struct file *f)
{
  printk(KERN_INFO "Driver: open()\n");
  return 0;
}

static int my_close(struct inode *i, struct file *f)
{
  printk(KERN_INFO "Driver: close()\n");
  return 0;
}

static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
  printk(KERN_INFO "Driver: read() %i \n", (int)*off);

  if (*off != 0)
    return 0;

  if (copy_to_user(buf, &c, 1) != 0)
  {
    return -EFAULT;
  }
  else
  {
    *off += 1;
    return 1;
  }
}

static ssize_t my_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
  printk(KERN_INFO "Driver: write()\n");
  if (copy_from_user(&c, buf + len - 1, 1) != 0)
    return -EFAULT;
  else
    return len;
}

static struct file_operations ofcd_fops =
{
  .owner = THIS_MODULE,
  .open = my_open,
  .release = my_close,
  .read = my_read,
  .write = my_write
};

static int __init ofcd_init(void) /* Constructor */
{
  printk(KERN_INFO "ofcd registered\n");
  if (alloc_chrdev_region(&first, 0, 1, DEVICE_NAME) < 0)
  {
    return -1;
  }
  if ((cl = class_create(THIS_MODULE, DEVICE_TYPE)) == NULL)
  {
    unregister_chrdev_region(first, 1);
    return -1;
  }
  if (device_create(cl, NULL, first, NULL, NODE_DEVICE_NAME) == NULL)
  {
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    return -1;
  }
  cdev_init(&c_dev, &ofcd_fops);
  if (cdev_add(&c_dev, first, 1) == -1)
  {
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    printk(KERN_INFO "Unable to add character device");
    return -1;
  }

  printk(KERN_INFO "ofcd initiliazed\n");
  return 0;
}

static void __exit ofcd_exit(void) /* Destructor */
{
  cdev_del(&c_dev);
  device_destroy(cl, first);
  class_destroy(cl);
  unregister_chrdev_region(first, 1);
  printk(KERN_INFO "ofcd unregistered\n");
}

module_init(ofcd_init);
module_exit(ofcd_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rikard Söderström <YOUR_EMAIL>");
MODULE_DESCRIPTION("Works like /dev/null");
