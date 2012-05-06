#include "print.h"
#include "sched.h"
#include "task.h"
#include "malloc.h"
#include "funcs.h"

static pcontext *pid_array[MAX_THREADS];

static pcontext *cur_pcb = 0;
static pcontext *head = 0;
static pcontext *tail = 0;
static int num_threads = 0;

inline int thread_count()
{
	return num_threads;
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

int get_free_pid()
{
	int i;
	for (i = 1; i < MAX_THREADS; i++)
		if (!pid_array[i])
			return i;
	return -1;

}

void mark_pid(int id, pcontext *pcb)
{
	pid_array[id] = pcb;
}

pcontext *get_pcb_with_pid(int id)
{
	if(id >= 0 && id < MAX_THREADS)
		return pid_array[id];
	return 0;
}

int get_pid()
{
	return get_current()->pid;

}
char *get_task_name()
{
	return get_current()->name;

}

pcontext *common_thread_create(int pid, int (*thread_fn)(), const char *name, unsigned int mode)
{
	pcontext *pcb;
	/* copy all registers, 
 	 * and set the pc and lr to thread_fn
 	 * flags to a storage location */
	if (num_threads > MAX_THREADS)
		return 0;

	pcb = kmalloc(sizeof(pcontext));
	memset(pcb, 0, sizeof(*pcb));
	pcb->pid = pid;	
	pcb->pc = (long)thread_fn;
	pcb->lr = (long)exit_thread;
	pcb->sp = (long)pcb->stack_top;
	pcb->spsr = mode;
	strncpy(pcb->name, name, TASK_NAME_SIZE);
//	pcb->spsr = get_cpsr();
	return pcb;	

}
int create_idle_thread(int (*thread_fn)(), const char *name, unsigned int mode)
{
	pcontext *pcb =	common_thread_create(0, thread_fn, name, mode);
	mark_pid(0, pcb);
	head = tail = pcb;
	head->next = head->prev = head;
	num_threads++;
	return 1;
}

int create_thread(int (*thread_fn)(), const char *name, unsigned mode)
{
	int pid = get_free_pid();
	if (pid == -1)
		return 0;	
	/* disable interrupts */
	pcontext *pcb =	common_thread_create(pid, thread_fn, name, mode);
	pcontext *t = get_task_list_tail();
	pcontext *h = get_task_list_head();
	pcb->next = t->next;
	pcb->prev = t;
	t->next = pcb;
	h->prev = pcb;
	
	set_task_list_tail(pcb);
	num_threads++;
	/* enable interrupts */
	mark_pid(pid, pcb);
	return 1;
}

void exit_thread()
{
	/* cleanup thread, and schedule other threads, init shud never exit */
	/* load context and check pid */
	pcontext *pcb = get_current();
	if (pcb->pid == 0) {
		panic("Killing init thread \n");
	}
	mark_pid(pcb->pid, 0);
	num_threads--;
	pcontext *p, *n;
	p = pcb->prev;
	n = pcb->next;
	p->next = n;
	n->prev = p;
	if (get_task_list_tail() == pcb) 
		set_task_list_tail(p);
	kfree(pcb);
	schedule();
}
