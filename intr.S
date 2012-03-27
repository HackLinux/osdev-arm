.text
.equ VIC_BASEADDR, 0x10140000
.equ INTRSEL_OFF,  0x00c
.equ INTRENBL_OFF, 0x010
.equ SOFTINTR_OFF, 0x018
.global enable_vic, gen_soft_irq

enable_vic:
	push {lr}
	ldr r0, =VIC_BASEADDR		/* load base addr */
	mov r1, #2			/* software interrupt line */	
	ldr r2, [r0, #INTRSEL_OFF]	/* load the interrupt select register */
	bic r2, r2, r1			/* clear all interrupt lines except */ 
	str r2, [r0, #INTRSEL_OFF]	/* enables software interrupt line */
	str r1, [r0, #INTRENBL_OFF]

/* enable fiq interrupts */
	bl dump_cpsr
	mrs r0, cpsr
/*	orr r0, r0, #0x80*/
/*	and r0, r0, #0xFFFFFFBF*/
	msr cpsr_c, r0
	
	mov r0, lr
	bl screwed_up	
	pop {pc}


gen_soft_irq:
	ldr r0, =VIC_BASEADDR
	mov r1, #2
	str r1, [r0, #SOFTINTR_OFF]	
	mov pc, lr
