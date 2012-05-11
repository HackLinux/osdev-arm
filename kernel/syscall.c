#include "print.h"
#include "support.h"
int a = 0x12345;

int console_write(const char *);

#define MAX_SYSCALL 10
static long syscall_table[MAX_SYSCALL+1]= {0};

#if 0
#define install_syscall(int no, long func)
{
	if (no > MAX_SYSCALL) {
		printk("syscall beyound allowed range of : %d\n", MAX_SYSCALL);
		return 0;
	}
	syscall_table[no] = func;
}

#endif

void syscall(int no)
{
	log_info_str(In  syscall: %d , no);
}

void __div0(void)
{
    console_write("Division by zero in kernel.\n");
}

