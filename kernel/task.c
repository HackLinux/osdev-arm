#include "print.h"
#include "task.h"
#include "malloc.h"
#include "funcs.h"

typedef struct context {
/*	int reg[16]; */
	int r0, r1, r2, r3;
	int r4, r5, r6, r7, r8;
	int r9, r10, r11, r12;
	int sp, lr, pc;
	int pid;
	unsigned char stack[STACK_SIZE];
	struct context *next;
	unsigned int in_use;
}pcontext;

static pcontext *pid_array[MAX_THREADS];

static pcontext *cur_pcb = 0;
static int num_threads = 0;

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
	pcb->sp = (long)pcb->stack + sizeof(pcb->stack)-1;
	pcb->pc = (long)thread_fn;
	pcb->lr = (long)exit_thread;
	mark_pid(pid, pcb);
	num_threads++;
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
	kfree(pcb);
	schedule();
}
