#include "funcs.h"
#include "interrupt.h"

int print(char *s);
void setup_idt();
int arr[200];

extern int bss_start;
extern int bss_end;


void init_bss()
{
	printk("%x %x \n", &bss_start, &bss_end);
	memset(&bss_start, 0, &bss_end - &bss_start);
} 

int my_handler(int irqno, void *data)
{
	printk("%s :received : %d\n", __func__, irqno);
}

int main()
{
	printk("Entered main\n");

	init_bss();
	arch_init();
	
	request_irq(2, FIQ_MODE, my_handler, 0); 
	generate_software_interrupt(2);
	
	log_info(__func__);
}

void msg()
{
	printk("calling msg\n");

}

