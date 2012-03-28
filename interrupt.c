#include "print.h"

static intr_handler irq_h[32] = {};

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


int request_irq(int irqno, int mode, intr_handler h)
{
}
