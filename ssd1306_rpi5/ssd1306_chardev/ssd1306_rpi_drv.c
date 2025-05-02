#include <linux/module.h>	//module_init, module_exit, MODULE_XXX
#include <linux/kernel.h>	//printk
#include <linux/fs.h>		//struct file_operations, register_chrdev_region
#include <linux/init.h>		//__init, __exit
#include <linux/device.h>	//class_create, device_create
#include <linux/kdev_t.h>	//MAJOR(), MINOR()
#include <linux/uaccess.h>	//copy_from_user
#include <linux/i2c.h>		//struct i2c_client, i2c_add_driver
#include <linux/cdev.h>		//struct cdev, cdev_init, cdev_add
#include <linux/mm.h>		//vm_area_struct, remap_pfn_range
#include "ssd1306.h"
#include "ssd1306_mmap.h"

#define SUCCESS 0
#define DEVICE_NAME "ssd1306"

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 32
#define SSD1306_ADDR 0x3C

static int contrast;

u8 *oled_fb = NULL; //framebuffer

static struct class *oled_class=NULL; 
static ssd1306_t ssd1306_dev;
static dev_t devid=0;
static struct i2c_client *oled_client;
static struct cdev i2c_cdev;

static unsigned short address_list[] = {SSD1306_ADDR, I2C_CLIENT_END};

static int oled_open(struct inode *inode, struct file *file)
{
	ssd1306_init(&ssd1306_dev, SSD1306_WIDTH,SSD1306_HEIGHT,SSD1306_ADDR, oled_client);
    printk("oled_open()\n");

    return SUCCESS;
}

static int oled_release(struct inode *inode, struct file *file)
{
   // ssd1306_clear(&ssd1306_dev);
   // ssd1306_show(&ssd1306_dev);
    printk("oled_release()\n");

    return SUCCESS;
}

/* Called when a process, which already opened the dev file, attempts to write from it. 
 * struct file *filp: the file operation
 * char *buffer: The buffer to fill with data
 * size_t length: The length of the buffer
 * loff_t *offset: Our offset in the file
 */

/* Called when a process writes to dev file: echo "hi" > /dev/oled */
static ssize_t oled_write(struct file *filp, const char *buff, size_t len, loff_t *off)
{    
	char data[128]={0};
	if (len >= sizeof(data)){
		len = sizeof(data)-1;
	}

    if (copy_from_user(data, buff, len)!=0){
		printk("copy_from_user failed");
		return -EFAULT;
    }	
    data[len] = '\0';
    ssd1306_clear(&ssd1306_dev);
//    ssd1306_show(&ssd1306_dev);
    ssd1306_draw_string(&ssd1306_dev, 0, 0, data);
    ssd1306_show(&ssd1306_dev, STRING);
    printk("oled_write()\n");
    
    return len;
}

static int oled_mmap(struct file *file, struct vm_area_struct *vma){
	unsigned long size = vma->vm_end - vma->vm_start; //virtual memory
	
	if (size > OLED_FB_SIZE){
		return -EINVAL;
	}

	if(remap_pfn_range(vma, vma->vm_start, virt_to_phys(oled_fb) >> PAGE_SHIFT, size, vma->vm_page_prot)){
		return -EAGAIN;
	}
	printk("oled_mmap() succeed\n");
	return 0;
} 

static long oled_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
	switch (cmd){
		case OLED_CLEAR: //refresh screen
			ssd1306_clear(&ssd1306_dev);
			ssd1306_show(&ssd1306_dev, STRING);
			break;
		case OLED_CONTRAST:
			if (copy_from_user(&contrast, (int __user*)arg, sizeof(int))!=0){
				return -EFAULT;
			}
			ssd1306_send_cmd(&ssd1306_dev, SET_CONTRAST);
			ssd1306_send_cmd(&ssd1306_dev, contrast);
			break;
		case OLED_CLEAR_FB:
			ssd1306_clear_frambuf(oled_fb);
			break;
		case OLED_MMAP_DRAW:
			ssd1306_show(&ssd1306_dev, FIG);
			break;		
		default:
			return -EINVAL;
	}
	printk("oled_ioctl(): cmd = %u, arg = %lx\n", cmd, arg);
	return 0;
}


