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
	bl syscall
	ldmfd sp!,{r0-r3,pc}^

abort_data_vec:
	b abort_data_vec

abort_prefetch_vec:
	bl screwed_up
	b abort_prefetch_vec

reserved_vec:
	b reserved_vec

irq_vec:
	bl fiq_handler

fiq_vec:
	b fiq_vec
	bl fiq_handler

.data
STRING_IDT:
.string "string data in idt"
