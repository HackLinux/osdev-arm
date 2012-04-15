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
char pid_array[1024];
static int num_threads = 0;
static pcontext *cur_pcb = 0;

void set_current_pcb(pcontext *pcb)
{
	cur_pcb = pcb;
}

pcontext * get_curr_pcb()
{
	return cur_pcb;
}

int create_thread(int (*thread_fn)())
{
	/* copy all registers, 
 	 * and set the pc and lr to thread_fn
 	 * flags to a storage location */
	if (num_threads > MAX_THREADS)
		return 0;
	pcontext *pcb;
	pcb = malloc(sizeof(pcontext));
	memset(pcb, 0, sizeof(*pcb));
	pcb->sp = (long)pcb->stack + sizeof(pcb->stack)-1;
	pcb->pc = (long)thread_fn;
	pcb->lr = (long)exit_thread;
	num_threads++;
	return 1;
}

void exit_thread()
{
	/* cleanup thread, and schedule other threads, init shud never exit */
	/* load context and check pid */
	num_threads--;

}
