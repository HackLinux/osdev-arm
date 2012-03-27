#include "print.h"
int irq_handle()
{
	printk("In irq handler\n");
	dump_cpsr(__func__);

}
