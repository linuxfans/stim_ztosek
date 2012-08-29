//////////////////////////////////////////////////////////////////////////
//LPC21xx UART驱动程序 
//文件: dev_uart.h
//作者:	张鑫
//日期: [11/20/2005]
//////////////////////////////////////////////////////////////////////////
#ifndef __DEV_UART_H
#define __DEV_UART_H

//////////////////////////////////////////////////////////////////////////
//EXTREN宏定义　防止多重定义
//////////////////////////////////////////////////////////////////////////
#ifdef	__DEV_UART_C
#define	EXTERN
#else
#define EXTERN	extern
#endif

//////////////////////////////////////////////////////////////////////////
//UART0
#ifdef OS_DEV_USE_UART0
//////////////////////////////////////////////////////////////////////////
//缓冲区大小定义
//////////////////////////////////////////////////////////////////////////
#define UART0_TBUF_SIZE   256		//要求：2 的整次幂
#define UART0_RBUF_SIZE   1024		//要求：2 的整次幂
//////////////////////////////////////////////////////////////////////////
//检查缓冲区大小设置
//////////////////////////////////////////////////////////////////////////
#if UART0_TBUF_SIZE < 2
#error UART0_TBUF_SIZE is too small.  It must be larger than 1.
#elif ((UART0_TBUF_SIZE & (UART0_TBUF_SIZE-1)) != 0)
#error UART0_TBUF_SIZE must be a power of 2.
#endif

#if UART0_RBUF_SIZE < 2
#error UART0_RBUF_SIZE is too small.  It must be larger than 1.
#elif ((UART0_RBUF_SIZE & (UART0_RBUF_SIZE-1)) != 0)
#error UART0_RBUF_SIZE must be a power of 2.
#endif

//////////////////////////////////////////////////////////////////////////
//缓冲区结构体设置
//////////////////////////////////////////////////////////////////////////
typedef struct u0_rbuf_st
{
	UINT8 in;							//输入
	UINT8 out;							//输出
	UINT8  buf [UART0_RBUF_SIZE];		//缓冲区空间
}UART0_RBUF_ST;

typedef struct u0_tbuf_st
{
	UINT8 in;							//输入
	UINT8 out;							//输出
	UINT8  buf [UART0_TBUF_SIZE];		//缓冲区空间
}UART0_TBUF_ST;

//////////////////////////////////////////////////////////////////////////
//缓冲区初始化
//////////////////////////////////////////////////////////////////////////
#define	UART0_RBUFLEN	((UINT16)(uart0_rbuf.in - uart0_rbuf.out))
#define	UART0_TBUFLEN	((UINT16)(uart0_tbuf.in - uart0_tbuf.out))

//////////////////////////////////////////////////////////////////////////
//发送启动标志，取决于LPC21xx串口终端发送特性 参阅LPC21xx Datasheet
//////////////////////////////////////////////////////////////////////////

EXTERN	void uart0_init(UINT32 baud, UINT8 datab, UINT8 stopb, UINT8 parity);
EXTERN	void uart0_putchar(UINT8 c);

EXTERN	void uart0_getline(INT8 *c);
EXTERN	void uart0_putchars(INT8* c);
EXTERN	UINT8	uart0_getchar();	
EXTERN  UINT8 uart0_getchar_pend(void);
EXTERN  void uart0_putchar_hex(UINT8 c);
extern	void UART0_IRQ();
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////



#endif






//////////////////////////////////////////////////////////////////////////
//UART1
#ifdef OS_DEV_USE_UART1
//////////////////////////////////////////////////////////////////////////
//缓冲区大小定义
//////////////////////////////////////////////////////////////////////////
#define UART1_TBUF_SIZE   256		//要求：2 的整次幂
#define UART1_RBUF_SIZE   2048		//要求：2 的整次幂
//////////////////////////////////////////////////////////////////////////
//检查缓冲区大小设置
//////////////////////////////////////////////////////////////////////////
#if UART1_TBUF_SIZE < 2
#error UART1_TBUF_SIZE is too small.  It must be larger than 1.
#elif ((UART1_TBUF_SIZE & (UART1_TBUF_SIZE-1)) != 0)
#error UART1_TBUF_SIZE must be a power of 2.
#endif

