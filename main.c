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
	init_bss();
	enable_fiq();
	gen_soft_irq();
	dump_cpsr(__func__);
}

void msg()
{
	printk("calling msg\n");

}

