#ifndef _ZTOS_H
#define _ZTOS_H
#define	FALSE	0
#define TRUE	1
#define	NULL	0


#define OS_DEV_USE_UART0	  1
#define OS_DEV_USE_UART1	  1
//#define OS_DEV_USE_UART2	  1
//#define OS_DEV_USE_UART3	  1
#define OS_DEV_USE_TIMER0	  1
#define OS_DEV_USE_TIMER1	  1
//#define OS_DEV_USE_PWM		  1




#define SEM_TSK0 			1
#define SEM_TSK1			2
#define SEM_TSK2			3
#define SEM_TSK3			4
#define SEM_TSK4			5
#define SEM_TSK5			6
#define SEM_UART0			7
#define SEM_UART1			8
					  
#include "ztos_cpu.h"
//#include "ztos_config.h"
#include <lpc213x.h>
#include "ztos_device.h"
#include "ztos_core.h"
#endif
