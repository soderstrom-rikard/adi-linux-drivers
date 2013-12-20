### Using the GPIO driver

In this first tutorial about linux drivers I will simply present a hello world module.

##### Write your hello.c file

```C++
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>

static int __init hello_init(void)
{
  printk(KERN_INFO "Init: hello registered\n");
  return 0;
}

static void __exit hello_exit(void)
{
  printk(KERN_INFO "Exit: hello unregistered\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YOUR_NAME <YOUR_EMAIL>");
MODULE_DESCRIPTION("Hello world driver");
```


##### Export ADI root path and your CROSS COMPILE variable.

```bash
   $ export ADI_ROOT=/home/rikard/github/adi-root
   $ export CROSS_COMPILE=bfin-uclinux-
````

##### Write your Makefile

```Makefile
# Makefile – makefile of our first driver
 
# if KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.
ifneq (${KERNELRELEASE},)
    obj-m := hello.o
# Otherwise we were called directly from the command line.
# Invoke the kernel build system.
else
    KERNEL_SOURCE := $(ADI_ROOT)/build/buildroot/build/linux-custom/
    PWD := $(shell pwd)
default:
	${MAKE} -C ${KERNEL_SOURCE} SUBDIRS=${PWD} modules
 
clean:
	${MAKE} -C ${KERNEL_SOURCE} SUBDIRS=${PWD} clean
endif
```

##### Build your module

```bash
   $ make
````

##### Using your module

```bash
   root> insmod hello.ko
   root> lsmod | grep hello
   root> infomod hello.ko
   root> rmmod hello.ko
````

##### Reference
[Building our first Linux driver](http://www.linuxforu.com/2010/12/writing-your-first-linux-driver)
