#   1. two arguments, containing task1 , task2 pcb 
#   2. Save task1 state to task1 pcb
#   3. restrore task2 state from pcb 
#   4. switch to task2
#

#include "hw_defs.h"

.global context_switch
/* new mode of checking 
. check for mode
. if irq
	set the stack_top 


if current mode is swi, that means, user regs (r0-r12, lr) are already stores, we have to somehow store shadowed r13, r14
	now store r0-r12 (of swi)  switch to swi by modifying cpsr, save r13,r14, spsr(which is cpsr of user)
*/



check_mode:
	push {lr}
	mrs r0, spsr	
	and r0, r0, #0x1f
	cmp r0, #0x13  
	beq store_svc_regs
	pop {pc}
store_svc_regs:
		/* go to svc mode */
	mrs r0, cpsr
	bic r0, #0x1f
	orr r0, #0x13
	msr cpsr_c, r0
	ldr r12, =cur_pcb_ptr				/* now in svc mode overwrite pcb sp, lr with svc sp, lr*/
	ldr r12, [r12]
	str r13, [r12, #0x30]
	str r14, [r12, #0x34]
	mov r13, #SVC_STACK	/* till earlier we are pointing to process svc stack, now reset it */
	mrs r0, cpsr
	bic r0, #0x1f
	orr r0, #0x12
	msr cpsr_c, r0	
	pop {pc}
	
.global save_process_context_irq, save_process_context_svc

save_process_context_svc:
	
	
save_process_context_irq:
	mov sp, #IRQ_STACK - #60
	ldmfd sp!, {r0-r12, lr}
	ldr sp, =cur_pcb_ptr	
	ldr sp, [sp]
	stmia sp, {r0-r14}^  /* this will use the user registers, ie, r0-r12 common registers and r13_user n r14_user
			      after this statement, r13 and r14 will reflect the current mode registers
			      if we use pc as the last argument to ^, then spsr is copied to cpsr otherwise
			      the registers of the saved spsr are temporarily masked for the duration of operation*/	
	
	add sp, sp, #68	
	mrs  r0, spsr	      /* get the spsr register of current task */
	stmdb sp, {r0, lr}   /* copy the spsr and LR i.e the return address of task, no need of ^	*/

	/* check mode, if swi, then pcb's sp, lr points to user_stack and user_lr or sys_stack and sys_lr*/
	/* switch to swi, get the sp, lr and overwrite pcb sp and lr respectively 
	   current context is saved 
	   now restore the next task context */

	mov r13, #IRQ_STACK
	
	bl check_mode

	bl move_cur_pcb
	
	ldr r13, =nxt_pcb_ptr
	ldr r13, [r13]

	add r13, r13, #60
	ldm r13, {r0, lr}

	msr spsr_cxsf, r0

	sub r13, r13, #60
	mov r1, r13		/* this is req bcoz, if prev is svc then r13 is not longer accessible */
	mrs r0, spsr	
	and r0, r0, #0x1f
	cmp r0, #0x13  		/* if prev mode is svc restore svc regs */
	beq  restore_svc_regs
	ldmia r13, {r0-r14}^
	b epilogue

restore_svc_regs:
	mrs r0, cpsr
	bic r0, #0x1f
	orr r0, #0x13
	msr cpsr_c, r0
	mov r13, r1
	ldmia r13, {r0-r14}	/* this will restore svc r13, r14 */
	mrs r0, cpsr
	bic r0, #0x1f
	orr r0, #0x12
	msr cpsr_c, r0
	ldmia r13, {r0-r12}	/* now in irq mode, r13 is not changed, so .. */

epilogue:
	/* update the set_current 	
	restore r13 of irq stack */
	mov r13, #IRQ_STACK
	movs pc, r14


.global cur_pcb_ptr, nxt_pcb_ptr

.data
cur_pcb_ptr:
	.word 0x00000000
nxt_pcb_ptr:
	.word 0x00000000
