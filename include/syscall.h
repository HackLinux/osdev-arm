#ifndef __SYSCALL_H
#define __SYSCALL_H

void sys_exit();
void sys_open();
void sys_read();
void sys_write();
void sys_close();
void sys_sleep();
int sleep(int msecs,int id);
#endif
