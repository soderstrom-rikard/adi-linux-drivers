### Character Drivers)

In this tutorial step I once again get inspired by **Linux Drivers Articles**.

* [Linux Character Drivers](http://www.linuxforu.com/2011/02/linux-character-drivers/)
* [Device Files Creation](http://www.linuxforu.com/2011/04/character-device-files-creation-operations/)
* [Device Files Operations](http://www.linuxforu.com/2011/05/decoding-character-device-file-operations/)


##### Defining fileoperation callbacks.

In order to perform fileoperations on device files, we need to define callback functions for open/close and read/write.

```C++
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
```

Once we have these callbacks we can move on to connecting them.

##### Connecting callback functions with file operations.

```C++

static struct file_operations pugs_fops =
{
  .owner = THIS_MODULE,
  .open = my_open,
  .release = my_close,
  .read = my_read,
  .write = my_write
};

```

###### Creating the device and filesystem node

The following code snipped demonstrates how to create a character device driver, and populating /dev with a device node.
```C++

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
  cdev_init(&c_dev, &pugs_fops);
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
```
