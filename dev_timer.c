
#define __DEV_TIMER_C
#include "ztos.h"

#define	SEM_TIMER_SLEEP	5
UINT32 cycle;

void timer0_irq_c (void)	 
{

    UINT8 i = 0;


    os_sys_time++;

    for ( i = 0 ; i < OS_SEM_MAXNUM ; i++ )
    {
	// someone waiting
	if ( os_scb_tbl[i].os_scb_cnt < 0 )
	{
	    // system timer interval is 1ms 
	    os_scb_tbl[i].os_scb_timer --;
	    if ( os_scb_tbl[i].os_scb_timer <= 0 )
	    {
		os_scb_tbl[i].os_scb_timer	=	0;
		// time out, wakeup the waiting thread
		os_sem_vn( i, 1 );
	    } 
	}

    }
    /*
      if ( end_time != start_time && os_sys_time >= end_time )
      {	
      end_time = start_time;
      os_sem_vn(SEM_TIMER_SLEEP,1);
      } */
    T0IR = 0x01;
    VICVectAddr = 0x00;
	
}


void timer_init(void)
{
    os_sys_time = 0;

    T0TC	=	0;
    T0PR	=	0;
    T0MCR	=	0x03;
    T0MR0	=	150000-1;		//10ms

    T0TCR	=	0x01;

    VICVectCntl4	=	0x20 | 0x04;
    VICVectAddr4	=	(UINT32)TIMER0_IRQ;
    VICIntEnable	|=	1 << 0x04;	
						  
	
}
#undef __DEV_TIMER_C
