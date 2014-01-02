# Makefile – makefile of our first driver
DIRS = .
SRC  = $(foreach dir, $(DIRS), $(wildcard $(dir)/*.[ch]))
CTAGS_FLAGS = --c++-kinds=+p --fields=+imaS --extra=+q+f

# if KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.
ifneq (${KERNELRELEASE},)
    obj-m := do_not_disturb.o
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


tags: $(SRC)
	ctags $(CTAGS_FLAGS) -f \.$@ $(SRC)
