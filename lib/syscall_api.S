
.global exit, sleep, halt

exit:
	push {lr}
	mov lr, pc
	swi #0	
	pop {pc}

sleep:
	push {lr}
	mov lr, pc
	swi #1
	pop {pc}

halt:
    push {lr}
    mov lr, pc
    swi #2
    pop {pc}
