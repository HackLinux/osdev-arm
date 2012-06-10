#include "funcs.h"
#include "print.h"
#include "support.h"
#include "task.h"

	 
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
void dump_regs()
{
	pcontext *p = get_current();
	printk("----------------------\n");
	printk("pid = %d, name = %s\n", p->pid, p->name);
	printk("r0 = %x,  r1 = %x,  r2=%x, r3=%x, r4=%x\n", p->r0, p->r1, p->r2, p->r3, p->r4);
	printk("r5 = %x,  r5 = %x,  r6=%x, r7=%x, r8=%x\n", p->r5, p->r6, p->r7, p->r8, p->r9);
	printk("r9 = %x,  r10= %x, r11=%x, r12=%x, r13=%x\n", p->r9, p->r10, p->r11, p->r12, p->sp);
	printk("r14= %x,  pc = %x, cpsr=%x, spsr=%x \n", p->lr, p->pc, p->cpsr, p->spsr);
	printk("----------------------\n");

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
	printk("Panic ---> %s : %s\n", get_current()->name, str);
	halt();
}
void hard_loop()
{

    int i = 0;
    for (i = 0; i < 1<<22; i++);
}
