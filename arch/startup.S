.global _reset, change_to_mode
_reset:
	bl setup_stacks
	ldr r0, =panic
	bl asm_log_info
	bl main
	b .

#include "hw_defs.h"
/*Setup stack addresses*/

setup_stacks:
	ldr sp, =SVC_STACK		/*r13_svc (Supervisor mode)*/

	msr cpsr_c, #IRQ_MDMASK		/*switch to irq mode*/
	ldr sp, =IRQ_STACK		/*r13_irq */
	
	msr cpsr_c, #FIRQ_MDMASK	/*switch to fast irq mode*/
	ldr sp, =FIQ_STACK		/*r13_fiq*/	

	
	msr cpsr_c, #ABRT_MD		/*switch to abort mode*/
	ldr sp, =ABRT_STACK		/*r13_abrt*/

	msr cpsr_c, #UND_MD 		/*switch to undefined mode*/
	ldr sp, =UND_STACK		/*r13_und*/
	
	msr cpsr_c, #SYS_MD		/*switch to system mode*/
	ldr sp, =SYS_STACK		/*r13_sys*/
	
	msr cpsr_c, #SVC_MD 
	mov pc, lr


dump:
	ldr r0, =panic
	bl asm_log_info

change_to_mode:
	mov r2, lr
	mrs r1, cpsr
	orr r1, r0
	msr cpsr_c, r1
	mov pc, r2
	
panic:
	.string "panic"
