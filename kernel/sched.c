#include "sched.h"
#include "task.h"
#include "print.h"
#include "processor.h"
#include "support.h"

/* 1. pick next task, curr and next task should not be same
 * 2. save current task context in the task struct
 *    2.1 save r0-r3 registers, r13, r14, spsr
 * 3. pick next task context from task struct
 *    3.1 restrore r0-r3, r13, r14, spsr 
 * 4. 
 */

//extern int context_switch_req;
extern int cur_pcb_ptr, nxt_pcb_ptr;
static int sched_needed;

extern void context_switch();
extern int get_cpsr();

int schedule_needed()
{
	return (sched_needed)? 1 : 0;
}

void unset_schedule_needed()
{	
	sched_needed = 0;
}

void set_schedule_needed()
{
	sched_needed = 1;
}
int get_process_cpsr()
{
	return (get_current()->spsr);
}
int get_process_mode()
{
	return (get_current()->spsr & 0x1f);

}
int get_cur_mode()
{
	return (get_cpsr() & 0x1f);

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

	unset_schedule_needed();
	cur_pcb_ptr = (int)get_current();
	nxt_pcb_ptr = (int)(get_current()->next);
	
	switch (get_cur_mode()) {

	case IRQ_MODE:
		save_process_context_irq();
		break;

	case SVC_MODE:
		save_process_context_svc();
		break;

	case SYS_MODE:
		save_process_context_sys();
		break;
	default:
		log_info_str("Invalid mode, got confused\n");
		break;
	} 
	
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
