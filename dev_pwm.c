//////////////////////////////////////////////////////////////////////////
//LPC21xx PWM驱动程序 
//文件: dev_pwm.c
//作者:	张鑫
//日期: [08/17/2006]
//////////////////////////////////////////////////////////////////////////
#define	__DEV_PWM_C
#include "ztos.h"


void pwm_irq_c(void) __arm
{
	PWMIR       |= 0x00000001;               /* Clear match0 interrupt */
	VICVectAddr  = 0x00000000;
}

void pwm_init()
{

  	VICVectAddr8 = (UINT32)PWM_IRQ; 
	VICVectCntl8 = 0x28;            
	VICIntEnable |= 1 << 28;              

  PWMPR    = 0x00000000;                    /* Load prescaler  */
  
  PWMPCR = 1 << 4 | 1 << 6 | 1 << 12 | 1 << 14;//0x00000C0C;                      /* PWM channel 2 & 3 double edge control, output enabled */
  PWMMCR = 0x00000002;                      /* On match with timer reset the counter */
  PWMMR0 = 0x876;                           /* set cycle rate to sixteen ticks       */
  PWMMR3	=	0x00;
  PWMMR4 = 0x00;                               /* set rising  edge of PWM2 to 100 ticks    */
  PWMMR5	=	0x43B;
  PWMMR6 = 0x43B;                           /* set falling edge of PWM2 to 200 ticks   */
  PWMLER = 1 << 0 | 1 << 3 |1 << 4 |1 << 5 | 1 << 6;                             /* enable shadow latch for match 1 - 3   */ 
  PWMTCR = 0x00000002;                      /* Reset counter and prescaler           */ 
  PWMTCR = 0x00000009;                      /* enable counter and PWM, release counter from reset */

}

#undef	__DEV_PWM_C	
