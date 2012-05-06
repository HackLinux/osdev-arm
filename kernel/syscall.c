#include "print.h"
#include "support.h"
#include "syscall_table.h"
int a = 0x12345;

int console_write(const char *);


void syscall(int no)
{
	log_info_str(In  syscall: %d , no);
}
void __div0(void)
{
    console_write("Division by zero in kernel.\n");
}

