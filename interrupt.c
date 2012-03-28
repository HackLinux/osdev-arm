#include "print.h"
#include "interrupt.h"

//static intr_handler vect_h[MAX_IRQ];
#define MAX_IRQ 32
struct IRQ_LIST  irq_list[MAX_IRQ];



int handle_irq(int status)
{
	printk("in irq handler ,received interrupt on line %d \n", status);
	dump_cpsr(__func__);
	return status;
}
int handle_fiq(int status)
{
	printk("in fiq handler ,received interrupt on line %d \n", status);
	dump_cpsr(__func__);
	return status;

}

int request_irq(int irqno, int mode, intr_handler h)
{
	if (irqno > MAX_IRQ) {
		printk("Invalid irq no = %d\n");
		return 0;
	}
	if (irq_list[irqno].no != 0){
		printk("Irq is in use\n");
		return 0;
	}

	irq_list[irqno].no = 1;
	irq_list[irqno].h  = h;	
	irq_list[irqno].mode = mode;
		
	switch (mode) {
		case IRQ_MODE:
			enable_irq(1<<irqno);
		break;

		case FIQ_MODE:
			enable_fiq(1<<irqno);
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

void free_irq(int irqno)
{
	if (irq_list[irqno].no != 1) {
		printk("Irq is not registered\n");
		return ;
	}
	irq_list[irqno].no = 0;
	irq_list[irqno].h  = 0;
	irq_list[irqno].mode = 0;
}
