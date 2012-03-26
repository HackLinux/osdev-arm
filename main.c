int print(char *s);
void setup_idt();
int main()
{
	printk("hello world new\n");
	__asm__ __volatile__("swi 0x20");
	printk("Back buddy %d\n", 20);
}

void msg()
{
	printk("calling msg\n");

}

void screwed_up(int a)
{

	printk("screwed up man %x\n", a);
}
