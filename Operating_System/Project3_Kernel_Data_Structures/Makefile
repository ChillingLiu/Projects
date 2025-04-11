KBUILD_CFLAGS += -w
obj-m += color.o
obj-m += collatz.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
