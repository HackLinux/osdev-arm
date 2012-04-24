#ifndef __PROCESSOR_H
#define STACK_SIZE 1<<12
#define MAX_THREADS 16
typedef struct context {
/*	int reg[16]; */
	int r0, r1, r2, r3;
	int r4, r5, r6, r7, r8;
	int r9, r10, r11, r12;
	int sp, lr, pc;
	int pid;
	unsigned char stack[STACK_SIZE];
	struct context *prev, *next;
	unsigned int in_use;
}pcontext;

typedef struct context {
	int r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, lr;
	int spsr, pc;
	int pid;
	struct context *next;
	unsigned int in_use;
}pcontext;


void exit_thread();
int create_thread(int (*thread_fn)());
#endif
