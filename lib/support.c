#include "funcs.h"
#include "print.h"

char str_mode[][4] 	= {"", "abt", "fiq",  "irq", "svc", "sys", "und", "usr"}; 
#if 1
static char modes[4][4] 	= {
			{7, 0, 0, 0},
			{2, 0, 0, 0},
			{3, 0, 0, 0},
			{4, 1, 6, 5},
			};
#endif
#define get_mode(m) str_mode[modes[m&0x3][(m&0x6)>>2]]
static char buf[120];

char *get_cpsr_info()
{
	int a;
	char *ptr = buf;
	__asm__ __volatile__ ("mrs %0, cpsr" : "=r"(a)::);
	disable_con_write();
	ptr = printk("mode = %s , cpsr = %x ", get_mode(a & 0x0f), a);
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
