obj-m := gpio_switch.o
KERNELDR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

modules:
	make -C $(KERNELDR) M=$(PWD) modules
modules_install:
	make -C $(KERNELDR) M=$(PWD) modules_install
clean:
	rm -rf *.o *~core .depend .*.cmd *.ko *.mod.c .tmp_versions