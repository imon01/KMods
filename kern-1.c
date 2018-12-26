

#include <linux/fs.h>  //used for devices
#include <linux/init.h> //init & clean macros
#include <linux/kernel.h> //kern_info
#include <linux/module.h>
#include <linux/moduleparam.h>



#define DRIVER_AUTHOR "AUTHOR_1"
#define DRIVER_DESC "test driver"

static int xk_var  __initdata= 3;

//Called by 'insmod'
static int __init init_mod(void)
{
	printk(KERN_INFO "Hello! \nvar is: %d\n", xk_var++);

	return 0;
}


//called by 'rmmod'
static void __exit exit_mod(void)
{
	printk(KERN_INFO "bye Felicia\n");

}


module_init(init_mod);
module_exit(exit_mod);


//Removing taints
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_SUPPORTED_DEVICE("test device");
