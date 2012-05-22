.text 
reset:	
	b reset_vec

undefined: 	
	b undefined_vec

swi:	
	b swi_vec

abort_d:	
	b abort_data_vec

abort_p:	
	b abort_prefetch_vec

reserved:	
	b reserved_vec

irq:	
	b irq_vec

fiq:	
	b fiq_vec


reset_vec:
	b reset_vec

undefined_vec:
	b undefined_vec
/*
Okay. Here is the problem, 
	. We are trying to implement syscall which may sleep and hence schedule
      	. and another case where in syscall, interrupt comes and calls scheduler	

Assumptions: No nested irqs
user space => swi => 
		case 1: swi stores user space regs
			1.1 then interrupt comes, saves the swi context, context switch happens 
			then later context switches to the process and goes on
			1.2 swi is sleeping, call scheduler and context switch
		case 2: swi did not store the user space regs
			then interrupt comes, save the swi context, but do not switch the scheduler.

	for case 2: 
		simply do not context switch, if the prev mode is swi and user regs are not saved,
		return from irq/fiq/abort will directly go to swi mode and then goes to case 1
	for case 1:
		each process maintains a stack, which stores

  ==============================================
||		  user process 			||	
		  ------------
||                   stack top			||	
||		 ________________		||
||		|_____user regs__|		||
||		|_____swi regs___|		||
||                   				||
||						||
  ==============================================
||		  kernel process		||
		  --------------
||		   stack top			||		
||		 ________________		||
||		|___kernel regs__|		||
||		|___swi    regs__|		||
||						||
  ==============================================

process stack is shared by both (user & swi) or (kernel & swi)
context_switch code:
===================
while saving if previous mode is swi, 
	. store user regs in the pcb
	. store swi regs  in the pcb
While restoring, if prev mode is swi,
	. restore swi regs
	. copy the user regs onto swi stack	
			
*/

swi_vec:
	stmfd sp!,{r0-r12,lr}
	# read the swi instruction
	ldr r0, [lr, #-4]
	# mask of the top 8 bits
	bic r10, r10, #0xff000000
	mov lr,pc
	/* load syscall table and r10 as offset call function */
	ldr pc,=syscall
	bl check_context_switch  
	ldmfd sp!,{r0-r12,pc}^

abort_data_vec:
	b abort_data_vec

abort_prefetch_vec:
	b abort_prefetch_vec

reserved_vec:
	b reserved_vec

irq_vec:
	subs lr, lr, #4
	stmfd sp!, {r0-r12, lr}
	mov lr, pc
	ldr pc, =irq_handler
	bl check_context_switch 
	ldmfd sp!, {r0-r12, pc}^ 

fiq_vec:
	subs lr, lr, #4
	stmfd sp!, {r0-r12, lr}
	mov lr, pc
	ldr pc, =fiq_handler
	bl check_context_switch 
	ldmfd sp!, {r0-r12, pc}^


check_context_switch:
/* check for context switch flag, if set call context, switch, which will never return 
	 * so dont save the link register
	 */
	ldr r2, =context_switch
	ldr r0, =context_switch_req
	mov r4, #0
	ldr r3, [r0]
	str r4, [r0] 
	cmp r3, #1
	moveq pc,r2	/* never returns back */
	mov pc, lr
	
/*
.global context_switch_req
.data
context_switch_req:
	.word 0x00000000
*/
