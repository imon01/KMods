/*

	RO char device. Reports times read from dev file.


*/


#include <linux/fs.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/uaccess.h> 			//for put_user

#define SUCCESS 0
#define DEVICE_NAME "CHAR_DEVICE" 	//device name in /proc/device
#define BUF_LEN 80					//Max len from device message

//Prototypes: should be in header

int init_module(void);
void cleanup_module(void);

static int device_open(struct inode*, struct file*); 
static int device_release( struct inode*, struct file*);
static ssize_t device_read( struct file*, char*, size_t, loff_t*);
static ssize_t device_write( struct file*, const char*, size_t, loff_t*);

static int Major		= 0;		//Major number assigned to device
static int dev_open 	= 0;		//Device open flag, prevents multiple access to device
static char msg[BUF_LEN];			//msg buffer to device
static char *msg_ptr;



static struct file_operations fops = 
{
	.read	 	= device_read,
	.write		= device_write,
	.open		= device_open,
	.release	= device_release
};



// Kmod loadable function
int init_module(void)
{

	Major = register_chrdev(0, DEVICE_NAME, &fops);
	
	if( Major < 0 )
	{
		printk( KERN_INFO "Register Char Device Failed: %d\n", Major);
		return Major;
	}

	printk( KERN_INFO "Assigned major number: %d\n", Major);
	printk( KERN_INFO "To msg driver, create dev file w/: 'mknod /dev/%s c %d 0'\n", DEVICE_NAME, Major);
	printk( KERN_INFO "Try various minor numbers. Try 'cat' & 'echo' to dev file\n");
	printk( KERN_INFO "Remove device file and module when done\n");

	return SUCCESS;
}




void cleanup_module(void)
{
	ret = unregister_chrdev(Major, DEVICE_NAME);
	printk( KERN_INFO "chardev module unloaded\n");
}

/*
* Called when  a process tries to open the device file
* 	eg: 'cat /dev/thischarfile'
*
*/
static int device_open( struct inode* inode, struct file* file)
{
	static int counter = 0;
	

	if( dev_open)
		return -EBUSY;

	++dev_open;
	sprintf(msg, "char dev read, count: %d\n", counter);
	++counter;
	msg_ptr = msg;
	try_module_get(THIS_MODULE);
	return SUCCESS;
}

	
/*
* Called when  a process releases the device file
*
*/
static int device_release( struct inode* inode, struct file* file)
{

	--dev_open;	

	//Decrement usage count. Otherwise if file opened you'll never get rid of the module.
	module_put(THIS_MODULE);

	return SUCCESS;
}


//Called when a process has the dev file opened, attempts reading from it
static ssize_t device_read( struct file* file_ptr, char* buf, size_t len, loff_t* offset)
{
	int bytes_read = 0;

	//if at end of message, return 0 for EOF
	if( *msg_ptr == 0)
		return 0;

	//put data in buffer
	while( len & *msg_ptr )
	{
		//Buffer is in USER data segment. "*" won't work.
		//Must use 'put_user' to copy from kernel data segment to 
		// 	user data segment
		put_user( *(msg_ptr++), buffer++);
	
	}


	return bytes_read;
}



static ssize_t device_write( struct file* file_ptr, const char* buf, size_t len, loff_t* offset)
{
	printk( KERN_ALERT "Device write not supported\n");
	return -EINVAL;
}
