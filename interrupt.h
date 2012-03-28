#ifndef __INTERRUPT_H
#define __INTERRUPT_H

typedef int (*intr_handler)(int, void *);
int request_irq(int irq, int type, int (*intr_handler)(int irq, void *data));
#endif
