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

/* switch stack depending on process id  before interrupt
   disable interrupts
   set stack depending on pid
   store registers
   enable interrupts  
   .... goto syscall 
   disable interrupts
   restore registers
   enable interrupts
   
   
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
/* check for schedule_needed func, if set call schedule which will never return back in irq/fiq mode  */
	push {lr}
	ldr r2, =schedule
	mov lr, pc
	ldr pc, =schedule_needed
	cmp r0, #1
	moveq pc,r2	/* never returns back */
	pop {pc}
