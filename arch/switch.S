#   1. two arguments, containing task1 , task2 pcb 
#   2. Save task1 state to task1 pcb
#   3. restrore task2 state from pcb 
#   4. switch to task2
#

#include "hw_defs.h"

.global save_process_context_irq, save_process_context_svc

/* according to abi , r0-r4 are used as arguments and temporaries, so .... we can use them */
/* we are being called from svc context, so we shud get back to the next instruction after */

restore_process_regs:
	bl move_cur_pcb
	bl get_process_mode	
	cmp r0, #SVC_MD
	beq  restore_process_regs_svc
		
/* when in irq mode, 
	the return state is definitly usr/sys, 
	so set the svc mode stack here by moving to svc mode. 
	It helps in going from usr->svc then schedule etc.   */
/* fall thru code */

restore_process_regs_irq:
	msr cpsr_c, #IRQ_MD	/* with mode change regs/stacks will change */
	

	/* set the svc stack */	
	mov lr, pc
	ldr pc, =get_svc_stack
	msr cpsr_c, #SVC_MD
	mov sp, r0
	msr cpsr_c, #IRQ_MD
	/* end of svc stack */

	ldr sp, =nxt_pcb_ptr
	ldr sp, [sp]
	add sp, sp, #60

	ldm sp, {r0, lr}
	msr spsr_cxsf, r0
	
	sub sp, sp, #60
	ldmia sp, {r0-r14}^
	ldr sp, =IRQ_STACK
	movs pc, lr

restore_process_regs_svc:
	bl get_process_cpsr	
	msr cpsr, r0
	ldr r13, =nxt_pcb_ptr
	ldr sp, [r13]
	ldmia sp!, {r0-r12}
	ldmia sp!, {r0,r14}
	mov sp, r0
	movs pc, lr

save_process_context_sys:
	mov pc, lr

save_process_context_svc:
	mov r0, sp
	ldr r13, =cur_pcb_ptr
	ldr sp, [r13]
	stmia sp!, {r0-r12}
	stmia sp!, {r0,r14}
	mrs r0, lr
	mrs r1, cpsr
	mov r2, spsr
	stmia sp!, {r0-r3}
		
	ldr sp, =SVC_STACK
	
	bl move_cur_pcb
	bl get_process_mode	
	
	cmp r0, #USR_MD
	beq restore_process_usr_regs
	cmp r0, #SYS_MD
	beq restore_process_sys_regs

restore_process_svc_regs:
		
	




/* with mode change regs/stacks will change */

restore_process_usr_regs:	/* lets make a decision that we will move to IRQ mode when restoring user/sys regs */
restore_process_sys_regs:
	msr cpsr_c, #IRQ_MD	
	
	/* set the svc stack */	
	mov lr, pc
	ldr pc, =get_svc_stack
	msr cpsr_c, #SVC_MD
	mov sp, r0
	msr cpsr_c, #IRQ_MD
	/* end of svc stack */

	ldr sp, =nxt_pcb_ptr
	ldr sp, [sp]
	add sp, sp, #60

	ldm sp, {r0, r1}
	msr spsr_cxsf, r1
	mov lr, r0
	
	sub sp, sp, #60
	ldmia sp, {r0-r14}^
	ldr sp, =IRQ_STACK
	movs pc, lr


		



save_process_context_irq:
	ldr sp, =(IRQ_STACK - 56)
	ldmfd sp!, {r0-r12, lr}
	ldr r13, =cur_pcb_ptr	
	ldr sp, [r13]
	stmia sp, {r0-r14}^  /* this will use the user registers, ie, r0-r12 common registers and r13_user n r14_user
			      after this statement, r13 and r14 will reflect the current mode registers
			      if we use pc and the last argument to ^, then spsr is copied to cpsr otherwise
			      the registers of the saved spsr and temporarily masked for the duration of operation*/	
	
	add sp, sp, #68	
	mrs  r0, spsr	      /* get the spsr register of current task */
	stmdb sp, {r0, lr}   /* copy the spsr and LR i.e the return address of task, no need of ^	*/

	/* current context is saved */
	/* now restore the next task context */

	ldr sp, =IRQ_STACK
			
	bl move_cur_pcb

	bl get_process_mode	
	cmp r0, #SVC_MD
	bne set_svc_stack
	
restore_svc_regs:


set_svc_stack:
	/*set the svc stack*/	
	mov lr, pc
	ldr pc, =get_svc_stack
	msr cpsr_c, #SVC_MD
	mov sp, r0
	msr cpsr_c, #IRQ_MD
	/* end of svc stack */

	ldr sp, =nxt_pcb_ptr
	ldr sp, [sp]
	add sp, sp, #60

	ldm sp, {r0, lr}
	msr spsr_cxsf, r0
	
	sub sp, sp, #60
	ldmia sp, {r0-r14}^
	ldr sp, =IRQ_STACK
	movs pc, lr


	b restore_process_regs
		
	

.global cur_pcb_ptr, nxt_pcb_ptr

.data
cur_pcb_ptr:
	.word 0x00000000
nxt_pcb_ptr:
	.word 0x00000000
