#include "print.h"
#include "support.h"
#include "syscall_api.h"

void dump_regs();
void print_debug(int i)
{
    log_info_str("Running %s: %d, stop = %x, jiffies = %d", get_task_name(), i++, get_stack_top(), get_jiffies());
}


int idle_thread()
{
//	set_stack_top(get_current()->usr_stack_top);
	__asm__ __volatile__("mov sp, %0"::"r"(get_usr_stack()));
	int i=100;
	while (1) {
	    print_debug(i++);
        schedule();
        hard_loop();
	}
	return 0;
}
int normal_thread()
{
	int i = 20;
	while (1) {
	    print_debug(i++);
	    sleep(1000);
	}
	return 0;
}

int normal_thread1()
{
	int i = 200;
	while (1) {
	    print_debug(i++);
		sleep(5000);
	}
	exit(0);
	return 0;
}

