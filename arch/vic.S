.text
.equ VIC_BASEADDR, 0x10140000
.equ IRQSTATUS_OFF, 0x00
.equ FIQSTATUS_OFF, 0x04
.equ RAWSTATUS_OFF, 0X08

.equ INTRSEL_OFF,  0x00c
.equ INTRENBL_OFF, 0x010
.equ INTRCLR_OFF,  0x014


.equ SOFTINTR_OFF,    0x018
.equ SOFTINTRCLR_OFF, 0x01C

.equ VECADDR_OFF,     0X030
.equ DEF_VECTADDR_OFF, 0x034
.equ VECTADDR_BASE_OFF, 0x100
.equ VECTCNTL_BASE_OFF, 0x200

.global enable_vic, gen_soft_irq, enable_irq, enable_fiq, enable_cpsr_irq, enable_cpsr_fiq, \
	disable_cpsr_irq, disable_cpsr_fiq

.global clear_hard_irq, clear_soft_irq
.global disable_interrupts, enable_interrupts


/* enable irq interrupts */
enable_cpsr_irq:
	mrs r0, cpsr
	and r0, r0, #0xFFFFFF7F
	msr cpsr_c, r0
	mov pc, lr

disable_cpsr_irq:
	mrs r0, cpsr
	orr r0, r0, #0x80
	msr cpsr_c, r0
	mov pc, lr





/* enable fiq interrrupts */
enable_cpsr_fiq:
	mrs r0, cpsr
	and r0, r0, #0xFFFFFFBF
	msr cpsr_c, r0
	mov pc, lr

disable_cpsr_fiq:
	mrs r0, cpsr
	orr r0, r0, #0x40
	msr cpsr_c, r0
	mov pc, lr



disable_interrupts:
	push {lr}
	bl disable_cpsr_irq
	bl disable_cpsr_fiq	
	pop {pc}

enable_interrupts:
    push {lr}
    bl enable_cpsr_irq
    bl enable_cpsr_fiq
    pop {pc}

/* we expect only one argument in r0, which contains the interrupt to enable
r0 = interrupt to enable
eg: enable_irq(1);
Generates an irq interrupt to processor, if an interrupt occurs on line 1
*/

enable_irq:
	stmfd sp!, {lr}

	ldr r1, =VIC_BASEADDR		/* load base addr */
	ldr r2, [r1, #INTRSEL_OFF]	/* load the interrupt select register */
	bic r2, r2, r0			/* enable irqs, by masking the corresponding irq bit */
	str r2, [r1, #INTRSEL_OFF]	/* enables software interrupt line */
	str r0, [r1, #INTRENBL_OFF]

	ldmfd sp!, {pc}

/* 
r0 = irq no
eg: enable_fiq(2);
Generates an fiq interrupt to processor, if an interrupt occurs on line 2
*/
enable_fiq:
	stmfd sp!, {lr}

	ldr r1, =VIC_BASEADDR		
	str r0, [r1, #INTRSEL_OFF]	
	str r0, [r1, #INTRENBL_OFF]
	
	ldmfd sp!, {pc}


enable_vector_irq:
	stmfd sp!, {lr}

	mov r1, r0		/* contains interrupt number */
	ldr r0, =VIC_BASEADDR
	
	ldmfd sp!, {r0-r3, pc}
/*generate_softint(20): Generates soft interrupt on line 20*/

gen_soft_irq:
	stmfd sp!, {lr}
	ldr r1, =VIC_BASEADDR
	str r0, [r1, #SOFTINTR_OFF]	
	ldmfd sp!, {pc}

.global fiq_handler, irq_handler


clear_soft_irq:
	ldr r1, =VIC_BASEADDR
	str r0, [r1, #SOFTINTRCLR_OFF]  
	mov pc, lr

clear_hard_irq:
	ldr r1, =VIC_BASEADDR
	str r0, [r1, #INTRCLR_OFF]
	mov pc, lr

get_enabled_softirq:




fiq_handler:
	stmfd sp!, {lr}
	ldr r3, =VIC_BASEADDR
	ldr r0, [r3, #FIQSTATUS_OFF] 
	bl handle_generic_IRQ 

/*	ldr r1, =VIC_BASEADDR
	str r0, [r1, #SOFTINTRCLR_OFF] */

	ldmfd sp!, {pc}


irq_handler:
	stmfd sp!, {lr}
	
	ldr r3, =VIC_BASEADDR
	ldr r0, [r3, #IRQSTATUS_OFF] 
        bl handle_generic_IRQ

/*      ldr r1, =VIC_BASEADDR
        str r0, [r1, #SOFTINTRCLR_OFF] */
	
	ldmfd sp!, {pc}
