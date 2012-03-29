#ifndef __INTERRUPT_H
#define __INTERRUPT_H

enum mode{
IRQ_MODE 	= 1,
FIQ_MODE 	= 2,
VECTOR_MODE 	= 3,
};

typedef int (*intr_handler)(int, void *);

struct IRQ_LIST {
	enum mode mode;
	intr_handler h;
	void *data;
};

int request_irq(int irq, int type, int (*intr_handler)(int irq, void *data), void *data);

#endif
