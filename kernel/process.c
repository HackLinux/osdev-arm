#include "print.h"
#include "support.h"
#include "syscall_api.h"

void dump_regs();
int idle_thread()
{
//	set_stack_top(get_current()->usr_stack_top);
	__asm__ __volatile__("mov sp, %0"::"r"(get_usr_stack()));
	printk("In idle thread: %d\n", get_pid());
	int i=100;
	int stop;
	while (1) {
		get_stack_top();
		log_info_str("Running %s: %d, stop = %x", get_task_name(), i++, stop);
		schedule();
	}
	return 0;
}
int normal_thread()
{
	printk("running %d\n", get_pid());
	int i = 20;
	int stop;
	while (1) {
		get_stack_top();
		log_info_str("Running %s: pid = %d,%d, stop = %x", get_task_name(), get_pid(), i++, stop);
		sleep(10000);
	}
	return 0;
}

int normal_thread1()
{
	printk("running %d\n", get_pid());
	int i = 200;
	int stop = 0;
//	while (1) {
		log_info_str("before schedule %s: pid = %d,%d, stop = %x", get_task_name(), get_pid(), i++, stop);
		schedule();
		sleep(1000);
		log_info_str("after schedule %s: pid = %d,%d, stop = %x", get_task_name(), get_pid(), i++, stop);
//	}
	exit(0);
	return 0;
}

