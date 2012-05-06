#include "print.h"
#include "support.h"
#include "syscall_table.h"
int a = 0x12345;

int console_write(const char *);


void syscall(int no)
{
	log_info_str(In  syscall: %d , no);
}

void sys_exit()
{
}

void sys_open()
{
}

void sys_read()
{
}

void sys_write()
{
}

void sys_close()
{
}

void sys_sleep()
{
}
void __div0(void)
{
    console_write("Division by zero in kernel.\n");
}

