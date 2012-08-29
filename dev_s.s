.equ OS_DEV_USE_UART0, 1
.equ OS_DEV_USE_UART1, 1
/*.equ OS_DEV_USE_UART2, 1*/
/*.equ OS_DEV_USE_UART3, 1*/
.equ OS_DEV_USE_TIMER0, 1
/*.equ OS_DEV_USE_NRF, 1*/

.text
.arm



			.extern os_int_ctx_sw

            .extern os_tcb_cur /*指向当前任务TCB的指针			  */
            .extern os_tcb_rdy /*指向将就绪最高优先级任务TCB的指针	*/
            .extern os_pri_cur /*当前任务的优先级*/
            .extern os_pri_rdy /*就绪最高优先级任务的优先级		*/
		


.ifdef 	OS_DEV_USE_UART0
.extern uart0_irq_c
.global UART0_IRQ,
.func	UART0_IRQ	
UART0_IRQ:
				SUB		LR, LR, #4
				STMFD	SP!, {R0-R3, R12, LR}
				MRS		R3, SPSR
				STMFD	SP!, {R3}



				BL		uart0_irq_c

				MSR		CPSR_c, #0X92


				LDR		R0, =os_tcb_rdy
				LDR		R0, [R0]
				LDR		R1, =os_tcb_cur
				LDR		R1, [R1]
				CMPEQ	R0, R1

				LDMFD	SP!, {R3}
				MSR		SPSR_cxsf, R3

				LDMEQFD	SP!, {R0-R3, R12, PC}^
				LDR		PC, =os_int_ctx_sw
.endfunc

.endif

.ifdef 	OS_DEV_USE_UART1
.extern uart1_irq_c
.global UART1_IRQ,
.func	UART1_IRQ
UART1_IRQ:
				SUB		LR, LR, #4
				STMFD	SP!, {R0-R3, R12, LR}
				MRS		R3, SPSR
				STMFD	SP!, {R3}



				BL		uart1_irq_c

				MSR		CPSR_c, #0X92


				LDR		R0, =os_tcb_rdy
				LDR		R0, [R0]
				LDR		R1, =os_tcb_cur
				LDR		R1, [R1]
				CMPEQ	R0, R1

				LDMFD	SP!, {R3}
				MSR		SPSR_cxsf, R3

				LDMEQFD	SP!, {R0-R3, R12, PC}^
				LDR		PC, =os_int_ctx_sw
.endfunc

.endif


.ifdef 	OS_DEV_USE_UART2
.extern uart2_irq_c
.global UART2_IRQ,
.func	UART2_IRQ
UART2_IRQ:
				SUB		LR, LR, #4
				STMFD	SP!, {R0-R3, R12, LR}
				MRS		R3, SPSR
				STMFD	SP!, {R3}



				BL		uart2_irq_c

				MSR		CPSR_c, #0X92


				LDR		R0, =os_tcb_rdy
				LDR		R0, [R0]
				LDR		R1, =os_tcb_cur
				LDR		R1, [R1]
				CMPEQ	R0, R1

				LDMFD	SP!, {R3}
				MSR		SPSR_cxsf, R3

				LDMEQFD	SP!, {R0-R3, R12, PC}^
				LDR		PC, =os_int_ctx_sw
.endfunc

.endif


.ifdef 	OS_DEV_USE_UART3
.extern uart3_irq_c
.global UART3_IRQ,
.func	UART3_IRQ
UART3_IRQ:
				SUB		LR, LR, #4
				STMFD	SP!, {R0-R3, R12, LR}
				MRS		R3, SPSR
				STMFD	SP!, {R3}



				BL		uart3_irq_c

				MSR		CPSR_c, #0X92


				LDR		R0, =os_tcb_rdy
				LDR		R0, [R0]
				LDR		R1, =os_tcb_cur
				LDR		R1, [R1]
				CMPEQ	R0, R1

				LDMFD	SP!, {R3}
				MSR		SPSR_cxsf, R3

				LDMEQFD	SP!, {R0-R3, R12, PC}^
				LDR		PC, =os_int_ctx_sw
.endfunc

.endif

.ifdef 	OS_DEV_USE_TIMER0
.extern timer0_irq_c
.global TIMER0_IRQ	
.func	TIMER0_IRQ
TIMER0_IRQ:
				SUB		LR, LR, #4
				STMFD	SP!, {R0-R3, R12, LR}
				MRS		R3, SPSR
				STMFD	SP!, {R3}



				BL		timer0_irq_c

				MSR		CPSR_c, #0X92

				LDR		R0, =os_tcb_rdy
				LDR		R0, [R0]
				LDR		R1, =os_tcb_cur
				LDR		R1, [R1]
				CMPEQ	R0, R1

				LDMFD	SP!, {R3}
				MSR		SPSR_cxsf, R3

				LDMEQFD	SP!, {R0-R3, R12, PC}^
				LDR		PC, =os_int_ctx_sw
.endfunc
.endif




.ifdef 	OS_DEV_USE_TIMER1
.extern timer1_irq_c
.global TIMER1_IRQ
.func	TIMER2_IRQ	
TIMER1_IRQ:
				SUB		LR, LR, #4
				STMFD	SP!, {R0-R3, R12, LR}
				MRS		R3, SPSR
				STMFD	SP!, {R3}



				BL		timer1_irq_c

				MSR		CPSR_c, #0X92

				LDR		R0, =os_tcb_rdy
				LDR		R0, [R0]
				LDR		R1, =os_tcb_cur
				LDR		R1, [R1]
				CMPEQ	R0, R1

				LDMFD	SP!, {R3}
				MSR		SPSR_cxsf, R3

				LDMEQFD	SP!, {R0-R3, R12, PC}^
				LDR		PC, =os_int_ctx_sw
.endfunc				
.endif
			  
.ifdef 	OS_DEV_USE_NRF
.extern nrf905_4_irq_c
.global NRF905_4_IRQ
.func	NRF905_4_IRQ	
NRF905_4_IRQ:
				SUB		LR, LR, #4
				STMFD	SP!, {R0-R3, R12, LR}
				MRS		R3, SPSR
				STMFD	SP!, {R3}

				BL		nrf905_4_irq_c

				MSR		CPSR_c, #0X92

				LDR		R0, =os_tcb_rdy
				LDR		R0, [R0]
				LDR		R1, =os_tcb_cur
				LDR		R1, [R1]
				CMPEQ	R0, R1

				LDMFD	SP!, {R3}
				MSR		SPSR_cxsf, R3

				LDMEQFD	SP!, {R0-R3, R12, PC}^
				LDR		PC, =os_int_ctx_sw
.endfunc				
.endif

/*
#if	defined(OS_DEV_USE_PWM)
EXTERN CODE32 (pwm_irq_c?A)			
PUBLIC PWM_IRQ?A		
PWM_IRQ?A      	PROC    CODE32
				SUB		LR, LR, #4
				STMFD	SP!, {R0-R3, R12, LR}
				MRS		R3, SPSR
				STMFD	SP!, {R3}



				BL		pwm_irq_c?A

				MSR		CPSR_c, #0X92

				LDR		R0, =os_tcb_rdy
				LDR		R0, [R0]
				LDR		R1, =os_tcb_cur
				LDR		R1, [R1]
				CMPEQ	R0, R1

				LDMFD	SP!, {R3}
				MSR		SPSR_cxsf, R3

				LDMEQFD	SP!, {R0-R3, R12, PC}^
				LDR		PC, =os_int_ctx_sw
				ENDP
#endif*/
.end
