//////////////////////////////////////////////////////////////////////////
//ZtOS	核心
//文件：ztos_core.c
//作者：张鑫
//日期：[11/18/2005]
//////////////////////////////////////////////////////////////////////////
#define	__ZTOS_CORE_C
#include "ztos.h"




//////////////////////////////////////////////////////////////////////////
//寻找最高优先级（函数原型，应被高效率内联函数替代）
//描述：
//作者：张鑫
//  [8/16/2005]
//////////////////////////////////////////////////////////////////////////

UINT32 FindHighestPrio( UINT32 r)
{
	//int r;
	UINT32	prio;

	prio = 0;
	if ( (r&0x0000ffff) == 0)
	{
		r>>=16;
		prio += 16;
	}
	if ( (r&0x000000ff) == 0 )
	{
		r>>=8;
		prio += 8;
	}
	if ( (r&0x0000000f) == 0 )
	{
		r>>=4;
		prio += 4;
	}
	if ( (r&0x00000003) == 0 )
	{
		r>>=2;
		prio += 2;
	}
	if ( (r&0x00000001) == 0 )
	{
		prio += 1;
	}
	//cout << prio;
	return prio;
}
//////////////////////////////////////////////////////////////////////////
//操作系统初始化
//描述：初始化系统全局变量
//		建立IDLE任务
//作者：张鑫
//日期：[11/18/2005]
//////////////////////////////////////////////////////////////////////////
void os_init()
{
	UINT32	i;
	//初始化全局变量
	os_running		=	FALSE;
	os_pri_flg		=	0;
	os_pri_rdy		=	0;
	os_pri_cur		=	0;
	os_tcb_rdy		=	(OS_TCB*) NULL;
	os_tcb_cur		=	(OS_TCB*) NULL;

	//初始化信号量
	for ( i = 0 ; i < OS_SEM_MAXNUM ; i++ )
	{
		os_scb_tbl[i].os_scb_cnt	=	0;
		os_scb_tbl[i].os_scb_wat_tsk	=	OS_TSK_MAXNUM;
	}

	//创建 IDEL 任务
	os_tsk_create(os_tsk_idle,
		(void*) NULL,
		&os_stk_idletsk[32-1],
		OS_PRI_IDLE);
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void os_start()
{
	OS_ENTER_CRITICAL();
	if ( os_running == FALSE )
	{
		os_pri_rdy	=	FindHighestPrio(os_pri_flg);
		os_pri_cur	=	os_pri_rdy;
		os_tcb_rdy	=	&(os_tcb_tbl[os_pri_rdy]);
		os_tcb_cur	=	os_tcb_rdy;
		OS_LEAVE_CRITICAL();
		os_tsk_start();
	}
	OS_LEAVE_CRITICAL();
}
//////////////////////////////////////////////////////////////////////////
//创建任务
//描述：初始化任务堆栈
//		初始化任务TCB
//作者：张鑫
//日期：[8/14/2005]
//////////////////////////////////////////////////////////////////////////
void  os_tsk_create (void (*task)(void *pd), void *pdata, OS_TSK_STK *ptos, OS_PRI pri)
{
	OS_TSK_STK*	psp;

	//初始化任务堆栈
	psp = (OS_TSK_STK *)os_tsk_stk_init(task, pdata, ptos);//, 0);	

	//初始化任务TCB
	os_tcb_init(pri, psp);

}
//////////////////////////////////////////////////////////////////////////
//初始化任务TCB
//描述：获取空闲TCB
//		加入TCB链表
//		初始化成员变量
//作者：张鑫
//日期：[8/14/2005]
//////////////////////////////////////////////////////////////////////////
void os_tcb_init(OS_PRI pri, OS_TSK_STK* ptos)
{
	OS_ENTER_CRITICAL();

	(os_tcb_tbl[pri]).os_tcb_stk_ptr	=	ptos;
	(os_tcb_tbl[pri]).os_tcb_pri		=	pri;


	//在os_pri_flg登记
	os_pri_flg	|=	(1 << pri);
	OS_LEAVE_CRITICAL();
}

//////////////////////////////////////////////////////////////////////////
//IDLE任务
//描述：空闲计数器增
//作者：张鑫
//日期：[8/14/2005]
//////////////////////////////////////////////////////////////////////////
void os_tsk_idle(void* pData) 
{
	pData	=	pData;
	for (;;)
	{		
		OS_ENTER_CRITICAL();
		OS_LEAVE_CRITICAL();
	}
}

//////////////////////////////////////////////////////////////////////////
//信号量P操作，并判断超时
//超时返回0，否则返回完成时剩余时间
//作者：张鑫
//  [7/1/2006]
//////////////////////////////////////////////////////////////////////////
INT32 os_sem_p(OS_SEM SemID, UINT32 timeout)
{
	OS_ENTER_CRITICAL();
	os_scb_tbl[SemID].os_scb_timer	=	timeout / 10;
	os_scb_tbl[SemID].os_scb_cnt--;

	//无足够资源，挂起
	if ( os_scb_tbl[SemID].os_scb_cnt < 0)
	{
		os_scb_tbl[SemID].os_scb_wat_tsk	=	os_pri_cur;
		os_pri_flg		&=	~(1<<os_pri_cur);
		OS_LEAVE_CRITICAL();
		//必然进行任务切换
		os_tsk_sch();
	}
	
	OS_LEAVE_CRITICAL();
	return  os_scb_tbl[SemID].os_scb_timer;
}
//////////////////////////////////////////////////////////////////////////
//信号量try操作，并判断超时
//超时返回0，否则返回完成时剩余时间
//作者：张鑫
//  [7/1/2006]
//////////////////////////////////////////////////////////////////////////
INT32 os_sem_try(OS_SEM SemID)
{
	OS_ENTER_CRITICAL();

	//无足够资源，直接返回失败
	if ( os_scb_tbl[SemID].os_scb_cnt <=0)
	{
		OS_LEAVE_CRITICAL();
		return 0;
	}
	os_scb_tbl[SemID].os_scb_cnt--;
	
	OS_LEAVE_CRITICAL();
	return  1;
}


/*
暂不提供此功能
//////////////////////////////////////////////////////////////////////////
//信号量P操作n次
//作者：张鑫
//  [9/1/2005]
//////////////////////////////////////////////////////////////////////////
void os_sem_pn(OS_SEM SemID, INT32 n)
{
	OS_ENTER_CRITICAL();
	os_scb_tbl[SemID].os_scb_cnt		-=	n;

	//无足够资源，挂起
	if ( os_scb_tbl[SemID].os_scb_cnt < 0)
	{
		os_scb_tbl[SemID].os_scb_wat_tsk	=	os_pri_cur;
		os_pri_flg		&=	~(1<<os_pri_cur);
		OS_LEAVE_CRITICAL();
		//必然进行任务切换
		os_tsk_sch();
	}
	OS_LEAVE_CRITICAL();

}
*/

//////////////////////////////////////////////////////////////////////////
//信号量V操作n次
//作者：张鑫
//  [9/1/2005]
//////////////////////////////////////////////////////////////////////////
void os_sem_vn(OS_SEM SemID, INT32 n)
{
	os_scb_tbl[SemID].os_scb_cnt		+=	n;

	//资源足够，就绪被挂起任务
	if ( (os_scb_tbl[SemID].os_scb_cnt) >= 0 &&
		os_scb_tbl[SemID].os_scb_wat_tsk < OS_SEM_MAXNUM) 
	{
		os_pri_flg		|=	( 1 << os_scb_tbl[SemID].os_scb_wat_tsk );
		//如果该任务优先级高于就绪最高优先级任务
		//最高优先级变更，中断级的任务切换通过判断
		//OSPrioHighRdy == OSPrioCur判断是否进行任务切换
		if ( os_scb_tbl[SemID].os_scb_wat_tsk < os_pri_rdy )
		{
			os_pri_rdy	=	os_scb_tbl[SemID].os_scb_wat_tsk;
			os_tcb_rdy	=	&(os_tcb_tbl[os_pri_rdy]);
		}
		//告知任务没有被当前信号量挂起
		os_scb_tbl[SemID].os_scb_wat_tsk	=	OS_SEM_MAXNUM;
	}
}

//////////////////////////////////////////////////////////////////////////
//任务调度
//描述：查找最高优先级就绪任务（将查找最高优先级就绪任务展开）
//		调用任务切换函数（软中断，系统调用）
//////////////////////////////////////////////////////////////////////////
void os_tsk_sch()
{

	UINT32	r = os_pri_flg;
	OS_ENTER_CRITICAL();

	os_pri_rdy = 0;
	if ( (r&0x0000ffff) == 0)
	{
		r>>=16;
		os_pri_rdy += 16;
	}
	if ( (r&0x000000ff) == 0 )
	{
		r>>=8;
		os_pri_rdy += 8;
	}
	if ( (r&0x0000000f) == 0 )
	{
		r>>=4;
		os_pri_rdy += 4;
	}
	if ( (r&0x00000003) == 0 )
	{
		r>>=2;
		os_pri_rdy += 2;
	}
	if ( (r&0x00000001) == 0 )
	{
		os_pri_rdy += 1;
	}

	os_tcb_rdy	=	&(os_tcb_tbl[os_pri_rdy]);
	OS_LEAVE_CRITICAL();
	OS_TASK_SW();

}


#undef __ZTOS_CORE_C
