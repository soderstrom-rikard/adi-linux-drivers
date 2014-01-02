### Using the GPIO driver

In this tutorial about linux drivers, I will show how to disable/enable interrupts.

##### Disabling Interrupts

```C++
unsigned long irq_flags;
...

local_irq_save(irq_flags);
local_irq_disable();
```

##### Enabling Interrupts


```C++
local_irq_restore(irq_flags);
local_irq_enable();
```

##### Complete Example

```C++
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>

static unsigned long irq_flags;

static int __init do_not_disturb_init(void)
{
  printk(KERN_INFO "do_not_disturb registered\n");
  // Disable interrupts (only allowed for a short period of time)
  local_irq_save(irq_flags);
  local_irq_disable();
  printk(KERN_INFO "do_not_disturb interrupts disabled\n");
  // Enable Interrupts
  local_irq_restore(irq_flags);
  local_irq_enable();
  printk(KERN_INFO "do_not_disturb interrupts enabled\n");
  return 0;
}

static void __exit do_not_disturb_exit(void)
{
  printk(KERN_INFO "do_not_disturb unregistered\n");
}

module_init(do_not_disturb_init);
module_exit(do_not_disturb_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rikard Söderström <YOUR_EMAIL>");
MODULE_DESCRIPTION("While the  \"do not disturb\" driver is loaded, all interrupts are disabled.");
```

##### References
[Disabling and enabling interrupts](http://www.kernelfaq.com/2007/07/enabling-and-disabling-interrupts.html)
