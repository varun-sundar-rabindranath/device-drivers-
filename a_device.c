#include<linux/module.h>
#include<linux/init.h>
#include<linux/cdev.h>
#include<linux/fs.h>
//hellokl
dev_t devno;
struct cdev *chr_dev;
struct anex
{
  int a;
  struct cdev *chr_dev;
};
int open_device (struct inode *ino, struct file *filp)
{
  printk(KERN_INFO " inside  open \n");
  printk(KERN_INFO " position of  cdev in inode !    %p\n",ino->i_cdev);
  printk(KERN_INFO " position of  fops  in filp !    %p\n",filp->f_op);

  return 0;
}
int close_device (struct inode *ino, struct file *filp)
{
  printk(KERN_INFO "inside close \n");
  return 0;
}
struct file_operations fops ={
  .open=open_device,
  .release=close_device,
};
static int hello(void)
{
  printk(KERN_ALERT "hello , entering a char dev  \n");
  alloc_chrdev_region(&devno,0,1, "dummy_device")==0?printk(KERN_INFO " region allocated "): printk(KERN_INFO " region not  allocated\n");
  printk( KERN_INFO " the dedvice major  number  %d",MAJOR(devno));
  chr_dev  = cdev_alloc();
  chr_dev->ops  = &fops;
  cdev_add(chr_dev,devno,1)==0?printk(KERN_INFO " cdev added \n"):printk(KERN_INFO " cdev not added \n");
  printk(KERN_INFO " postion of fops and cdev  %p     %p",&fops,chr_dev);
  return 0;
}
void bye(void)
{  
  unregister_chrdev_region(devno,1);
  cdev_del(chr_dev);
}
module_init(hello);
module_exit(bye); 
