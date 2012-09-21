//////////////////////////////////////////////////////////////////////////
//ZtOS	����
//�ļ���ztos_core.c
//���ߣ�����
//���ڣ�[11/18/2005]
//////////////////////////////////////////////////////////////////////////
#define	__ZTOS_CORE_C
#include "ztos.h"




//////////////////////////////////////////////////////////////////////////
//Ѱ��������ȼ�������ԭ�ͣ�Ӧ����Ч���������������
//������
//���ߣ�����
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
//����ϵͳ��ʼ��
//��������ʼ��ϵͳȫ�ֱ���
//		����IDLE����
//���ߣ�����
//���ڣ�[11/18/2005]
//////////////////////////////////////////////////////////////////////////
void os_init()
{
	UINT32	i;
	//��ʼ��ȫ�ֱ���
	os_running		=	FALSE;
	os_pri_flg		=	0;
	os_pri_rdy		=	0;
	os_pri_cur		=	0;
	os_tcb_rdy		=	(OS_TCB*) NULL;
	os_tcb_cur		=	(OS_TCB*) NULL;

	//��ʼ���ź���
	for ( i = 0 ; i < OS_SEM_MAXNUM ; i++ )
	{
		os_scb_tbl[i].os_scb_cnt	=	0;
		os_scb_tbl[i].os_scb_wat_tsk	=	OS_TSK_MAXNUM;
	}

	//���� IDEL ����
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
//��������
//��������ʼ�������ջ
//		��ʼ������TCB
//���ߣ�����
//���ڣ�[8/14/2005]
//////////////////////////////////////////////////////////////////////////
void  os_tsk_create (void (*task)(void *pd), void *pdata, OS_TSK_STK *ptos, OS_PRI pri)
{
	OS_TSK_STK*	psp;

	//��ʼ�������ջ
	psp = (OS_TSK_STK *)os_tsk_stk_init(task, pdata, ptos);//, 0);	

	//��ʼ������TCB
	os_tcb_init(pri, psp);

}
//////////////////////////////////////////////////////////////////////////
//��ʼ������TCB
//��������ȡ����TCB
//		����TCB����
//		��ʼ����Ա����
//���ߣ�����
//���ڣ�[8/14/2005]
//////////////////////////////////////////////////////////////////////////
void os_tcb_init(OS_PRI pri, OS_TSK_STK* ptos)
{
	OS_ENTER_CRITICAL();

	(os_tcb_tbl[pri]).os_tcb_stk_ptr	=	ptos;
	(os_tcb_tbl[pri]).os_tcb_pri		=	pri;


	//��os_pri_flg�Ǽ�
	os_pri_flg	|=	(1 << pri);
	OS_LEAVE_CRITICAL();
}

//////////////////////////////////////////////////////////////////////////
//IDLE����
//���������м�������
//���ߣ�����
//���ڣ�[8/14/2005]
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
//�ź���P���������жϳ�ʱ
//��ʱ����0�����򷵻����ʱʣ��ʱ��
//���ߣ�����
//  [7/1/2006]
//////////////////////////////////////////////////////////////////////////
INT32 os_sem_p(OS_SEM SemID, UINT32 timeout)
{
	OS_ENTER_CRITICAL();
	os_scb_tbl[SemID].os_scb_timer	=	timeout / 10;
	os_scb_tbl[SemID].os_scb_cnt--;

	//���㹻��Դ������
	if ( os_scb_tbl[SemID].os_scb_cnt < 0)
	{
		os_scb_tbl[SemID].os_scb_wat_tsk	=	os_pri_cur;
		os_pri_flg		&=	~(1<<os_pri_cur);
		OS_LEAVE_CRITICAL();
		//��Ȼ���������л�
		os_tsk_sch();
	}
	
	OS_LEAVE_CRITICAL();
	return  os_scb_tbl[SemID].os_scb_timer;
}
//////////////////////////////////////////////////////////////////////////
//�ź���try���������жϳ�ʱ
//��ʱ����0�����򷵻����ʱʣ��ʱ��
//���ߣ�����
//  [7/1/2006]
//////////////////////////////////////////////////////////////////////////
INT32 os_sem_try(OS_SEM SemID)
{
	OS_ENTER_CRITICAL();

	//���㹻��Դ��ֱ�ӷ���ʧ��
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
�ݲ��ṩ�˹���
//////////////////////////////////////////////////////////////////////////
//�ź���P����n��
//���ߣ�����
//  [9/1/2005]
//////////////////////////////////////////////////////////////////////////
void os_sem_pn(OS_SEM SemID, INT32 n)
{
	OS_ENTER_CRITICAL();
	os_scb_tbl[SemID].os_scb_cnt		-=	n;

	//���㹻��Դ������
	if ( os_scb_tbl[SemID].os_scb_cnt < 0)
	{
		os_scb_tbl[SemID].os_scb_wat_tsk	=	os_pri_cur;
		os_pri_flg		&=	~(1<<os_pri_cur);
		OS_LEAVE_CRITICAL();
		//��Ȼ���������л�
		os_tsk_sch();
	}
	OS_LEAVE_CRITICAL();

}
*/

//////////////////////////////////////////////////////////////////////////
//�ź���V����n��
//���ߣ�����
//  [9/1/2005]
//////////////////////////////////////////////////////////////////////////
void os_sem_vn(OS_SEM SemID, INT32 n)
{
	os_scb_tbl[SemID].os_scb_cnt		+=	n;

	//��Դ�㹻����������������
	if ( (os_scb_tbl[SemID].os_scb_cnt) >= 0 &&
		os_scb_tbl[SemID].os_scb_wat_tsk < OS_SEM_MAXNUM) 
	{
		os_pri_flg		|=	( 1 << os_scb_tbl[SemID].os_scb_wat_tsk );
		//������������ȼ����ھ���������ȼ�����
		//������ȼ�������жϼ��������л�ͨ���ж�
		//OSPrioHighRdy == OSPrioCur�ж��Ƿ���������л�
		if ( os_scb_tbl[SemID].os_scb_wat_tsk < os_pri_rdy )
		{
			os_pri_rdy	=	os_scb_tbl[SemID].os_scb_wat_tsk;
			os_tcb_rdy	=	&(os_tcb_tbl[os_pri_rdy]);
		}
		//��֪����û�б���ǰ�ź�������
		os_scb_tbl[SemID].os_scb_wat_tsk	=	OS_SEM_MAXNUM;
	}
}

//////////////////////////////////////////////////////////////////////////
//�������
//����������������ȼ��������񣨽�����������ȼ���������չ����
//		���������л����������жϣ�ϵͳ���ã�
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
