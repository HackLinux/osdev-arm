#ifndef __HW_DEFS_H
#define __HW_DEFS_H
#define STACK_SIZE 0x1000
#define BASE_STACK 0x20000
#define ABRT_STACK (BASE_STACK+STACK_SIZE)
#define FIQ_STACK  (ABRT_STACK+STACK_SIZE)
#define IRQ_STACK  (FIQ_STACK+STACK_SIZE)
#define SVC_STACK  (IRQ_STACK+STACK_SIZE)
#define SYS_STACK  (SVC_STACK+STACK_SIZE)
#define UND_STACK  (SYS_STACK+STACK_SIZE)
#define USR_STACK  (UND_STACK+STACK_SIZE)
#define ABRT_MD	  0x17
#define FIQ_MD	  0x11
#define IRQ_MD	  0x12
#define SVC_MD	  0x13 
#define SYS_MD	  0x1f
#define UND_MD	  0x1b
#define USR_MD	  0x10

#define IRQ_MDMASK   0xd2
#define FIRQ_MDMASK  0xd1

#endif
