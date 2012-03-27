#include "funcs.h"

int print(char *s);
void setup_idt();
int arr[200];

extern int bss_start;
extern int bss_end;

void dump_cpsr()
{
	int a;
	__asm__ __volatile__ ("mrs %0, cpsr" : "=r"(a)::);
	printk("cpsr = %x \n", a);
}


void init_bss()
{
	printk("%x %x \n", &bss_start, &bss_end);
	memset(&bss_start, 0, &bss_end - &bss_start);

} 
int main()
{
//	init_bss();
	printk("hello world new\n");
	__asm__ __volatile__("swi 0x20");
	printk("Back buddy %d\n", 20);
	dump_cpsr();
	gen_soft_irq();
	printk("after gen_soft_irq\n");
	dump_cpsr();
}

void msg()
{
	printk("calling msg\n");

}

void screwed_up(int a)
{

	printk("screwed up man %x\n", a);
}
