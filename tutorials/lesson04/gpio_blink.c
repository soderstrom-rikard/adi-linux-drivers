#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <asm/gpio.h>

static int __init gpio_blink_init(void) /* Constructor */
{
  int ret;
  printk(KERN_INFO "gpio_blink registered %d \n", GPIO_PF8);

  ret = gpio_request(GPIO_PF8, "LED 1");
  if (ret) {
    printk(KERN_WARNING "gpio_blink: request denied on GPIO_PF8\n");
    return ret;
  }

  ret = gpio_direction_output(GPIO_PF8, 1);
  if (ret) {
    printk(KERN_WARNING "gpio_blink: unable to set GPIO_PF8 direction to output\n");
    return ret;
  }

  gpio_set_value(GPIO_PF8, 0);

  return 0;
}

static void __exit gpio_blink_exit(void) /* Destructor */
{
  int ret;

  gpio_set_value(GPIO_PF8, 1);

  ret = gpio_direction_input(GPIO_PF8);
  if (ret) {
    printk(KERN_WARNING "gpio_blink: unable to set GPIO_PF8 direction to input\n");
  }

  gpio_free(GPIO_PF8);
  printk(KERN_INFO "gpio_blink unregistered\n");
}

module_init(gpio_blink_init);
module_exit(gpio_blink_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rikard Söderström <YOUR_EMAIL>");
MODULE_DESCRIPTION("Turns off LED 1 on load and turns it back on, when unloaded");
