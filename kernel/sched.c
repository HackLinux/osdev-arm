#include <sched.h>
#include <task.h>
#include <print.h>
#include <processor.h>
#include <support.h>


pcontext *cur_pcb_ptr, *nxt_pcb_ptr;
static int sched_needed;

extern void context_switch();
extern int get_cpsr();


static pcontext *cur_pcb 	= 0;
static pcontext *head 		= 0;
static pcontext *tail 		= 0;
static int num_threads 		= 0;

int thread_count()
{
	return num_threads;
}

void inc_thread_count()
{
	num_threads++;
}

void dec_thread_count()
{
	num_threads++;
}

void set_task_list_head(pcontext *pcb)
{
	head = pcb;
}

void set_task_list_tail(pcontext *pcb)
{
	tail = pcb;
}

pcontext *get_task_list_tail()
{
	return tail;
}

pcontext *get_task_list_head()
{
	return head;
}



void set_current(pcontext *pcb)
{
	cur_pcb = pcb;
}

pcontext *get_current()
{
	return cur_pcb;
}

int get_pid()
{
	return get_current()->pid;
}

char *get_task_name()
{
	return get_current()->name;

}

int get_usr_stack()
{
	return (long)((get_current())->usr_stack_top);
}

int get_svc_stack()
{
	return (long)((get_current())->svc_stack_top);
}



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
	return (get_current()->cpsr);
}

int get_process_spsr()
{
	return (get_current()->spsr);
}

int get_process_mode_cpsr()
{
	return (get_current()->cpsr & 0x1f);
}

int get_process_mode_spsr()
{
	return (get_current()->spsr & 0x1f);
}

int get_cur_mode()
{
	return (get_cpsr() & 0x1f);

}

void update_rq_ptrs()
{
	cur_pcb_ptr = get_current();
	nxt_pcb_ptr = (get_current()->next);

}


int insert_task(pcontext *pcb)
{
	pcontext *t = get_task_list_tail();
	pcontext *h = get_task_list_head();
	pcb->next = t->next;
	pcb->prev = t;
	t->next = pcb;
	h->prev = pcb;
	set_task_list_tail(pcb);
	inc_thread_count();
}


int remove_task(pcontext *pcb, int free_pcb)
{
	pcontext *p, *n;
	p = pcb->prev;
	n = pcb->next;
	p->next = n;
	n->prev = p;
	pcb->next = pcb->prev = 0;
	if (get_task_list_tail() == pcb) 
		set_task_list_tail(p);

	dec_thread_count();
	set_current(p);
	update_rq_ptrs();
	if (free_pcb)
		kfree(pcb);

	switch_to_next_task();

}

void init_runq(pcontext *pcb)
{
	head = tail = pcb;
	head->next = head->prev = pcb; 
	inc_thread_count();
}

void schedule()
{
    static int csw = 0;
//	printk("scheduler called \n");
	/* 1. num of ready tasks == 1 , i.e idle thread running, so dont bother, return back
 	*  2. num of ready taks > 1, round robin, pick next task
 	*  3. 
 	*  	3.1 save current task on thats task's kernel stack
 	*  	3.2 restore new task state from that task's kernel stack
 	*  	context switch
 	*/
 	disable_interrupts();

	unset_schedule_needed();

	if (thread_count() == 1)
		return;

    log_info_str("***********Context switch ************ %s, %d\n", get_current()->name, csw++);
	update_rq_ptrs();
	
	switch (get_cur_mode()) {

	case IRQ_MODE:
		save_process_context_in_irq();
		break;

	case SVC_MODE:
		save_process_context_svc();
		break;

	case SYS_MODE:
		save_process_context_sys();
	    enable_interrupts();
		break;

	default:
		log_info_str("Invalid mode, got confused\n");
		panic();
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
