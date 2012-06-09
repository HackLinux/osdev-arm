#ifndef __SCHED_H
#define __SCHED_H
#include <task.h> 
int thread_count();
void inc_thread_count();
void set_task_list_head(pcontext *pcb);
void set_task_list_tail(pcontext *pcb);
pcontext *get_task_list_tail();
pcontext *get_task_list_head();
void init_runq(pcontext *pcb);
void set_current(pcontext *pcb);
pcontext *get_current();
int get_pid();
char *get_task_name();
int get_usr_stack();
int get_svc_stack();
int schedule_needed();
void unset_schedule_needed();
void set_schedule_needed();
int get_process_cpsr();
int get_process_spsr();
int get_process_mode_cpsr();
int get_process_mode_spsr();
int get_cur_mode();
void update_rq_ptrs();
void schedule(void);
void scheduler_init(void);
#endif
