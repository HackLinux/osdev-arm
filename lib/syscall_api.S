
.global exit, sleep

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

