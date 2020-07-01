#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>
#include <linux/gpio.h>
#include <mach/map.h>
#include <mach/gpio.h>
#include <mach/regs-clock.h>
#include <mach/regs-gpio.h>
#include <linux/device.h>

int major = 0;
static struct class *adc_class;
static struct class_device	*adc_dev;

typedef struct Adc_Struct{
	unsigned int ADCCON;
	unsigned int reserve1;
	unsigned int ADCDLY;
	unsigned int ADCDAT;
	unsigned int reserve2;
	unsigned int reserve3;
	unsigned int CLRINTADC;
	unsigned int ADCMUX;
}Adc_Struct;


#define Adc_Reg_Base (0x126C0000)
#define VIR_ADDR(addr) (volatile Adc_Struct*)(ioremap(addr,sizeof(Adc_Struct)))
#define RELEASE_VIR_ADDR(vir_addr) (iounmap(vir_addr))

Adc_Struct* p_Adc_Struct = NULL;

static DECLARE_WAIT_QUEUE_HEAD(adc_waitq);

static volatile int adc_can_use = 0;
static unsigned int adc_val;



static irqreturn_t adc_irq(int irq, void *dev_id)
{

	//读取adc 转换值
	adc_val = p_Adc_Struct->ADCDAT;
	printk("adc_val:%d",adc_val);
	adc_can_use = 1;                  /* 表示中断发生了 */
    wake_up_interruptible(&adc_waitq);   /* 唤醒休眠的进程 */

	p_Adc_Struct->CLRINTADC |= (1 << 0);
	
	return IRQ_RETVAL(IRQ_HANDLED);
}


static int adc_open(struct inode *inode, struct file *file){
	//初始化adc
	p_Adc_Struct->ADCCON |= (1 << 16);
	p_Adc_Struct->ADCCON |= (1 << 14);
	p_Adc_Struct->ADCCON &= ~(0xFF << 6);
	p_Adc_Struct->ADCCON |= (65 << 6);
	p_Adc_Struct->ADCCON &= ~(1 << 2);
	p_Adc_Struct->ADCCON &= ~(1 << 1);
	p_Adc_Struct->ADCMUX &= ~(0x0F << 0);
	p_Adc_Struct->CLRINTADC |= (1 << 0);
	
	//注册中断
	request_irq(IRQ_ADC,  adc_irq, IRQ_TYPE_EDGE_BOTH, "adc", NULL);
	
	return 0;
}

ssize_t adc_read(struct file *file, char __user *buf, size_t size, loff_t *ppos){
	//开始转换
	p_Adc_Struct->ADCCON |= (1 << 0);
	//没有数据则休眠
	wait_event_interruptible(adc_waitq, adc_can_use);
	//有数据返回应用层
	copy_to_user(buf, &adc_val, 4);
	adc_can_use = 0;
	return 0;
}

int adc_close(struct inode *inode, struct file *file){

	free_irq(IRQ_ADC, NULL);
	
	return 0;
}





static struct file_operations adc_fops = {
    .owner   =  THIS_MODULE,    /* 这是一个宏，推向编译模块时自动创建的__this_module变量 */
    .open    =  adc_open,     
	.read	 =	adc_read,	   
	.release =  adc_close,
};


static int __init myAdc_Init(void){

	major = register_chrdev(0, "myAdc", &adc_fops);
	adc_class = class_create(THIS_MODULE, "myAdc");
	adc_dev = device_create(adc_class, NULL, MKDEV(major, 0), NULL, "myAdc"); 
	p_Adc_Struct = VIR_ADDR(Adc_Reg_Base);
	return 0;

}

static void __exit myAdc_Exit(void){
	unregister_chrdev(major, "fifth_drv");
	class_destroy(adc_class);
	device_destroy(adc_class,MKDEV(major, 0));
	RELEASE_VIR_ADDR(p_Adc_Struct);
}





/* 这两行指定驱动程序的初始化函数和卸载函数 */
module_init(myAdc_Init);
module_exit(myAdc_Exit);

/* 描述驱动程序的一些信息，不是必须的 */
MODULE_AUTHOR("Low Bee");
MODULE_VERSION("0.1.0");
MODULE_DESCRIPTION("Adc Driver For Android");
MODULE_LICENSE("GPL");



