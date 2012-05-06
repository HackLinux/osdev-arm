#ifndef __SYSCALL_TABLE_H
#define __SYSCALL_TABLE_H
#include "syscall.h"

#define MAX_SYSCALLS 10

static long syscall_table[]= {
	(long)sys_exit,
	(long)sys_open,
	(long)sys_read,
	(long)sys_write,
	(long)sys_close,
	(long)sys_sleep
};
#endif
