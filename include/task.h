#ifndef __PROCESSOR_H
#define STACK_SIZE 1<<12
#define MAX_THREADS 16

void exit_thread();
int create_thread(int (*thread_fn)());
#endif