#if UART1_RBUF_SIZE < 2
#error UART1_RBUF_SIZE is too small.  It must be larger than 1.
#elif ((UART1_RBUF_SIZE & (UART1_RBUF_SIZE-1)) != 0)
#error UART1_RBUF_SIZE must be a power of 2.
#endif

//////////////////////////////////////////////////////////////////////////
//缓冲区结构体设置
//////////////////////////////////////////////////////////////////////////
typedef struct u1_rbuf_st
{
	UINT32 in;							//输入
	UINT32 out;							//输出
	UINT8  buf [UART1_RBUF_SIZE];		//缓冲区空间
}UART1_RBUF_ST;

typedef struct u1_tbuf_st
{
	UINT32 in;							//输入
	UINT32 out;							//输出
	UINT8  buf [UART1_TBUF_SIZE];		//缓冲区空间
}UART1_TBUF_ST;

//////////////////////////////////////////////////////////////////////////
//缓冲区初始化
//////////////////////////////////////////////////////////////////////////
#define	UART1_RBUFLEN	((UINT16)(uart1_rbuf.in - uart1_rbuf.out))
#define	UART1_TBUFLEN	((UINT16)(uart1_tbuf.in - uart1_tbuf.out))

//////////////////////////////////////////////////////////////////////////
//发送启动标志，取决于LPC21xx串口终端发送特性 参阅LPC21xx Datasheet
//////////////////////////////////////////////////////////////////////////
EXTERN	void uart1_init(UINT32 baud, UINT8 datab, UINT8 stopb, UINT8 parity);
EXTERN	void uart1_putchar(UINT8 c); 
EXTERN	void uart1_putchars(UINT8* c);   
EXTERN	UINT8	uart1_getchar();
EXTERN	void uart1_getline(UINT8 *c);
EXTERN	UINT8 uart1_getchar_pend(void);
	
extern	void UART1_IRQ() ;
#endif




//////////////////////////////////////////////////////////////////////////
//UART2
#ifdef OS_DEV_USE_UART2
//////////////////////////////////////////////////////////////////////////
//缓冲区大小定义
//////////////////////////////////////////////////////////////////////////
#define UART2_TBUF_SIZE   1024		//要求：2 的整次幂
#define UART2_RBUF_SIZE   1024		//要求：2 的整次幂
//////////////////////////////////////////////////////////////////////////
//检查缓冲区大小设置
//////////////////////////////////////////////////////////////////////////
#if UART2_TBUF_SIZE < 2
#error UART2_TBUF_SIZE is too small.  It must be larger than 1.
#elif ((UART2_TBUF_SIZE & (UART2_TBUF_SIZE-1)) != 0)
#error UART2_TBUF_SIZE must be a power of 2.
#endif

#if UART2_RBUF_SIZE < 2
#error UART2_RBUF_SIZE is too small.  It must be larger than 1.
#elif ((UART2_RBUF_SIZE & (UART2_RBUF_SIZE-1)) != 0)
#error UART2_RBUF_SIZE must be a power of 2.
#endif

//////////////////////////////////////////////////////////////////////////
//缓冲区结构体设置
//////////////////////////////////////////////////////////////////////////
typedef struct u2_rbuf_st
{
	UINT32 in;							//输入
	UINT32 out;							//输出
	UINT8  buf [UART2_RBUF_SIZE];		//缓冲区空间
}UART2_RBUF_ST;

typedef struct u2_tbuf_st
{
	UINT32 in;							//输入
	UINT32 out;							//输出
	UINT8  buf [UART2_TBUF_SIZE];		//缓冲区空间
}UART2_TBUF_ST;

