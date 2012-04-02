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

	init_bss();
	arch_init();
	timer_init();
	
//	generate_software_interrupt(2);
	
	int i, j;

/*	request_irq(4, IRQ_MODE, my_handler, 0); 
	for( i =1 ; i < 10; i++) {
		printk("hello %d \n", i);
		generate_software_interrupt(4);
	} */
	extern void change_to_mode(int);
#define USER_MODE 0x10
#define SVC_MODE 0x13
#define SYS_MODE 0x1f
	log_info(__func__);
	change_to_mode(((~1>>5)<<5) | SYS_MODE);
	log_info(__func__);
	change_to_mode(((~1>>5)<<5) | SVC_MODE);
	log_info(__func__);
	while(1);
}

void msg()
{
	printk("calling msg\n");

}

