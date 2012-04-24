#include "sched.h"
#include "task.h"

/* 1. pick next task, curr and next task should not be same
 * 2. save current task context in the task struct
 *    2.1 save r0-r3 registers, r13, r14, spsr
 * 3. pick next task context from task struct
 *    3.1 restrore r0-r3, r13, r14, spsr 
 * 4. 
 */

void schedule()
{
	pcontext *curr, *next;
	printk("scheduler called \n");
	/* 1. num of ready tasks == 1 , i.e idle thread running, so dont bother, return back
 	*  2. num of ready taks > 1, round robin, pick next task
 	*  3. 
 	*  	3.1 save current task on thats task's kernel stack
 	*  	3.2 restore new task state from that task's kernel stack
 	*  	context switch
	* will update  flag context_switch_req and sets cur_pcb_ptr, nxt_pcb_ptr  and will return
 	*/

}

void scheduler_init()
{
	set_current(get_pcb_with_pid(0));
	set_task_list_head(get_current());
	set_task_list_tail(get_current());
}
