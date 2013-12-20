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
MODULE_AUTHOR("Rikard Söderström <YOUR_EMAIL>");
MODULE_DESCRIPTION("Hello world driver");
