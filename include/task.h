#ifndef __TASK_H
#define __TASK_H
#ifndef __PROCESSOR_H
#define USER_STACK_SIZE 1<<12
#define SVC_STACK_SIZE 1<<12
#define MAX_THREADS 16
#define TASK_NAME_SIZE 16

typedef struct context {
	int r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, sp, lr;
	int spsr, pc;
	int pid;
	struct context *next, *prev;
	unsigned int in_use;
	char name[TASK_NAME_SIZE];
	unsigned char stack[USER_STACK_SIZE];
	unsigned char stack_top[0]; 
	unsigned char svc_stack[0];
}pcontext;


void exit_thread();
int create_thread(int (*thread_fn)(), const char *name, unsigned int mode);
int create_idle_thread(int (*thread_fn)(), const char *name, unsigned int mode);
pcontext *get_current();
int get_pid();
char *get_task_name();
int thread_count(void);
void set_current(pcontext *);
pcontext *get_pcb_with_pid(int pid);
#endif
#endif
