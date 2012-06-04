#include "print.h"
#include "support.h"

void dump_regs();
int idle_thread()
{
//	set_stack_top(get_current()->usr_stack_top);
	__asm__ __volatile__("mov sp, %0"::"r"(get_usr_stack()));
	printk("In idle thread: %d\n", get_pid());
	int i=100;
	int stop;
	while (1) {
		sleep(1000, get_pid());
		__asm__ __volatile("mov %0, sp" :"=r"(stop)::);
		log_info_str("Running %s: %d, stop = %x", get_task_name(), i++, stop);
	//	dump_regs();
	}
	return 0;
}
int normal_thread()
{
	printk("Normal thread %d\n", get_pid());
	int i = 20;
	int stop = 0;
	while (1) {
		sleep(1000, get_pid());
	//	dump_regs();
		__asm__ __volatile("mov %0, sp" :"=r"(stop)::);
		log_info_str("Running %s: pid = %d,%d, stop = %x", get_task_name(), get_pid(), i++, stop);
	 	__asm__ __volatile__("swi #20"::);
		log_info_str("back to user space %s: pid = %d,%d, stop = %x", get_task_name(), get_pid(), i++, stop);
	//	dump_regs();
	}
	return 0;
}

int normal_thread1()
{
	printk("Normal thread1 %d\n", get_pid());
	int i = 200;
	int stop = 0;
	while (1) {
		__asm__ __volatile("mov %0, sp" :"=r"(stop)::);
		log_info_str("before schedule %s: pid = %d,%d, stop = %x", get_task_name(), get_pid(), i++, stop);
		schedule();
		log_info_str("after schedule %s: pid = %d,%d, stop = %x", get_task_name(), get_pid(), i++, stop);
	//	dump_regs();
	}
	return 0;
}

