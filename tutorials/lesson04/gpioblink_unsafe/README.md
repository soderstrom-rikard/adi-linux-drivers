### GPIO Blinking - The unsafe way

Accessing the hardware directly is quite simple and its just a matter of knowing your hardware.
The part that makes this approach unsafe is that is skips all the resource allocation etc (meaning if some other driver uses your pin it might have very unexpected behavior).
But as long as you are sure that you and only you will ever in all eternity use that pin, then maybe it is okay to do this..

I am using Blackfin BF52x family, thus I look at [Processor Hardware Specification](http://www.analog.com/static/imported-files/processor_manuals/BF52xProcHWR031.pdf).

##### Modifying GPIOs


In order to select or deselect a pin as input/output you need to change its DIR register.
```
  Ref. Manual Chapter 8-30
  PORTxIO_DIR
  |Px15|Px14|...|Px0|   16-bit register
  For all bits, 0 - Input, 1 - Output

  Example:
  Port F, Pin 8, Set as output
  PORTFIO_DIR |= 0x0100
```

When the pin is an output pin, to change its value, you need to modify its SET register.
```
  Ref. Manual Chapter 8-30
  PORTxIO_SET
  |Px15|Px14|...|Px0|   16-bit register
  For all bits, 0 - Clear, 1 - Set

  Example:
  Port F, Pin 8, Clear
  PORTFIO_SET &= 0xFEFF
```

The port register definitions can be found in **linux-kernel/arch/blackfin/mach-bf527/include/mach/defBF527.h**, basically they tell you at wich address a specific register is.
