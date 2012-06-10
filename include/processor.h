#ifndef __PROCESSOR_H
#define __PROCESSOR_H

#define USR_MODE  0x10
#define SVC_MODE  0x13
#define IRQ_MODE  0x12
#define	FIQ_MODE  0x11
#define	SYS_MODE  0x1f
#define	ABT_MODE  0x17
#define UND_MODE  0x1b
#define MODE_MASK 0X1f

#define USR_MODE_IRQS_OFF 0Xd0
#define SVC_MODE_IRQS_OFF 0xd3
#define IRQ_MODE_IRQS_OFF 0xd2
#define	FIQ_MODE_IRQS_OFF 0xd1
#define	SYS_MODE_IRQS_OFF 0xdf
#define	ABT_MODE_IRQS_OFF 0xd7
#define UND_MODE_IRQS_OFF 0xdb
 
static inline void change_mode(int mode)
{
	__asm__ __volatile__("msr cpsr_c, %0"::"r"(mode):);
}

typedef unsigned int u32;
typedef int size_t;
#define NULL  (void *)0
#endif

