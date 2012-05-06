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

swi_vec:
	stmfd sp!,{r0-r12,lr}
	# read the swi instruction
	ldr r0, [lr, #-4]
	# mask of the top 8 bits
	bic r0, r0, #0xff000000
	mov lr,pc
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
