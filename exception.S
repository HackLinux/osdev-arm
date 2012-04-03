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
	stmfd sp!,{r0-r3,lr}
	mov lr,pc
	mov r0, r10
	ldr pc,=syscall
	ldmfd sp!,{r0-r3,pc}^

abort_data_vec:
	b abort_data_vec

abort_prefetch_vec:
	b abort_prefetch_vec

reserved_vec:
	b reserved_vec

irq_vec:
	subs lr, lr, #4
	stmfd sp!, {r0-r3, lr}
	mov lr, pc
	ldr pc, =irq_handler
	ldmfd sp!, {r0-r3, pc}^ 

fiq_vec:
	subs lr, lr, #4
	stmfd sp!, {r0-r3, lr}
	mov lr, pc
	ldr pc, =fiq_handler
	ldmfd sp!, {r0-r3, pc}^


.global offset
.data
offset :
	.word 0x00000000

