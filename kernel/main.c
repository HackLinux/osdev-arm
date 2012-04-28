#include "funcs.h"
#include "interrupt.h"
#include "task.h"
#include "processor.h"

int print(char *s);
void setup_idt();
int arr[200];

extern int bss_start;
extern int bss_end;
extern int mem_start;
int timer_handler(int irq, void *data);


void bss_init()
{
	printk("%x %x \n", &bss_start, &bss_end);
	memset(&bss_start, 0, &bss_end - &bss_start);
} 

int my_handler(int irqno, void *data)
{
	printk("%s :received : %d\n", __func__, irqno);
	clear_soft_irq(1<<irqno);
}

void print_timer_values(int);

void parse_args(const char *args)
{
	printk("cmd line = %s\n", args);
}
void busy_loop1()
{
	unsigned int i, j, k;
	for (i = 0; i < (0xffffffff) ; i++)
		for (j = 0; j < (0xffffffff); j++)
			for (k = 0; k < (0xffffffff); k++);

}

void busy_loop2()
{
	unsigned int i, j, k;
//	for (i = 0; i < (0xffffffff) ; i++)
//		for (j = 0; j < (0xffffffff); j++)
			for (k = 0; k < (1<<26); k++);

}

int idle_thread()
{
	printk("In idle thread\n");
	int i=0;
	while (1) {
		sleep(5000);
		printk("Running idle thread, %d\n", i++);
	}
}
int normal_thread()
{
	printk("Normal thread\n");
	int i = 0;
	while (1) {
		sleep(1000);
		printk("Running normal thread, %d\n", i++);
	}
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

	//	generate_software_interrupt(2);
	
	//log_info();
	int j;
/*
	request_irq(4, IRQ_MODE, my_handler, 0); 
	for( i =1 ; i < 10; i++) {
		printk("hello %d \n", i);
		generate_software_interrupt(4);
	} 
*/
	create_idle_thread(idle_thread);
	create_thread(normal_thread);
	scheduler_init();
	__asm__ __volatile__("msr cpsr_c, #0x10");
//	log_info_str("In user space\n");
//	change_mode(USR_MODE);
	idle_thread();
	/* we shall never return here */
	/* end of world */
	
	int count = 0;
	 __asm__ __volatile__("swi #10"::);
	
	printk("into user land\n");
	
	while(1) {
		printk("sleep for 2 secs\n");
		sleep(2000);
	}
}

void msg()
{
	printk("calling msg\n");

}

