#include "sched.h"
void schedule()
{
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
}
