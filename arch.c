

void arch_init()
{
	disable_cpsr_fiq();
	disable_cpsr_irq();
	enable_cpsr_fiq();
	enable_cpsr_irq();
}
