#include "syscall_defs.h"

int sys_sleep(long msecs)
{
	schedule();
	return 1;
}
