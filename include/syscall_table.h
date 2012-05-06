#ifndef __SYSCALL_TABLE_H
#define __SYSCALL_TABLE_H

#define MAX_SYSCALLS 10

typedef int (*sys_func)(void); 
static sys_func syscall_table[]= {
	sys_exit,
	sys_open,
	sys_read,
	sys_write,
	sys_close,
	sys_sleep
};
