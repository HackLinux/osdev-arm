
int a = 0x12345;
void syscall(int no)
{
	printk("In  syscall: %d \n", no);
}
void __div0(void)
{
    console_write("Division by zero in kernel.\n");
}

