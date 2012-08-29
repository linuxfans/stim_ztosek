
/*
//ZtOS ����������ļ�
//�ļ���ztosek_cpu.s
//���ߣ�����
//���ڣ�[11/19/2005]													  
*/

# Standard definitions of Mode bits and Interrupt (I & F) flags in PSRs

        .equ    Mode_USR,       0x10
        .equ    Mode_FIQ,       0x11
        .equ    Mode_IRQ,       0x12
        .equ    Mode_SVC,       0x13
        .equ    Mode_ABT,       0x17
        .equ    Mode_UND,       0x1B
        .equ    Mode_SYS,       0x1F


		.equ	NoInt,			0x80
        .equ    I_Bit,          0x80
        .equ    F_Bit,          0x40 
        .equ    T_Bit,          0x20 
/*
NoInt       EQU 0x80
USR32Mode   EQU 0x10								;�û�ģʽ
SVC32Mode   EQU 0x13								;����ģʽ
SYS32Mode   EQU 0x1f								;ϵͳģʽ
IRQ32Mode   EQU 0x12								;IRQģʽ
FIQ32Mode   EQU 0x11								;FIQģʽ

;T_bit���ڼ������쳣ǰcpu�Ƿ���THUMB״̬
T_bit               EQU         0x20
*/

        .text
        .arm
/*
    AREA    |subr|, CODE, READONLY

            EXTERN  DATA	(os_tcb_cur)				;ָ��ǰ����TCB��ָ��
            EXTERN  DATA	(os_tcb_rdy)				;ָ�򽫾���������ȼ�����TCB��ָ��
            EXTERN  DATA	(os_pri_cur)				;��ǰ��������ȼ�
            EXTERN  DATA	(os_pri_rdy)				;����������ȼ���������ȼ�
            EXTERN  DATA	(os_running)				;ZtOS���б�־
*/
            
            .global	SWI_Handler  
			.global		os_int_ctx_sw

# .section .text,"x"
#.func	SWI_Handler
.arm
.global SWI_Handler,�ж�

/*////////////////////////////////////////////////////////////////////////
//����ж�
// [8/16/2005]
////////////////////////////////////////////////////////////////////////*/

# Software Interrupt��
SWI_Handler:
        CMP		R0, #4		 					/*��ת����Ӧϵͳ����*/
        LDRLO	PC, [PC, R0, LSL #2]
        MOVS	PC, LR			

SwiFunction:
        .word		TSK_SW
        .word		ENTER_CRITICAL
        .word		LEAVE_CRITICAL
		.word		TSK_START


TSK_SW:
        MRS		R3, SPSR                        /*���������CPSR*/
        MOV		R2, LR                          /*���������PC*/
        
        MSR		CPSR_c, #(NoInt | Mode_SYS)		/*�л���ϵͳģʽ*/
        STMFD	SP!, {R2}                       /*����PC����ջ*/
        STMFD   SP!, {R0-R12, LR}               /*����R0-R12,LR����ջ*/
        B       os_int_ctx_sw_0                 /*�������������л�*/

ENTER_CRITICAL:									/*���ж�*/                                 
        MRS     R0, SPSR
        ORR     R0, R0, #NoInt
        MSR     SPSR_c, R0
        MOVS    PC, LR

LEAVE_CRITICAL:									/*���ж�*/
        MRS		R0, SPSR
        BIC		R0, R0, #NoInt
        MSR		SPSR_c, R0
        MOVS	PC, LR





/*////////////////////////////////////////////////////////////////////////
//�жϼ��л�
// [8/18/2005]
////////////////////////////////////////////////////////////////////////*/
os_int_ctx_sw:
												/*����Ϊ�������񻷾�*/
        LDR     R2, [SP, #20]					/*��ȡPC*/
        LDR     R12, [SP, #16]					/*��ȡR12*/
        MRS     R0, CPSR

        MSR     CPSR_c, #(NoInt | Mode_SYS)
        MOV     R1, LR
        STMFD   SP!, {R1-R2}					/*����LR,PC*/
        STMFD   SP!, {R4-R12}					/*����R4-R12*/

        MSR     CPSR_c, R0
        LDMFD   SP!, {R4-R7}					/*��ȡR0-R3*/
        ADD     SP, SP, #8						/*��ջR12,PC*/
        
        MSR     CPSR_c, #(NoInt | Mode_SYS)
        STMFD   SP!, {R4-R7}					/*����R0-R3*/
        
os_int_ctx_sw_0:        

        STMFD   SP!, {R3}						/*����CPSR*/
												/*�����ջָ�뵽��ǰ�����TCB*/
        LDR     R1, =os_tcb_cur
        LDR     R1, [R1]
        STR     SP, [R1]
												/*os_pri_cur <= os_pri_rdy*/
        LDR     R4, =os_pri_cur
        LDR     R5, =os_pri_rdy
        LDRB    R6, [R5]
        STRB    R6, [R4]
												/*os_tcb_cur <= os_tcb_rdy*/
        LDR     R6, =os_tcb_rdy
        LDR     R6, [R6]
        LDR     R4, =os_tcb_cur
        STR     R6, [R4]
        
os_int_ctx_sw_1:
												/*��ȡ�������ջָ��*/
        LDR     R4, [R6]
        ADD     SP, R4, #64						/*16���Ĵ�����CPSR,R0-R12,LR,SP��*/
        LDR     LR, [SP, #-8]
        MSR     CPSR_c, #(NoInt | Mode_SYS)	/*�������ģʽ*/
        MOV     SP, R4							/*���ö�ջָ��*/

        LDMFD   SP!, {R5}						/*CPSR*/
    
        MSR     SPSR_cxsf, R5					/*�ָ�CPSR*/
        LDMFD   SP!, {R0-R12, LR, PC }^			/*����������*/

/*////////////////////////////////////////////////////////////////////////
//��һ����������
// [8/18/2005]
////////////////////////////////////////////////////////////////////////*/
TSK_START:
        MSR     CPSR_c, #(NoInt | Mode_SYS)
												/*os_running = TRUE*/
        LDR     R4, =os_running
        MOV     R5, #1
        STRB    R5, [R4]
												/*R6 = os_tcb_rdy*/
        LDR     R6, =os_tcb_rdy					
        LDR     R6, [R6]
        B       os_int_ctx_sw_1

    .end

/*////////////////////////////////////////////////////////////////////////
//	End of file
////////////////////////////////////////////////////////////////////////*/