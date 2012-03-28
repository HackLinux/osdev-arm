#include "print.h"
int handle_irq(int status)
{
	printk("in irq handler ,received interrupt on line %d \n", status);
	dump_cpsr(__func__);
	return status;
}
int handle_fiq(int status)
{
	printk("in fiq handler ,received interrupt on line %d \n", status);
	dump_cpsr(__func__);
	return status;

}
