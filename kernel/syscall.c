#include <print.h>
#include <support.h>
#include <syscall_tbl.h>
int a = 0x12345;

int console_write(const char *);


long get_syscall_base()
{
	return (long)syscall_table;
}

void __div0(void)
{
    console_write("Division by zero in kernel.\n");
}

