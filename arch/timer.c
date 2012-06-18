#include <print.h>
#include <interrupt.h>
#include <sched.h>
#include <timer.h>
#include <malloc.h>
/* SP 804 */

/*registers*/

#define  TIMER_BASE_0   0X101E2000
#define  TIMER_BASE_1   TIMER_BASE_0  + 0X20

#define  TIMER_BASE_2   0X101E3000 
#define  TIMER_BASE_3   TIMER_BASE_2  + 0X20

#define TIMER_LOAD 			0x00
#define TIMER_VALUE    		0x01
#define TIMER_CNTL			0X02
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

volatile static unsigned long  jiffies = 0;

unsigned long get_jiffies()
{
    return jiffies;
}

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
	return -1;
}
void print_timer_values(int count)
{
	volatile unsigned int *tbase = (volatile unsigned int *)get_timer_base(0);
	int i = 0;

	for (i = 0; i < count; i++) {
		printk("Timer value = %d \n", tbase[TIMER_VALUE]);
	}
}



/* we will maintain a linked list of structures, with decreasing timeout values, relative 
 * to previous entries(maintain in jiffies)
 * 10    - 25    - 35     - 50
 * ____    ___     ___      ___
 *| 10 |->|15 |-> |10 | -> |15 |
 *|____|  |___|   |___|	   |___| 
 */

static timer_list *h_tm = 0;
 /* jiffies -> number of clock ticks 
 */ 
#define INTRS_PER_SEC 10
#define JIFFIES_IN_SEC INTRS_PER_SEC
#define MSEC_PER_JIFFY (1000/JIFFIES_IN_SEC)


/*HZ is number of times scheduler called a sec*/
#define HZ 5
#define HZ_TO_JIFFIES (JIFFIES_IN_SEC/HZ) 

#if HZ > JIFFIES_IN_SEC
#error "HZ is greater than JIFFIES_PER_SEC"
#endif

static long msecs_to_jiffies(long msecs)
{
	return (msecs + (MSEC_PER_JIFFY - 1))/(MSEC_PER_JIFFY);
}


int insert_timer(timer_list *c_tm)
{	
	long msec_j = c_tm->jiffies;

	timer_list *t, *pt;

    /*  h_tm : Pointer to the head of the timer list */
	if (h_tm == 0) {
		h_tm = c_tm;
		return 0;
	}
	pt = h_tm;
	for (t = h_tm; t != 0; t = t->next) {

			if (msec_j < t->jiffies) {

				if (t == h_tm) {
					c_tm->next = h_tm;
					h_tm = c_tm;	
				}
				else {
					pt->next = c_tm;
					c_tm->next = t;	
				}

				c_tm->jiffies = msec_j;
				
				for (t = c_tm->next; t != 0; t = t->next)
					t->jiffies -= c_tm->jiffies;
				
				return 0;
			}
			msec_j -= t->jiffies;
			pt = t;
	}
	pt->next = c_tm;
	return 0;	
}

int add_timer(long msecs, void (*call_back)(void *data), void *data)
{
	timer_list *tm = kmalloc(sizeof(timer_list));

	tm->jiffies = msecs_to_jiffies(msecs);
	tm->func = call_back;
	tm->data = data;
	tm->next = 0;
	insert_timer(tm);
	return 0;
}

int update_timer_list()
{
	if (h_tm == 0)
		return 0;

	h_tm->jiffies--;
	while (h_tm && h_tm->jiffies == 0) {
		h_tm->func(h_tm->data);	
		kfree(h_tm);
		h_tm = h_tm->next;
	}
	return 0;
}



void call_handlers()
{
	update_timer_list();
}

int timer_handler(int irq, void *data)
{
	volatile unsigned int *tbase = (volatile unsigned int *)get_timer_base(0);
	tbase[TIMER_INTCLR] = 1;
	jiffies++;
	call_handlers();
	if (jiffies % (HZ_TO_JIFFIES) == 0)
    	set_schedule_needed();	
	return 0;
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
	/* set the timer for 10 ms */
	/* 1000000 = 1 sec, 1 HZ */
	timer_reload = (1 * 1000000)/(1 * 256 * INTRS_PER_SEC) ;//1 sec
	timer_bg_reload = timer_reload;
	tbase[TIMER_LOAD] = timer_reload;
	tbase[TIMER_BGLOAD] = timer_bg_reload;

	request_irq(4, IRQ_MODE, timer_handler, 0); 
	tbase[TIMER_CNTL] |= TIMER_ENABLE|TIMER_INT_EN;
	printk("Timer value = %d, irstatus = %d, timer_ctrl = %x \n", tbase[TIMER_VALUE], tbase[TIMER_INTR_STATUS], tbase[TIMER_CNTL]);

	printk("Timer initialization done \n");
	
}
