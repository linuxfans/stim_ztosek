//////////////////////////////////////////////////////////////////////////
//
//�ļ���dev_timer.h
//���ߣ�����
//���ڣ�[11/18/2005]
//////////////////////////////////////////////////////////////////////////
#ifndef __DEV_TIMER_H
#define __DEV_TIMER_H


//////////////////////////////////////////////////////////////////////////
//EXTREN�궨�塡��ֹ���ض���
//////////////////////////////////////////////////////////////////////////
#ifdef	__DEV_TIMER_C
#define	EXTERN
#else
#define EXTERN	extern
#endif

EXTERN void timer_init(void);
EXTERN void sleep(UINT32 ms);

EXTERN UINT32	os_sys_time;
EXTERN void TIMER0_IRQ(void) ;
EXTERN void TIMER1_IRQ(void) ;
//EXTERN void PlayWav(const unsigned short* wavbuf,UINT32 length,UINT32 _cycle); 
//EXTERN void SetWav(unsigned short* wavbuf);

#undef EXTERN
#endif
