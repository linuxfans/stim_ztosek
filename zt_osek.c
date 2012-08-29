#include "ztos.h"
#define	TSK0_STK_SIZE	256
#define	TSK1_STK_SIZE	256
#define	TSK2_STK_SIZE	256

OS_TSK_STK	tsk0_stk[TSK0_STK_SIZE];
OS_TSK_STK	tsk1_stk[TSK1_STK_SIZE];
OS_TSK_STK	tsk2_stk[TSK2_STK_SIZE];

void tsk0(void *pd)
{
    pd = pd;			
				
    uart0_putchars("hello\r\n");
    for( ; ; )
    {					 
	os_sem_p(SEM_UART0,100000);
	uart1_putchar(uart0_getchar());
    }

}

void tsk1(void *pd)						 //eeprom and rfinsert check
{
    int i;
    pd = pd;
    for( ; ; )
    {  
	os_sem_p(SEM_UART1,100000);
	uart0_putchar(uart1_getchar());
    }
}

void tsk2(void *pd)						 //eeprom and rfinsert check
{
    int i;
    pd = pd;
    for( ; ; )
    {
	os_sem_p(SEM_TSK2,2);
    }
}

void pwm_init() 
{
  PWMTC = 0;
  PWMPR = 0;
  PWMMCR = 0x02;
  PWMMR0 = 20;
  PWMMR5 = 10;
  PWMPCR = 0x2000;
  PWMTCR = 0x09;
  DACR = 256 << 6;
}

	

int main()
{
    PINSEL0 = 0x00050005;		  
    PINSEL1 = 0x15480400;
    PINSEL2 = 0x00000000;

    pwm_init();
    timer_init();
    uart0_init(38400, 8 , 1, 0);	
    uart1_init(38400, 8 , 1, 0);	
    //nrf905_4_init();	
    os_init();
    os_tsk_create(tsk0,NULL,&(tsk0_stk[TSK0_STK_SIZE-1]),0);  
    os_tsk_create(tsk1,NULL,&(tsk1_stk[TSK1_STK_SIZE-1]),1);
    os_tsk_create(tsk2,NULL,&(tsk2_stk[TSK2_STK_SIZE-1]),2); 
    OS_LEAVE_CRITICAL();
    os_start();
    while(1);
}