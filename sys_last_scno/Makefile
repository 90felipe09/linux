obj-y := sys_last_scno.o

all:
	make -C /lib/modules/$(shell uname -r)/build=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean