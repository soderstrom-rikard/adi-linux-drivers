#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <mach/defBF527.h>

static int __init gpio_blink_init(void) /* Constructor */
{
  // Turn off LED 1
  short *direction = (short *)PORTFIO_DIR;
  short *set       = (short *)PORTFIO_SET;
  *direction |= 0x0100;
  *set &= 0xFEFF;

  return 0;
}

static void __exit gpio_blink_exit(void) /* Destructor */
{
  // Turn on LED 1
  short *direction = (short *)PORTFIO_DIR;
  *direction &= 0xFEFF;
}

module_init(gpio_blink_init);
module_exit(gpio_blink_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rikard Söderström <YOUR_EMAIL>");
MODULE_DESCRIPTION("Turns off LED 1 on load and turns it back on, when unloaded");
