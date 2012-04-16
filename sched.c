#include "sched.h"


void schedule()
{
	printk("scheduler called \n");
}

void scheduler_init()
{
	set_current(get_pcb_with_pid(0));
}
