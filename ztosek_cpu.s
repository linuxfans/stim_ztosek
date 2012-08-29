
/*
//ZtOS 处理器相关文件
//文件：ztosek_cpu.s
//作者：张鑫
//日期：[11/19/2005]													  
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
USR32Mode   EQU 0x10								;用户模式
SVC32Mode   EQU 0x13								;管理模式
SYS32Mode   EQU 0x1f								;系统模式
IRQ32Mode   EQU 0x12								;IRQ模式
FIQ32Mode   EQU 0x11								;FIQ模式

;T_bit用于检测进入异常前cpu是否处于THUMB状态
T_bit               EQU         0x20
*/

        .text
        .arm
/*
    AREA    |subr|, CODE, READONLY

            EXTERN  DATA	(os_tcb_cur)				;指向当前任务TCB的指针
            EXTERN  DATA	(os_tcb_rdy)				;指向将就绪最高优先级任务TCB的指针
            EXTERN  DATA	(os_pri_cur)				;当前任务的优先级
            EXTERN  DATA	(os_pri_rdy)				;就绪最高优先级任务的优先级
            EXTERN  DATA	(os_running)				;ZtOS运行标志
*/
            
            .global	SWI_Handler  
			.global		os_int_ctx_sw

# .section .text,"x"
#.func	SWI_Handler
.arm
.global SWI_Handler,中断

/*////////////////////////////////////////////////////////////////////////
//软件中断
// [8/16/2005]
////////////////////////////////////////////////////////////////////////*/

# Software Interrupt软
SWI_Handler:
        CMP		R0, #4		 					/*跳转到相应系统调用*/
        LDRLO	PC, [PC, R0, LSL #2]
        MOVS	PC, LR			

SwiFunction:
        .word		TSK_SW
        .word		ENTER_CRITICAL
        .word		LEAVE_CRITICAL
		.word		TSK_START


TSK_SW:
        MRS		R3, SPSR                        /*保存任务的CPSR*/
        MOV		R2, LR                          /*保存任务的PC*/
        
        MSR		CPSR_c, #(NoInt | Mode_SYS)		/*切换到系统模式*/
        STMFD	SP!, {R2}                       /*保存PC到堆栈*/
        STMFD   SP!, {R0-R12, LR}               /*保存R0-R12,LR到堆栈*/
        B       os_int_ctx_sw_0                 /*真正进行任务切换*/

ENTER_CRITICAL:									/*关中断*/                                 
        MRS     R0, SPSR
        ORR     R0, R0, #NoInt
        MSR     SPSR_c, R0
        MOVS    PC, LR

LEAVE_CRITICAL:									/*开中断*/
        MRS		R0, SPSR
        BIC		R0, R0, #NoInt
        MSR		SPSR_c, R0
        MOVS	PC, LR





/*////////////////////////////////////////////////////////////////////////
//中断级切换
// [8/18/2005]
////////////////////////////////////////////////////////////////////////*/
os_int_ctx_sw:
												/*下面为保存任务环境*/
        LDR     R2, [SP, #20]					/*获取PC*/
        LDR     R12, [SP, #16]					/*获取R12*/
        MRS     R0, CPSR

        MSR     CPSR_c, #(NoInt | Mode_SYS)
        MOV     R1, LR
        STMFD   SP!, {R1-R2}					/*保存LR,PC*/
        STMFD   SP!, {R4-R12}					/*保存R4-R12*/

        MSR     CPSR_c, R0
        LDMFD   SP!, {R4-R7}					/*获取R0-R3*/
        ADD     SP, SP, #8						/*出栈R12,PC*/
        
        MSR     CPSR_c, #(NoInt | Mode_SYS)
        STMFD   SP!, {R4-R7}					/*保存R0-R3*/
        
os_int_ctx_sw_0:        

        STMFD   SP!, {R3}						/*保存CPSR*/
												/*保存堆栈指针到当前任务的TCB*/
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
												/*获取新任务堆栈指针*/
        LDR     R4, [R6]
        ADD     SP, R4, #64						/*16个寄存器（CPSR,R0-R12,LR,SP）*/
        LDR     LR, [SP, #-8]
        MSR     CPSR_c, #(NoInt | Mode_SYS)	/*进入管理模式*/
        MOV     SP, R4							/*设置堆栈指针*/

        LDMFD   SP!, {R5}						/*CPSR*/
    
        MSR     SPSR_cxsf, R5					/*恢复CPSR*/
        LDMFD   SP!, {R0-R12, LR, PC }^			/*运行新任务*/

/*////////////////////////////////////////////////////////////////////////
//第一次运行任务
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