static struct file_operations fops = {
    .owner = THIS_MODULE,		//avoid this module to be removed while being opened by user space, but the module can be removed after calling release()
    .write = oled_write,
    .open = oled_open,
    .release = oled_release,
	.mmap = oled_mmap,
	.unlocked_ioctl = oled_ioctl,
};

//i2c driver probe/remove/detect
static int ssd1306_probe(struct i2c_client *client){

    int ret;
    ret = alloc_chrdev_region(&devid, 0, 1, "oled"); //allocate the device number, 0 means minor number start from 0, 1 means the amount of devices that you want to register
    if (ret < 0) {
		printk ("alloc_chrdev_region() fail\n");
        return ret;
    }
    else {
		printk("alloc device number: major=%d, minor=%d succeed\n", MAJOR(devid), MINOR(devid));
    }
    
	//char dev initialize
    cdev_init(&i2c_cdev, &fops);  
    ret = cdev_add(&i2c_cdev, devid, 1);//add char device, 1 means the amount of devices (minor device count)
    if(ret < 0) {
        printk ("Error adding cdev\n");
        return ret;
    }

	//create class
    oled_class = class_create("oled_class"); 
    if(IS_ERR(oled_class)) {
    	printk("Err: failed in creating class.\n");
    	return -1;
    }
  
	//create /dev/oled
    device_create(oled_class, NULL, devid, NULL, "oled");
    printk(KERN_INFO "create device file 'oled' succeed\n");

	//save the client info
    oled_client = client;
    printk(KERN_INFO "get i2c_client, client name = %s, addr=0x%x\n", oled_client->name, oled_client->addr);
    printk(KERN_INFO "get i2c_adapter, adapter name=%s\n", oled_client->adapter->name);
	
	oled_fb = kmalloc(OLED_FB_SIZE, GFP_KERNEL);
	if (!oled_fb){
		printk("Failed to allocate framebuffer\n");
		return -ENOMEM;
	}

    return SUCCESS;
}

EXPORT_SYMBOL(oled_fb);

static void ssd1306_remove(struct i2c_client *client){
    
	kfree(oled_fb);
	oled_fb = NULL;
    device_destroy(oled_class, devid); 
    class_destroy(oled_class); 

    cdev_del(&i2c_cdev);
    unregister_chrdev_region(devid, 1);
    printk("unregister_chrdev\n");

}

static int ssd1306_detect(struct i2c_client *client, struct i2c_board_info *info){
	printk("oled detect success\n");
	strcpy(info->type, "oled");
	return SUCCESS;
}

static const struct i2c_device_id ssd1306_id[] = {
	{"oled",0},	//device name (must match with driver name)
	{},
};
MODULE_DEVICE_TABLE(i2c, ssd1306_id);

static struct i2c_driver ssd1306_driver = {
	.driver = {
		.name = "oled",	//driver name (must match with device name)
		.owner = THIS_MODULE,
	},
	.probe = ssd1306_probe,
	.remove = ssd1306_remove,
	.detect = ssd1306_detect,
	.id_table = ssd1306_id, //load the device name info to match with the driver, if match, probe() will be called
	.address_list = address_list,
};


static struct i2c_board_info oled_info = {
	I2C_BOARD_INFO("oled", SSD1306_ADDR),	//match the device name in id_table
};

static int __init oled_init(void){
	printk(KERN_ALERT"oled_init");

	//get the I2C bus struct of /dev/i2c-"1"
	struct i2c_adapter *adapter = i2c_get_adapter(1); //call get_adapter with reference count++
	if(!adapter){
		return -ENODEV;
	}
	
	/*
	use i2c_new_client_device() and i2c_add_driver() to register the device and the driver, if the device name and the driver name match, then probe() will be called by kernel
	*/

	//create a new I2C slave device (i2c_client) with I2C bus & address info
	oled_client = i2c_new_client_device(adapter, &oled_info); 
	
	//release adapter with reference count--
	i2c_put_adapter(adapter);

	//register the driver
	i2c_add_driver(&ssd1306_driver);

	return SUCCESS;
}
module_init(oled_init);

static void __exit oled_exit(void){
	i2c_unregister_device(oled_client);
	i2c_del_driver(&ssd1306_driver);
	printk(KERN_ALERT"oled_exit");
}
module_exit(oled_exit);
MODULE_LICENSE ("GPL");
MODULE_AUTHOR("JNY");
MODULE_DESCRIPTION("SSD1306 I2C OLED driver");
