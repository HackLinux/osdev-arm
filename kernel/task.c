#include <print.h>
#include <sched.h>
#include <task.h>
#include <malloc.h>
#include <funcs.h>
#include <support.h>
#include <processor.h>

static pcontext *pid_array[MAX_THREADS];

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



void abyss()
{
	while (1) {
		printk("In abyss moron = %s \n", get_current()->name);
	    panic("Check the code idiot\n"); 
	}	
	

}

pcontext *common_thread_create(int pid, int (*thread_fn)(), const char *name, unsigned int mode)
{
	pcontext *pcb;
	unsigned int pcb_sz;
	
	if (thread_count() > MAX_THREADS)
		return 0;

	pcb = kmalloc(sizeof(*pcb));
	memset(pcb, 0, sizeof(*pcb));
	pcb->pid = pid;	
	pcb->pc = (long)thread_fn;
	pcb->lr = (long)abyss;
	pcb->sp = (long)pcb->usr_stack_top;
    pcb->cpsr = mode;
    if ((mode & MODE_MASK) == SVC_MODE) {
        pcb->lr = pcb->pc;
//        pcb->cpsr = mode|0x80;
    }
            
    strncpy(pcb->name, name, TASK_NAME_SIZE);
	return pcb;	
}
int create_idle_thread(int (*thread_fn)(), const char *name, unsigned int mode)
{
	pcontext *pcb =	common_thread_create(0, thread_fn, name, mode);
	init_runq(pcb);
	mark_pid(0, pcb);
	return 1;
}

int create_thread(int (*thread_fn)(), const char *name, unsigned mode)
{
	int pid = get_free_pid();
	
	if (pid == -1)
		return 0;	
	
	printk("in create thread %s\n",name);
	/* disable interrupts */
	pcontext *pcb =	common_thread_create(pid, thread_fn, name, mode);
	insert_task(pcb);

	/* enable interrupts */
	mark_pid(pid, pcb);
	return 1;
}

void sys_exit(int status)
{
	pcontext *pcb = get_current();
    printk("Process: %d:%s exiting\n", pcb->pid, pcb->name);
	if (pcb->pid == 0) {
		panic("Killing init thread \n");
	}
	mark_pid(pcb->pid, 0);
	remove_task(pcb, 1);
}
void sys_halt()
{
    while(1);
}

