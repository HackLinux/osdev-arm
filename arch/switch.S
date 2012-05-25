#   1. two arguments, containing task1 , task2 pcb 
#   2. Save task1 state to task1 pcb
#   3. restrore task2 state from pcb 
#   4. switch to task2
#

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
	mrs r0, spsr_c	
	and r0, r0, #0x1f
	cmp r0, #0x13  
	be  store_svc_regs
	pop {pc}
store_svc_regs:
		/* go to svc mode */
	mrs r0, cpsr_c
	bic r0, #0x1f
	orr r0, #0x13
	msr cpsr_c, r0
	ldr r12, =cur_pcb_ptr				/* now in svc mode overwrite pcb sp, lr with svc sp, lr*/
	ldr r12, [r12]
	mov r13, [r12, #0x30]
	mov r14, [r12, #0x34]
	mrs r0, cpsr_c
	bic r0, #0x1f
	orr r0, #0x12
	msr cpsr_c, r0	
	pop {pc}
	

context_switch:
	ldmfd sp!, {r0-r12, lr}
	ldr r13, =cur_pcb_ptr	
	ldr r13, [r13]
	stmia r13, {r0-r14}^  /* this will use the user registers, ie, r0-r12 common registers and r13_user n r14_user
			      after this statement, r13 and r14 will reflect the current mode registers
			      if we use pc and the last argument to ^, then spsr is copied to cpsr otherwise
			      the registers of the saved spsr and temporarily masked for the duration of operation*/	
	
	add r13, r13, #68	
	mrs  r0, spsr	      /* get the spsr register of current task */
	stmdb r13, {r0, lr}   /* copy the spsr and LR i.e the return address of task, no need of ^	*/

	/* check mode, if swi, then pcb's sp, lr points to user_stack and user_lr or sys_stack and sys_lr*/
	/* switch to swi, get the sp, lr and overwrite pcb sp and lr respectively 
	/* current context is saved */
	/* now restore the next task context */

	mov r13, #0x23000
	
	bl check_mode

	bl move_cur_pcb
	
	ldr r13, =nxt_pcb_ptr
	ldr r13, [r13]

	add r13, r13, #60
	ldm r13, {r0, lr}

	msr spsr_cxsf, r0

	sub r13, r13, #60
	mrs r0, spsr_c	
	and r0, r0, #0x1f
	cmp r0, #0x13  
	be  restore_svc_regs
	ldmia r13, {r0-r14}^
	b epilogue

restore_svc_regs:
	ldmia r13, {r0-r14}

epilogue:
	/* update the set_current */	
	/* restore r13 of irq stack */
	mov r13, #0x23000
	movs pc, r14


.global cur_pcb_ptr, nxt_pcb_ptr

.data
cur_pcb_ptr:
	.word 0x00000000
nxt_pcb_ptr:
	.word 0x00000000
