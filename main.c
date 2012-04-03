#include "funcs.h"
#include "interrupt.h"

int print(char *s);
void setup_idt();
int arr[200];

extern int bss_start;
extern int bss_end;
int timer_handler(int irq, void *data);


void init_bss()
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
int main()
{
	printk("Entered main\n");

	printk("Cmd args : %s \n", (char*)0x100);
	init_bss();
	arch_init();
	timer_init();
	
//	generate_software_interrupt(2);
	
	log_info(__func__);
	int i, j;

/*	request_irq(4, IRQ_MODE, my_handler, 0); 
	for( i =1 ; i < 10; i++) {
		printk("hello %d \n", i);
		generate_software_interrupt(4);
	} */
	 __asm__ __volatile__("msr cpsr_c, #0x10"::);
	log_info(__func__);
	int count = 0;
	while(1) {
	}
}

void msg()
{
	printk("calling msg\n");

}

