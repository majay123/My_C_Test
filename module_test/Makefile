# make on ubuntu 
KERNELDIR = /lib/modules/$(shell uname -r)/build

PWD := $(shell pwd)

obj-m += module_hello.o

all:
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean