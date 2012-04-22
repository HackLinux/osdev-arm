#include "sched.h"
#include "task.h"

/* 1. pick next task, curr and next task should not be same
 * 2. save current task context in the task struct
 *    2.1 save r0-r3 registers, r13, r14, spsr
 * 3. pick next task context from task struct
 *    3.1 restrore r0-r3, r13, r14, spsr 
 * 4. 
 */

void context_switch(pcontext *curr, pcontext *next)
{
}

void schedule()
{
	pcontext *curr, *next;
	printk("scheduler called \n");
	context_switch(curr, next);
}

void scheduler_init()
{
	set_current(get_pcb_with_pid(0));
	set_task_list_head(get_current());
	set_task_list_tail(get_current());
}
