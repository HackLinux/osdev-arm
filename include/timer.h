/*
 * =====================================================================================
 *
 *       Filename:  timer.h
 *
 *    Description:  timer handler declarations
 *
 *        Version:  1.0
 *        Created:  06/10/2012 12:14:48 AM
 *       Revision:  none
 *       Compiler:  arm
 *
 *         Author:  madan (), madanpandiri@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __TIMER_H
#define __TIMER_H

typedef struct timer_list {
		long jiffies;
		int (*func)(void *);
		void *data;
		struct timer_list *next;
}timer_list;
unsigned long get_jiffies(void);
int add_timer(long msecs, int (*call_back)(void *data), void *data);

#endif
