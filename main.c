#include "funcs.h"

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
int main()
{
	printk("Entered main\n");
	dump_cpsr(__func__);
	init_bss();
	disable_cpsr_fiq();
	disable_cpsr_irq();
	enable_cpsr_fiq();
	enable_cpsr_irq();
/*
	request_irq(irq
	enable_fiq(4);
	enable_irq(2);
*/
	gen_soft_irq(2);
	gen_soft_irq(4);
	dump_cpsr(__func__);
}

void msg()
{
	printk("calling msg\n");

}

