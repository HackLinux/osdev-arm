#include "print.h"
#include "interrupt.h"
/* SP 804 */

/*registers*/

#define  TIMER_BASE_0   0X101E2000
#define  TIMER_BASE_1   TIMER_BASE_0  + 0X20

#define  TIMER_BASE_2   0X101E3000 
#define  TIMER_BASE_3   TIMER_BASE_2  + 0X20

#define TIMER_LOAD 		0x00
#define TIMER_VALUE    		0x01
#define TIMER_CNTL		0X02
#define TIMER_INTCLR		0x03
#define TIMER_INTR_STATUS	0X04
#define TIMER_MASK_INTR_STATUS  0X05
#define TIMER_BGLOAD		0X06

#define TIMER_LOAD_VAL 0xffffffff

#define TIMER_ENABLE    	(1 << 7)
#define TIMER_MODE_PD   	(1 << 6)
#define TIMER_INT_EN    	(1 << 5)
#define TIMER_CTRL_DIV1    	(0<<2)
#define TIMER_CTRL_DIV16   	(1<<2)
#define TIMER_CTRL_DIV256  	(2<<2)
#define TIMER_SIZE_32  		(1 << 1)
#define TIMER_ONE_SHOT  	(1 << 0)
inline unsigned int get_timer_base(int index)
{
	switch (index) {
	case 0:
		return TIMER_BASE_0;
	case 1:
		return TIMER_BASE_1;
	case 2:
		return TIMER_BASE_2;
	case 3:
		return TIMER_BASE_3;
	}
}
void print_timer_values(int count)
{
	volatile unsigned int *tbase = (volatile unsigned int *)get_timer_base(0);
	int i = 0;

	for (i = 0; i < count; i++) {
		printk("Timer value = %d \n", tbase[TIMER_VALUE]);
	}
}



int timer_handler(int irq, void *data)
{
	volatile unsigned int *tbase = (volatile unsigned int *)get_timer_base(0);
	tbase[TIMER_INTCLR] = 1;

}


void timer_init()
{
	volatile unsigned int timer_reload, timer_bg_reload;
	volatile unsigned int *tbase = (volatile unsigned int *)get_timer_base(0);

	/*
	volatile unsigned int sys_base = *(volatile unsigned int *)(0X101E0000);
	sys_base |= 0x2a8000;
	*(volatile unsigned int *)(0x101E0000) = sys_base;
	tbase[TIMER_CNTL]  &= ~TIMER_ENABLE;
	*/
	/* set timer for periodic, timermode to 1, oneshot to 0, */
	/* periodic, 
 	* interrupt enable
 	* one shot disable
 	* 32 timer
 	* 16 divider
 	*/
	tbase[TIMER_CNTL]  &= ~(TIMER_ONE_SHOT); /* periodic mode */
	tbase[TIMER_CNTL] |= TIMER_SIZE_32|TIMER_CTRL_DIV256|TIMER_MODE_PD; /* 32 bit, divide by 16 */
/*	
 *	timerxload = (Interval x TIMCLKFREQ)/ (TIMCLKENXDIV x PRESCALEDIV)
 *	TIMCLKENXDIV = 1, PRESCALEDIV = 256, Interval = 1 sec, TIMCLKFREQ = 1MHZ 
 */
	timer_reload =  (10 * 1000000)/(1 * 256);
	timer_bg_reload = timer_reload;
	tbase[TIMER_LOAD] = timer_reload;
	tbase[TIMER_BGLOAD] = timer_bg_reload;

	request_irq(4, IRQ_MODE, timer_handler, 0); 
/*	request_irq(5, IRQ_MODE, timer_handler, 0); 
	request_irq(6, IRQ_MODE, timer_handler, 0); 
	request_irq(7, IRQ_MODE, timer_handler, 0); */
	tbase[TIMER_CNTL] |= TIMER_ENABLE|TIMER_INT_EN;
	printk("Timer value = %d, irstatus = %d, timer_ctrl = %x \n", tbase[TIMER_VALUE], tbase[TIMER_INTR_STATUS], tbase[TIMER_CNTL]);

	printk("Timer initialization done \n");
	
}
