#include "funcs.h"

int print(char *s);
char str_mode[][4] 	= {"", "abt", "fiq",  "irq", "svc", "sys", "und", "usr"}; 
static int modes[4][4] 	= {
			{7, 0, 0, 0},
			{2, 0, 0, 0},
			{3, 0, 0, 0},
			{4, 1, 6, 5},
			};

#define get_mode(m) str_mode[modes[m&0x3][(m&0x6)>>2]]
void dump_cpsr(char *str)
{
	int a;
	__asm__ __volatile__ ("mrs %0, cpsr" : "=r"(a)::);
	printk("mode = %s , func = %s, cpsr = %x \n", get_mode(a & 0x0f), str, a);
}
