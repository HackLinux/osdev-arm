#include <print.h>
#include <support.h>
#include <syscall_api.h>
#include <sched.h>
#include <timer.h>

void dump_regs();
void hard_loop();
void print_debug(int i)
{
    log_info_str("Running %s: %d, stop = %x, jiffies = %d", get_task_name(), i++, get_stack_top(), get_jiffies());
}


int idle_thread()
{
//	set_stack_top(get_current()->usr_stack_top);
	__asm__ __volatile__("mov sp, %0"::"r"(get_usr_stack()));
	//int i=100;
	while (1) {
	//    print_debug(i++);
        schedule();
        hard_loop();
	}
	return 0;
}
int normal_thread()
{
	//int i = 20;

    sleep(5000);
 //   disable_con_write();
	while (1) {
	    //print_debug(i++);
	    sleep(1000);
   //     print_rbuf();

    }
	return 0;
}

int normal_thread1()
{
	int i = 200;
	disable_con_write();
	while (1) {
   //     print_debug(i++);
        printk("normal thread 1 %d\n", i++);
	    hard_loop();
    	sleep(1000);
    	if (++i%10 == 0)
            print_rbuf();
	}
	exit(0);
	return 0;
}

