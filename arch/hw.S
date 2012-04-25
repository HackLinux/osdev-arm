.global get_cpsr

get_cpsr:
	mrs r0, cpsr
	mov pc, lr
