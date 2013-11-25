obj-m := yaaah_device.o  


all:  
	make  -C /lib/modules/$(shell uname -r)/build  M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build -lgcc M=$(PWD) clean
