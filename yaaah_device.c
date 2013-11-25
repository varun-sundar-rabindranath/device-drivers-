#include<linux/module.h>
#include<linux/init.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/string.h>
#include<linux/slab.h>
#include<linux/uaccess.h>

#define CHECK_ERR(retval) if(retval==0) \
			  {		\
					pr_info("Function call success \n"); \
			  }						  \
			  else 					  \
			  {						  \
					pr_info("Function call unsuccessful \n"); \
			  }						  		 
				

dev_t dev_no;
struct cdev *char_dev;

int in_use = 0;

char text[100] = "You are a  ";

ssize_t my_read(struct file *file_ptr, char __user * buffer, size_t size, loff_t *off)
{
		
	int kern_alert = KERN_ALERT;

	printk(KERN_ALERT " KERN_ALERT : %d ",kern_alert);
	if(*off != 0)
	{
		return 0;
	}

	int copied = copy_to_user(buffer, text, strlen(text));
	CHECK_ERR(copied);
	*off = *off + strlen(text) - copied;
	return strlen(text)-copied;
}

ssize_t my_write(struct file * file_ptr, const char __user * buffer_user, size_t size, loff_t *off)
{
	pr_info("in write");
	int i;
	
	char buffer[100];
	copy_from_user(buffer, buffer_user, size);
	
	for(i=0;i<size;i++)
	{
		text[i+10] = buffer[i];
	}
	return strlen(text);	
}
	
int open_device(struct inode * inode_ptr, struct file * file_ptr)
{
	if(in_use==1)
	{
		pr_info("Device already in use \n");
		return -1;
	}
	in_use = 1;
	return 0;
}



int close_device(struct inode * inode_ptr, struct file * file_ptr)
{
	in_use = 0;
	pr_info("Device released \n");
	return 0;
}

struct file_operations fops={
	.open = open_device,
	.release = close_device,
	.read = my_read,
	.write = my_write,
};

int hello(void)
{
	int retval;

	pr_info("Inside module init \n");
	retval = alloc_chrdev_region(&dev_no, 0, 1, "yaaah");
	pr_info("Dev no : %d , Device name : yaaah", MAJOR(dev_no));
	CHECK_ERR(retval);
	char_dev = cdev_alloc();
	char_dev->ops = &fops;
	retval = cdev_add(char_dev, dev_no, 1);
	CHECK_ERR(retval);
	return 0;
}

void bye(void)
{
	pr_info("Exiting module \n");

	unregister_chrdev_region(dev_no, 1);
	cdev_del(char_dev);
}

module_init(hello);
module_exit(bye);
