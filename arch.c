#include "funcs.h"
extern unsigned char __idt_start, __idt_end;

volatile unsigned char *exception_table = 0x0;
extern unsigned int offset;

void arch_init()
{
	disable_cpsr_fiq();
	disable_cpsr_irq();
	enable_cpsr_fiq();
	enable_cpsr_irq();
	printk("%s : %x\n", __func__, &__idt_end -  &__idt_start);
	// may not work, due to elf symbol relocation
	memcpy(0x0, &__idt_start, &__idt_end - &__idt_start);
	offset =  (unsigned int)&__idt_start; 
	
}
