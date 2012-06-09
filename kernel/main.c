#include "funcs.h"
#include "interrupt.h"
#include "task.h"
#include "processor.h"
#include "print.h"
#include "support.h"

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
int idle_thread();
int normal_thread();
int normal_thread1();

#define QEMU_CMDLINE_ADDR 0x12c

void bss_init()
{
	printk("%x %x \n", &bss_start, &bss_end);
	memset(&bss_start, 0, &bss_end - &bss_start);
} 


void parse_args(const char *args)
{
	printk("cmd line = %s\n", args);
}

int main()
{
	printk("Entered main\n");
	parse_args((const char *)QEMU_CMDLINE_ADDR);
	bss_init();
	arch_init();
	timer_init();
	mem_init(&mem_start, 10<<20); //ask to manage 10 MB

	create_idle_thread(idle_thread, "idle", 0x11f);
	scheduler_init();
	mmu_init();	
	
	create_thread(normal_thread, "normal_thread", 0x110);
	create_thread(normal_thread, "normal_thread1", 0x110);
	create_thread(normal_thread1, "screw_up", 0x11f);
	

	__asm__ __volatile__("msr cpsr_c, #0x1f");
	
	log_info_str("In main space %40d", 1<<31);
	
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

