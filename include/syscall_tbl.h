#ifndef __SYSCALL_TBL_H
#define __SYSCALL_TBL_H
#include <syscall_defs.h>

#define MAX_SYSCALL 3
static long syscall_table[MAX_SYSCALL+1]= {
	(long)sys_exit,
	(long)sys_sleep,
	(long)sys_halt,
};


#endif
