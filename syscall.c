#include "funcs.h"
int a = 0x12345;

void syscall(int no)
{
	log_info_str("In  syscall: %d \n", no);
}
void __div0(void)
{
    console_write("Division by zero in kernel.\n");
}