//////////////////////////////////////////////////////////////////////////
//缓冲区初始化
//////////////////////////////////////////////////////////////////////////
#define	UART2_RBUFLEN	((UINT16)(uart2_rbuf.in - uart2_rbuf.out))
#define	UART2_TBUFLEN	((UINT16)(uart2_tbuf.in - uart2_tbuf.out))

//////////////////////////////////////////////////////////////////////////
//发送启动标志，取决于LPC21xx串口终端发送特性 参阅LPC21xx Datasheet
//////////////////////////////////////////////////////////////////////////
EXTERN	void uart2_init(UINT32 baud, UINT8 datab, UINT8 stopb, UINT8 parity);
EXTERN	void uart2_putchar(UINT8 c); 
EXTERN	void uart2_putchars(UINT8* c);   
EXTERN	UINT8	uart2_getchar();
EXTERN	void uart2_getline(UINT8 *c);
EXTERN	UINT8 uart2_getchar_pend(void);
	
extern	void UART2_IRQ() ;
#endif



//////////////////////////////////////////////////////////////////////////
//UART3
#ifdef OS_DEV_USE_UART3
//////////////////////////////////////////////////////////////////////////
//缓冲区大小定义
//////////////////////////////////////////////////////////////////////////
#define UART3_TBUF_SIZE   256		//要求：2 的整次幂
#define UART3_RBUF_SIZE   256		//要求：2 的整次幂
//////////////////////////////////////////////////////////////////////////
//检查缓冲区大小设置
//////////////////////////////////////////////////////////////////////////
#if UART3_TBUF_SIZE < 2
#error UART3_TBUF_SIZE is too small.  It must be larger than 1.
#elif ((UART3_TBUF_SIZE & (UART3_TBUF_SIZE-1)) != 0)
#error UART3_TBUF_SIZE must be a power of 2.
#endif

#if UART3_RBUF_SIZE < 2
#error UART3_RBUF_SIZE is too small.  It must be larger than 1.
#elif ((UART3_RBUF_SIZE & (UART3_RBUF_SIZE-1)) != 0)
#error UART3_RBUF_SIZE must be a power of 2.
#endif

//////////////////////////////////////////////////////////////////////////
//缓冲区结构体设置
//////////////////////////////////////////////////////////////////////////
typedef struct u3_buf_st
{
	UINT32 in;							//输入
	UINT32 out;							//输出
	UINT8  buf [UART3_RBUF_SIZE];		//缓冲区空间
}UART3_RBUF_ST;

typedef struct u3_tbuf_st
{
	UINT32 in;							//输入
	UINT32 out;							//输出
	UINT8  buf [UART3_TBUF_SIZE];		//缓冲区空间
}UART3_TBUF_ST;

//////////////////////////////////////////////////////////////////////////
//缓冲区初始化
//////////////////////////////////////////////////////////////////////////
#define	UART3_RBUFLEN	((UINT16)(uart3_rbuf.in - uart3_rbuf.out))
#define	UART3_TBUFLEN	((UINT16)(uart3_tbuf.in - uart3_tbuf.out))

//////////////////////////////////////////////////////////////////////////
//发送启动标志，取决于LPC21xx串口终端发送特性 参阅LPC21xx Datasheet
//////////////////////////////////////////////////////////////////////////
EXTERN	void uart3_init(UINT32 baud, UINT8 datab, UINT8 stopb, UINT8 parity);
EXTERN	void uart3_putchar(UINT8 c); 
EXTERN	void uart3_putchars(UINT8* c);   
EXTERN	UINT8	uart3_getchar();
EXTERN	UINT8 uart3_getchar_pend(void);

EXTERN	void uart3_getline(UINT8 *c);	
extern	void UART3_IRQ() ;
#endif




#undef	EXTERN

#endif
