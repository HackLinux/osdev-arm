#include "print.h"
int fiq_handle()
{
	printk("In fiq handler\n");
	dump_cpsr(__func__);

}
