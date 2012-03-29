#include "print.h"
#include "interrupt.h"

//static intr_handler vect_h[MAX_IRQ];
#define MAX_IRQ 32
struct IRQ_LIST  irq_list[MAX_IRQ];


void generate_software_interrupt(int irq)
{
	gen_soft_irq(1<<irq);	
}

int handle_generic_IRQ(int status, int irq_status, int soft_irq_status)
{
	int i;
	int interrupt;
	for (i = 0; i < MAX_IRQ; i++) {
		if (status & (1 << i)){
			irq_list[i].h(i, irq_list[i].data);
			interrupt = 1<<i;
			if (irq_status & interrupt) 
				clear_hard_irq(interrupt);
			else if (soft_irq_status & interrupt)
				clear_soft_irq(interrupt);
			else
				printk("U screweup, irq unknown \n");
		}
	}
			
	dprintk("in fiq handler ,received interrupt on line %d \n", status);
	log_info(__func__);
	return status;

}

int request_irq(int irq, int mode, intr_handler h, void *data)
{
	if (irq > MAX_IRQ) {
		printk("Invalid irq no = %d\n");
		return 0;
	}
	if (irq_list[irq].h){
		printk("Irq is in use\n");
		return 0;
	}

	irq_list[irq].h  	= h;	
	irq_list[irq].mode 	= mode;
	irq_list[irq].data	= data;
		
	switch (mode) {
		case IRQ_MODE:
			enable_irq(1<<irq);
		break;

		case FIQ_MODE:
			enable_fiq(1<<irq);
		break;
#if 0
		case VECTOR_MODE:
			if (!vect_h[irqno]){
				vect_h[irqno] = h;
				/*able_fiq(1<<irqno);*/
				ret = 1;
			}
				
		break;
#endif
		default:
			printk("Invalid mode\n");
		break;
	}

	return 1;
}

void free_irq(int irq)
{
	if (!irq_list[irq].h) {
		printk("Irq is not registered\n");
		return;
	}
	irq_list[irq].h  = 0;
	irq_list[irq].mode = 0;
	irq_list[irq].data = 0;
}
