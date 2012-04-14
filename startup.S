.global _reset, change_to_mode
_reset:
	bl setup_stacks
	ldr r0, =panic
	bl asm_log_info
	bl main
	b .

/*Setup stack addresses*/

setup_stacks:
	mov sp, #SVC_STACK		/*r13_svc (Supervisor mode)*/

	msr cpsr_c, #IRQ_MDMASK		/*switch to irq mode*/
	mov sp, #IRQ_STACK		/*r13_irq */
	
	msr cpsr_c, #FIRQ_MDMASK	/*switch to fast irq mode*/
	mov sp, #FIQ_STACK		/*r13_fiq*/	

	
	msr cpsr_c, #ABRT_MD		/*switch to abort mode*/
	mov sp, #ABRT_STACK		/*r13_abrt*/

	msr cpsr_c, #UND_MD 		/*switch to undefined mode*/
	mov sp, #UND_STACK		/*r13_und*/
	
	msr cpsr_c, #SYS_MD		/*switch to system mode*/
	mov sp, #SYS_STACK		/*r13_sys*/
	
	msr cpsr_c, #SVC_MD 
	mov pc, lr


dump:
	ldr r0, =panic
	bl asm_log_info

change_to_mode:
	mov r2, lr
	mrs r1, cpsr
	orr r1, r0
	msr cpsr_c, r1
	mov pc, r2
	
panic:
	.string "panic"





.equ STACK_SIZE, 0x1000
.equ BASE_STACK, 0x20000
.equ ABRT_STACK, BASE_STACK+STACK_SIZE
.equ FIQ_STACK , ABRT_STACK+STACK_SIZE
.equ IRQ_STACK , FIQ_STACK+STACK_SIZE
.equ SVC_STACK , IRQ_STACK+STACK_SIZE
.equ SYS_STACK , SVC_STACK+STACK_SIZE
.equ UND_STACK , SYS_STACK+STACK_SIZE
.equ USR_STACK , UND_STACK+STACK_SIZE
.equ ABRT_MD	 , 0x17
.equ FIQ_MD	 , 0x11
.equ IRQ_MD	 , 0x12
.equ SVC_MD	 , 0x13 
.equ SYS_MD	 , 0x1f
.equ UND_MD	 , 0x1b
.equ USR_MD	 , 0x10

.equ IRQ_MDMASK  , 0xd2
.equ FIRQ_MDMASK , 0xd1


