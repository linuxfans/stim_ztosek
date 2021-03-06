//////////////////////////////////////////////////////////////////////////
//ZtOS 处理器相关文件
//文件：ztos_cpu.c
//作者：张鑫
//日期：[11/19/2005]
//////////////////////////////////////////////////////////////////////////


#define	__ZTOS_CPU_C
#include "ztos.h"

//////////////////////////////////////////////////////////////////////////
//初始化任务堆栈
//描述：		PC
//				LR
//				R12-R1
//				r0(arg)
//				SPSR
//////////////////////////////////////////////////////////////////////////
OS_TSK_STK* os_tsk_stk_init(void (*task)(void *pd), void *pdata, OS_TSK_STK *ptos)
{
	OS_TSK_STK *stk;

	stk    = ptos;                          //获取堆栈指针 

	*stk = (OS_TSK_STK) task;                   //PC
	*--stk = (OS_TSK_STK) task;                 //LR

	stk -= 12;						   		//R1-R12
	*--stk = (UINT32) pdata;				//R0，传递参数pdata
	*--stk = (USER_USING_MODE|0x00);		//SPSR，允许 IRQ, FIQ 中断
	return (stk);
}

void os_tsk_start()
{
	SWI_Interrupt(3);							//软中断，系统调用
}

void SWI_Interrupt(int n)
{
  asm volatile ("mov r0, %0; swi 0"
  				:
  				: "r" (n));
}

//////////////////////////////////////////////////////////////////////////
//	End of file
//////////////////////////////////////////////////////////////////////////

								  
#undef	__ZTOS_CPU_C
