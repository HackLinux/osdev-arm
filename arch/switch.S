#   1. two arguments, containing task1 , task2 pcb 
#   2. Save task1 state to task1 pcb
#   3. restrore task2 state from pcb 
#   4. switch to task2
#

#include "hw_defs.h"

.global save_process_context_in_irq, save_process_context_svc, save_process_context_sys


save_process_context_in_irq:
	ldr sp, =(IRQ_STACK - 56)
	ldmfd sp!, {r0-r12, lr}
	ldr r13, =cur_pcb_ptr	
	ldr sp, [r13]
	stmia sp, {r0-r14}^  /* this will use the user registers, ie, r0-r12 common registers and r13_user n r14_user
			      after this statement, r13 and r14 will reflect the current mode registers
			      if we use pc and the last argument to ^, then spsr is copied to cpsr otherwise
			      the registers of the saved spsr and temporarily masked for the duration of operation*/	
	
	add sp, sp, #68	
	mov  r0, lr
	mrs  r1, spsr	      
	stmdb sp, {r0, r1}   /* copy the lr and spsr on pcb no need of  ^*/

	/* current task context is saved 
	 * restore the next task context 
 	 */
	ldr sp, =IRQ_STACK
	b restore_process_regs

save_process_context_sys:
	mov r0, lr
	msr cpsr_c, #IRQ_MD
	ldr sp, =IRQ_STACK
	mov lr, r0
	stmfd sp!, {r0-r12, lr}
	b save_process_context_in_irq

save_process_context_svc:
	mov r0, sp
	ldr r13, =cur_pcb_ptr
	ldr sp, [r13]
	stmia sp!, {r0-r12}
	stmia sp!, {r0,r14}
	mov r0, lr
	mrs r1, cpsr
	mrs r2, spsr
	stmia sp!, {r0-r2}
		
	ldr sp, =SVC_STACK
	b restore_process_regs	

restore_process_regs:
	bl move_cur_pcb
	bl get_process_mode_cpsr	
	cmp r0, #SVC_MD
	beq restore_process_svc_regs
	cmp r0, #USR_MD
	beq restore_process_usr_regs
	cmp r0, #SYS_MD
	beq restore_process_sys_regs
	ldr r0, =outage_string
	ldr pc,=panic


/******************************************************************************************* 
according to abi , r0-r4 are used as arguments and temporaries, so .... we can use them 
we are being called from svc context, so we shud get back to the next instruction after 
*******************************************************************************************/ 
restore_process_svc_regs:
	bl get_process_spsr	
	mov r5, r0
	bl get_process_cpsr	
	msr cpsr, r0
	msr spsr, r5
	ldr r13, =nxt_pcb_ptr
	ldr sp, [r13]
	ldmia sp, {r0-r14}
/*	mov sp, r0*/
	mov pc, lr		/* we are already in svc mode, do not use movs, it will change mode */



/*************************************************************************************************
   1. with mode change regs/stacks 
   2. Lets make a decision that we will move to IRQ mode when restoring user/sys regs this makes
      things easier

flow:
	. switch to IRQ_MODE
	. set the sys/user space svc stack pointer, which helps in switching from sys/user to svc 
	which shall not conflict with other process svc stack
	. restore regs from pcb except spsr, which sys/user does not have, so move to IRQ mode
	make this transparent.
**************************************************************************************************/

restore_process_usr_regs:	
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


		
outage_string:
	.string "You moron, screwed up everything"
