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
/*

  ==============================================
||		  user process 			||	
		  ------------
||                   stack top			||	
||		 ________________		||
||		|_____user regs__|
		
		_________________
||		|_____swi regs___|		||
||                   				||
||						||
  ==============================================
||		  kernel process		||
		  --------------
||		   stack top			||		
||		 ________________		||
||		|___kernel regs__|		||
		_________________
||		|___swi    regs__|		||
||						||
  ==============================================

*/

#include "hw_defs.h"


/* We store the user/system context here so that we are transperent to scheduler about our prev state */
/* make sure swi sp is restored to the original when context switching */
swi_vec:
	stmfd sp,{r0-r14}^	
	sub sp, sp, #60
	mrs r0, spsr
	stmfd sp!, {r0, lr}

	# load svc stack top in r0
	push {lr}
	mov lr, pc
	ldr pc,=get_svc_stack
	pop {lr}

	#copy contents of 68 bytes from svc stack top  to per process svc stack

	sub r2, sp, #68
loop:	ldmfd sp!, {r1} 
	stmfd r0!, {r1}
	cmp r2, sp
	bne loop
	mov sp, r0		
	

	
	# read the swi instruction
	ldr r0, [lr, #-4]
	# mask of the top 8 bits
	bic r0, r0, #0xff000000
	mov lr,pc
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
