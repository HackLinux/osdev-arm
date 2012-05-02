#ifndef __PROCESSOR_H
#define STACK_SIZE 1<<12
#define MAX_THREADS 16

typedef struct context {
	int r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, sp, lr;
	int spsr, pc;
	int pid;
	struct context *next, *prev;
	unsigned int in_use;
	unsigned char stack[STACK_SIZE];
	unsigned char stack_top[0];
}pcontext;


void exit_thread();
int create_thread(int (*thread_fn)(), unsigned int mode);
int create_idle_thread(int (*thread_fn)(), unsigned int mode);
pcontext *get_current();
#endif
