### GPIO Drivers

In this tutorial I will take about how to access the gpio pins. To my help I had [Blackfin GPIO drivers](http://blackfin.uclinux.org/doku.php?id=gpio).


##### GPIO Allocation

For all linux drivers, you need to have resource management. In this case we need to know if a pin is in use by some other driver or not.
This checking is done **Linux GPIO Framework**, accessible to us in
```C++
  #include <asm/gpio.h>
```

And specifically through the functions
```C++
  int  gpio_request(unsigned int gpio, const char *label);
  void gpio_free(unsigned gpio);
```

The gpio index should always be retrieved from the machine specific gpio header `arch/blackfin/mach-bf527/include/mach/gpio.h`, while the label can be any text of your choice, the label is used by the gpio request service to tell the user who is using the pin. So it should be considered as an unique identifier.


The following code requests the pin connected to the onboard LED 1 when module is loaded, and releases it when module is removed.
```C++
static int __init gpio_blink_init(void) /* Constructor */
{
  int ret;

  ret = gpio_request(GPIO_PF8, "LED 1");
  if (ret) {
    printk(KERN_WARNING "gpio_blink: request denied on GPIO_PF8\n");
    return ret;
  }

  return 0;
}

static void __exit gpio_blink_exit(void) /* Destructor */
{
  gpio_free(GPIO_PF8);
  printk(KERN_INFO "gpio_blink unregistered\n");
}
```

##### GPIO Modifiers

Now when we own the resource we are free to modify its direction and value. This can be accomplished by the follwing functions. 

```C++
int  gpio_direction_input(unsigned gpio);
int  gpio_direction_output(unsigned gpio, int value);
 
void gpio_set_value(unsigned gpio, int value);
int  gpio_get_value(unsigned gpio);
```

The following is a complete code example of how to make LED 1 blink on load/unloading a driver.
```C++
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
```

##### GPIO Function reference

All in all these are the most useful functions for gpio allocation/modification and verification.
```C++
int  gpio_is_valid(int number);
 
int  gpio_request(unsigned gpio, const char *label);
void gpio_free(unsigned gpio);
 
int  gpio_direction_input(unsigned gpio);
int  gpio_direction_output(unsigned gpio, int value);
 
void gpio_set_value(unsigned gpio, int value);
int  gpio_get_value(unsigned gpio);
```

