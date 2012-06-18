#ifndef __PRINT_H
#define __PRINT_H

char * printk(const char *fmt, ...);
char * snprintk(const char *fmt, ...);

void enable_con_write();
void disable_con_write();
int print_rbuf();
#ifdef DEBUG 
#define dprintk printk
#else
#define dprintk 
#endif

#endif
