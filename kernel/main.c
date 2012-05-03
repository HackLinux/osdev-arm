#include "funcs.h"
#include "interrupt.h"
#include "task.h"
#include "processor.h"
#include "print.h"
#include "support.h"
#include "syscall.h"

int print(char *s);
void setup_idt();
int arr[200];

extern int bss_start;
extern int bss_end;
extern int mem_start;


void arch_init();
void timer_init();
void mem_init();
void scheduler_init();

void bss_init()
{
	printk("%x %x \n", &bss_start, &bss_end);
	memset(&bss_start, 0, &bss_end - &bss_start);
} 


void parse_args(const char *args)
{
	printk("cmd line = %s\n", args);
}

int idle_thread()
{
	printk("In idle thread: %d\n", get_pid());
	int i=0;
	while (1) {
		sleep(1000, get_pid());
		log_info_str(Running %s: %d, get_task_name(), i++);
	}
	return 0;
}
int normal_thread()
{
	printk("Normal thread %d\n", get_pid());
	int i = 0;
	while (1) {
		sleep(5000, get_pid());
		log_info_str(Running %s: pid = %d %d, get_task_name(), get_pid(), i++);
	}
	return 0;
}
int main()
{
	printk("Entered main\n");
#define QEMU_CMDLINE_ADDR 0x12c
	parse_args((const char *)QEMU_CMDLINE_ADDR);
	bss_init();
	arch_init();
	timer_init();
	mem_init(&mem_start, 10<<20); //ask to manage 10 MB

	create_idle_thread(idle_thread, "idle", 0x113);
	scheduler_init();
#if 0
	__asm__ __volatile__("msr cpsr_c, #0x10");
#endif
	log_info_str(In main space);
#if 0
	change_mode(USR_MODE);
#endif
	create_thread(normal_thread, "normal_thread", 0x110);
	create_thread(normal_thread, "normal_thread", 0x110);
#if 0
	__asm__ __volatile__("msr cpsr_c, #0x11");
#endif
	log_info_str(In main space %d, 3);
	idle_thread();
	/* we shall never return here */
	/* end of world */
	
	 __asm__ __volatile__("swi #10"::);
	
	printk("into user land\n");
	
	while(1) {
		printk("sleep for 2 secs\n");
		sleep(2000, 10);
	}
	return 0;
}

void msg()
{
	printk("calling msg\n");

}

