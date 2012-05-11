#include "funcs.h"
#include "print.h"
#include "support.h"

	 
static char buf[120];

char *get_cpsr_info()
{
	int a;
	char *ptr = buf;
	__asm__ __volatile__ ("mrs %0, cpsr" : "=r"(a)::);
	disable_con_write();
	ptr = printk("mode = %s , cpsr = %x ", print_mode(a & 0x0f), a);
	enable_con_write();
	strcpy(buf, ptr);
	return buf;
}
void asm_log_info()
{
	log_info();
}

void dump_value(int a)
{
	printk("value = %x \n", a);
}

void hang()
{
	log_info();
	while(1);
}
void panic(char *str)
{
	printk(str);
	hang();
}
