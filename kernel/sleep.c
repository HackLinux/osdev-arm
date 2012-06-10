#include <syscall_defs.h>
#include <task.h>

int wake_up_task(pcontext *pcb)
{
	set_task_state(pcb, READY);
	insert_task(pcb);
}

int remove_from_rq(pcontext *pcb)
{
	set_task_state(pcb, SLEEPING);
	remove_task(pcb, 0);	
}


int sys_sleep(long msecs)
{
    schedule();
	return 1;
	add_timer(msecs, wake_up_task, get_current());
	remove_from_rq(get_current());
	return 1;
}
