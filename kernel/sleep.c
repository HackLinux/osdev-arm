#include <syscall_defs.h>
#include <timer.h>
#include <sched.h>

int wake_up_task(void *pcb)
{
    pcb = (pcontext *)pcb;
	set_task_state(pcb, READY);
	return insert_task(pcb);
}

int remove_from_rq(pcontext *pcb)
{
	set_task_state(pcb, SLEEPING);
	return remove_task(pcb, 0);	
}


int sys_sleep(long msecs)
{
	add_timer(msecs, wake_up_task, get_current());
	remove_from_rq(get_current());
	return 0;
}
