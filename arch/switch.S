#   1. two arguments, containing task1 , task2 pcb 
#   2. Save task1 state to task1 pcb
#   3. restrore task2 state from pcb 
#   4. switch to task2
#

.global context_switch

context_switch:
	ldmfd sp!, {r0-r12, lr}
	ldr r13, =cur_pcb_ptr	
	ldr r13, [r13]
	stmia r13, {r0-r14}^  /* this will use the user registers, ie, r0-r12 common registers and r13_user n r14_user
			      after this statement, r13 and r14 will reflect the current mode registers
			      if we use pc and the last argument to ^, then spsr is copied to cpsr otherwise
			      the registers of the saved spsr and temporarily masked for the duration of operation*/	
	
	add r13, r13, #60	
	mrs  r0, spsr	      /* get the spsr register of current task */
	stmdb r13, {r0, lr}   /* copy the spsr and LR i.e the return address of task, no need of ^	*/

	/* current context is saved */
	/* now restore the next task context */

	ldr r13, =nxt_pcb_ptr
	ldr r13, [r13]

	add r13, r13, #60
	ldmia r13, {r0, lr}

	msr spsr_cxsf, r0

	sub r13, r13, #60
	ldmia r13, {r0-r14}^
	
	movs pc, r14


.global cur_pcb_ptr, nxt_pcb_ptr

.data
cur_pcb_ptr:
	.word 0x00000000
nxt_pcb_ptr:
	.word 0x00000000
