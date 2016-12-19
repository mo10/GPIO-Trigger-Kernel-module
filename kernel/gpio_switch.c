#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("XiaoLu");
MODULE_DESCRIPTION("GPIO Switch Trigger");
MODULE_VERSION("0.1");

static unsigned int gpioSwitch  = 4; //开关的GPIO号
static unsigned int irqNumber;
static unsigned int numSwitched = 0;

//中断处理
static irq_handler_t  switch_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs){
    if(gpio_get_value(gpioSwitch)){
        //开关拨到ON
        printk(KERN_INFO "Switch_Trigger: Switch turn ON\n");
    }else{
        //开关拨到OFF
        printk(KERN_INFO "Switch_Trigger: Switch turn OFF\n");
    }
    numSwitched++;
    return (irq_handler_t) IRQ_HANDLED;
}

//初始化 Kernel module
static int __init switch_init(void){
    int result = 0;

    printk(KERN_INFO "Switch_Trigger: Initializing the Switch_Trigger\n");
    //如果该GPIO是无效的
    if (!gpio_is_valid(gpioSwitch)){
        printk(KERN_INFO "Switch_Trigger: invalid GPIO\n");
        return -ENODEV;
    }
    //设置GPIO
    gpio_request(gpioSwitch, "sysfs");
    gpio_direction_input(gpioSwitch);
    gpio_set_debounce(gpioSwitch, 500); //GPIO消抖,但是经过测试是没鸟用的
    gpio_export(gpioSwitch, false);

    printk(KERN_INFO "Switch_Trigger: The switch state is currently: %d\n", gpio_get_value(gpioSwitch));
    //GPIO映射到IRQ

    irqNumber = gpio_to_irq(gpioSwitch);
    result = request_irq(irqNumber,                           // 请求的中断号
                         (irq_handler_t) switch_irq_handler,  // 处理中断函数
                         IRQF_TRIGGER_RISING,                 // 中断触发
                         "switch_irq_handler",                // 用于 /proc/interrupts 中的所有者名称
                         NULL);

    printk(KERN_INFO "Switch_Trigger: The switch is mapped to IRQ: %d\n", irqNumber);
    printk(KERN_INFO "Switch_Trigger: The interrupt request result is: %d\n", result);
    return result;
}
//卸载 Kernel module
static void __exit switch_exit(void){
    printk(KERN_INFO "Switch_Trigger: The switch state is currently: %d\n", gpio_get_value(gpioSwitch));
    printk(KERN_INFO "Switch_Trigger: The switch was switched %d times\n", numSwitched);

    free_irq(irqNumber, NULL);               // 释放IRQ
    gpio_unexport(gpioSwitch);               // 取消导出GPIO
    gpio_free(gpioSwitch);                   // 释放GPIO
    printk(KERN_INFO "Switch_Trigger: Goodbye!\n");
}

module_init(switch_init);
module_exit(switch_exit);
