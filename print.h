#ifndef __PRINT_H
#define __PRINT_H

char * printk(const char *fmt, ...);

#ifdef DEBUG 
#define dprintk(fmt, ...) \
	printk(fmt, ...)
#else
#define dprintk(fmt, ...) 
#endif

#endif
