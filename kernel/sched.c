#include "sched.h"
#include "task.h"

/* 1. pick next task, curr and next task should not be same
 * 2. save current task context in the task struct
 *    2.1 save r0-r3 registers, r13, r14, spsr
 * 3. pick next task context from task struct
 *    3.1 restrore r0-r3, r13, r14, spsr 
 * 4. 
 */

//extern int context_switch_req;
extern int cur_pcb_ptr, nxt_pcb_ptr;
int context_switch_req;


/* If prev mode is swi and its sp not equal to swi top then not need to disable schedule*/
int disable_schedule()
{
	return 0;
}


void schedule()
{
//	printk("scheduler called \n");
	/* 1. num of ready tasks == 1 , i.e idle thread running, so dont bother, return back
 	*  2. num of ready taks > 1, round robin, pick next task
 	*  3. 
 	*  	3.1 save current task on thats task's kernel stack
 	*  	3.2 restore new task state from that task's kernel stack
 	*  	context switch
	* will update  flag context_switch_req and sets cur_pcb_ptr, nxt_pcb_ptr  and will return
 	*/
	if (thread_count() == 1)
		return;

	if (disable_schedule())
		return;

	cur_pcb_ptr = (int)get_current();
	nxt_pcb_ptr = (int)(get_current()->next);
	context_switch_req = 1;
	
}

void move_cur_pcb()
{
	cur_pcb_ptr = nxt_pcb_ptr;
	set_current((pcontext *)cur_pcb_ptr);
}

void scheduler_init()
{
	printk("in scheduler_init\n");
	set_current(get_pcb_with_pid(0));
/*	set_task_list_head(get_current());
	set_task_list_tail(get_current());*/
}
