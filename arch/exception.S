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
	stmfd sp,{r0-r14}^
	sub sp, sp, #60
	stmfd sp!, {lr}
	# read the swi instruction
	ldr r0, [lr, #-4]
	# mask of the top 8 bits
	bic r0, r0, #0xff000000
	mov lr,pc
	ldr pc,=syscall
	ldmfd sp!, {lr}
	ldmfd sp, {r0-r14}^
	add sp, sp, #60
	movs pc, lr

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
	bl check_schedule_needed 
	ldmfd sp!, {r0-r12, pc}^ 

fiq_vec:
	subs lr, lr, #4
	stmfd sp!, {r0-r12, lr}
	mov lr, pc
	ldr pc, =fiq_handler
	bl check_schedule_needed
	ldmfd sp!, {r0-r12, pc}^


check_schedule_needed:
/* check for schedule_needed func, if set call schedule which will never return, if cur mode is irq/fiq */
	push {lr}
	ldr r2, =schedule
	mov lr, pc
	ldr pc, =schedule_needed
	cmp r0, #1
	moveq pc,r2	/* never returns back */
	pop {pc}
