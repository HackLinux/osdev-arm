
int a = 0x12345;
void syscall()
{
	printk("In  syscall \n");
	printk(" stack addr = %x\n", &a);
	__asm__ __volatile__("mov %0, sp":"=r"(a)::);
	printk(" stack addr = %x\n", a);
}
void __div0(void)
{
    console_write("Division by zero in kernel.\n");
}

