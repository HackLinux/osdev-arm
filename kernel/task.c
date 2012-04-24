#include "print.h"
#include "task.h"
#include "malloc.h"
#include "funcs.h"

static pcontext *pid_array[MAX_THREADS];

static pcontext *cur_pcb = 0;
static pcontext *head = 0;
static pcontext *tail = 0;
static int num_threads = 0;

pcontext *set_task_list_head(pcontext *pcb)
{
	head = pcb;
}

pcontext *set_task_list_tail(pcontext *pcb)
{
	tail = pcb;
}

pcontext *get_task_list_tail()
{
	return tail;
}

void set_current(pcontext *pcb)
{
	cur_pcb = pcb;
}

pcontext * get_current()
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

int mark_pid(int id, pcontext *pcb)
{
	pid_array[id] = pcb;
}

pcontext *get_pcb_with_pid(int id)
{
	if(id >= 0 && id < MAX_THREADS)
		return pid_array[id];
	return 0;
	
}

int create_thread(int (*thread_fn)())
{
	int pid;
	pcontext *pcb;
	/* copy all registers, 
 	 * and set the pc and lr to thread_fn
 	 * flags to a storage location */
	if (num_threads > MAX_THREADS)
		return 0;

	pcb = kmalloc(sizeof(pcontext));
	printk("After malloc\n");
	pid = get_free_pid();
	if (pid == -1) {
		return 0;
	}
	
	memset(pcb, 0, sizeof(*pcb));
	pcb->pid = pid;	
	pcb->pc = (long)thread_fn;
	pcb->lr = (long)exit_thread;
	/* disable interrupts */
	pcontext *t = get_task_list_tail();
	pcb->next = t->next;
	pcb->prev = t;
	t->next = pcb;
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
	pcontext *p, *c, *n;
	p = pcb->prev;
	n = pcb->next;
	p->next = n;
	n->prev = p;
	if (get_task_list_tail() == pcb) 
		set_task_list_tail(p);
	kfree(pcb);
	schedule();
}
