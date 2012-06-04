#include "print.h"
#include "support.h"

void dump_regs();
int idle_thread()
{
	printk("In idle thread: %d\n", get_pid());
	int i=0;
	while (1) {
		sleep(1000, get_pid());
		log_info_str("Running %s: %d", get_task_name(), i++);
		dump_regs();
	}
	return 0;
}
int normal_thread()
{
	printk("Normal thread %d\n", get_pid());
	int i = 0;
	while (1) {
		sleep(1000, get_pid());
		dump_regs();
		log_info_str("Running %s: pid = %d,%d", get_task_name(), get_pid(), i++);
	 	__asm__ __volatile__("swi #20"::);
		log_info_str("back to user space id = %d\n", get_pid());
		dump_regs();
	}
	return 0;
}

