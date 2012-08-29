//////////////////////////////////////////////////////////////////////////
//LPC21xx UART�������� 
//�ļ�: dev_uart.c
//����:	����
//����: [11/20/2005]
//////////////////////////////////////////////////////////////////////////

#define	__DEV_UART_C
#include "ztos.h"
#include "ztos_core.h"

#define SEM_GPRS_LF	0

#ifdef OS_DEV_USE_UART0


UART0_RBUF_ST	uart0_rbuf	=	{ 0, 0, };
UART0_TBUF_ST	uart0_tbuf	=	{ 0, 0, };
UINT32	uart0_tx_rst	=	1;
//////////////////////////////////////////////////////////////////////////
//����0�жϴ�������
//////////////////////////////////////////////////////////////////////////
void uart0_irq_c (void)	 
{
    volatile char dummy;
    volatile char IIR;
    volatile UINT32 n = 0;
    UART0_RBUF_ST *p;

    //����δ�����ж�
    //IIR = U0IIR;
    while (((IIR = U0IIR) & 0x01) == 0)
    {
	switch (IIR & 0x0E)
	{
	case 0x06:		//Line Status
	    dummy = U0LSR;		//���ж�Դ
	    break;

	case 0x04:
	case 0x0C:
	    p = (UART0_RBUF_ST*)&uart0_rbuf;
	    if (((p->in - p->out) & ~(UART0_RBUF_SIZE-1)) == 0)
	    {
		p->buf [p->in & (UART0_RBUF_SIZE-1)] = U0RBR;

		n++;

		p->in++;

	    }
	    break;


	case 0x02:		//����FIFO�п�λ
	    p = (UART0_RBUF_ST*)&uart0_tbuf;
	    if (p->in != p->out)
	    {
		U0THR = p->buf [p->out & (UART0_TBUF_SIZE-1)];
		p->out++;
		uart0_tx_rst = 0;
	    }
	    else
	    {
		uart0_tx_rst = 1;
	    }
	    break;

	default:
	    break;
	}
    }	
    if ( n != 0){
	os_sem_vn(SEM_UART0,n);
	os_pri_rdy	=	FindHighestPrio(os_pri_flg);
	os_tcb_rdy	=	&(os_tcb_tbl[os_pri_rdy]);
    }
    
    VICVectAddr = 0;	//֪ͨ
}
void uart0_init(UINT32 baud, UINT8 datab, UINT8 stopb, UINT8 parity)
{
    volatile UINT32 dummy;
    //��ʼ������������
    uart0_tbuf.in = 0;
    uart0_tbuf.out = 0;
    uart0_tx_rst = 1;

    uart0_rbuf.in = 0;
    uart0_rbuf.out = 0;

    U0LCR = 0x03;					// 8 bits, no Parity, 1 Stop bit
    U0IER = 0;						//�ر�UART0�ж�
    U0FCR = 0x81;					//ʹ��FIFO 8Bytes�ж�
    VICVectAddr6 = (UINT32)UART0_IRQ;
    VICVectCntl6 = 0x20 | 6;
    VICIntEnable |=  ( 1 << 6 ) ;  //ʹ�ܴ��� 0 �ж�

    U0LCR |= 0x80;         
    dummy	=	( 12000000*5/4/16) / baud;
    U0DLL = dummy & 0xff;
    U0DLM = (dummy >> 8);
    U0LCR &= ~0x80;
	
    dummy	=	datab - 5;
    if ( stopb == 2 ) dummy|= 0x04;

    if ( parity != 0 )
    {
	parity--;
	dummy |= 0x08;
    }
    dummy |= ( parity << 4 );

    U0LCR	=	dummy;
	  
    dummy = U0IIR;				//�����ж�
    U0IER = 3;					//ʹ�ܽ��շ����ж�
}
//////////////////////////////////////////////////////////////////////////
//����0�����ַ� ���������뷢�ͻ�������
//////////////////////////////////////////////////////////////////////////
void uart0_putchar(UINT8 c)
{
    UART0_TBUF_ST *p = &uart0_tbuf;
    OS_ENTER_CRITICAL();
    if (uart0_tx_rst)
    {
	uart0_tx_rst = 0;
	U0THR = c;
	OS_LEAVE_CRITICAL();
    }
    else
    {
	p->buf [p->in & (UART0_TBUF_SIZE - 1)] = c;
	p->in++;
	OS_LEAVE_CRITICAL();
    }
    //return (0);
}

void uart0_putchars(INT8* c)
{
    INT8*	p;
    p = c;
    while( *p != 0 )
    {
	uart0_putchar(*p);
	p++;
    }
}

void uart0_putchar_hex(UINT8 c)
{
    UINT8 tmp;
    tmp = (c&0xf0)>>4;
    if(tmp < 0x0a)
	uart0_putchar(0x30+tmp);
    else
	uart0_putchar('A'+(tmp-0x0a));

    tmp = (c&0x0f);
    if(tmp < 0x0a)
	uart0_putchar(0x30+tmp);
    else
	uart0_putchar('A'+(tmp-0x0a));

}
//////////////////////////////////////////////////////////////////////////
//����0�����ַ����������ջ���������ȡ��
//////////////////////////////////////////////////////////////////////////
UINT8 uart0_getchar(void)
{
    UART0_RBUF_ST *p = &uart0_rbuf;

    return (p->buf [(p->out++) & (UART0_RBUF_SIZE - 1)]);
}

void uart0_getline(INT8 *c)
{
    int i = 0;
    do
    {
	c[i] = uart0_getchar_pend();
	i++;
    }while(c[i-1] !=  '\r');
    c[i] = 0;
}
UINT8 uart0_getchar_pend(void)
{
	
    UART0_RBUF_ST *p = &uart0_rbuf;
    if(os_sem_p(SEM_UART0,100000))
	return	(p->buf [(p->out++) & (UART0_RBUF_SIZE - 1)]);
    else
	return 0;
}

#endif


#ifdef OS_DEV_USE_UART1

static	UART1_RBUF_ST	uart1_rbuf	=	{ 0, 0, };
static	UART1_TBUF_ST	uart1_tbuf	=	{ 0, 0, };
static	UINT32	uart1_tx_rst	=	1;	

//////////////////////////////////////////////////////////////////////////
//����1�жϴ�������
//////////////////////////////////////////////////////////////////////////
void uart1_irq_c (void)	
{
    volatile char dummy;
    volatile char IIR;
    volatile UINT32 n = 0;
    volatile UART1_RBUF_ST *p;

    //����δ�����ж�
    while (((IIR = U1IIR) & 0x01) == 0)
    {
	switch (IIR & 0x0E)
	{
	case 0x06:		//Line Status
	    dummy = U1LSR;		//���ж�Դ
	    //uart0_putchar('L');
	    break;

	case 0x04:
	case 0x0C:
	    p = (UART1_RBUF_ST*)&uart1_rbuf;
	    if (((p->in - p->out) & ~(UART1_RBUF_SIZE-1)) == 0)
	    {
		p->buf [p->in & (UART1_RBUF_SIZE-1)] = U1RBR;
		n++;
		p->in++;
	    }
	    break;

	case 0x02:		//����FIFO�п�λ
	    p = (UART1_RBUF_ST*)&uart1_tbuf;
	    if (p->in != p->out)
	    {
		U1THR = p->buf [p->out & (UART1_TBUF_SIZE-1)];
		p->out++;
		uart1_tx_rst = 0;
	    }
	    else
	    {
		uart1_tx_rst = 1;

	    }
	    break;

	case 0x00: //Modem ״̬�ж�
	    dummy = U1MSR; //���ж�Դ
	    break;

	default:
	    break;
	}
    }
    if ( n != 0 )
    {
	os_sem_vn(SEM_UART1,n);
	os_pri_rdy	=	FindHighestPrio(os_pri_flg);
	os_tcb_rdy	=	&(os_tcb_tbl[os_pri_rdy]);
    }
    VICVectAddr = 0;	//֪ͨ
}
//////////////////////////////////////////////////////////////////////////
//����1��ʼ��
//////////////////////////////////////////////////////////////////////////
void uart1_init(UINT32 baud, UINT8 datab, UINT8 stopb, UINT8 parity)
{
    volatile UINT32 dummy;
    //��ʼ������������
    uart1_tbuf.in = 0;
    uart1_tbuf.out = 0;
    uart1_tx_rst = 1;

    uart1_rbuf.in = 0;
    uart1_rbuf.out = 0;

    U1LCR = 0x03;					// 8 bits, no Parity, 1 Stop bit
    U1IER = 0;						//�ر�UART0�ж�
    U1FCR = 0x81;					//ʹ��FIFO 8Bytes�ж�
    VICVectAddr7 = (UINT32)UART1_IRQ;
    VICVectCntl7 = 0x20 | 7;
    VICIntEnable |=  ( 1 << 7 ) ;  //ʹ�ܴ��� 0 �ж�

    U1LCR |= 0x80;         
    dummy	=	( 12000000*5/4/16) / baud;
    U1DLL = dummy & 0xff;
    U1DLM = (dummy >> 8);
    U1LCR &= ~0x80;
	
    dummy	=	datab - 5;
    if ( stopb == 2 ) dummy|= 0x04;

    if ( parity != 0 )
    {
	parity--;
	dummy |= 0x08;
    }
    dummy |= ( parity << 4 );

    U1LCR	=	dummy;
	  
    dummy = U1IIR;				//�����ж�
    U1IER = 3;					//ʹ�ܽ��շ����ж�
}


//////////////////////////////////////////////////////////////////////////
//����1�����ַ����������뷢�ͻ�������
//////////////////////////////////////////////////////////////////////////
void uart1_putchar(UINT8 c)
{
    UART1_TBUF_ST *p = &uart1_tbuf;

    OS_ENTER_CRITICAL();
    if (uart1_tx_rst)
    {
	uart1_tx_rst = 0;

	U1THR = c;
	OS_LEAVE_CRITICAL();
    }
    else
    {
	p->buf [p->in & (UART1_TBUF_SIZE - 1)] = c;
	p->in++;

    }
    OS_LEAVE_CRITICAL();
}


//////////////////////////////////////////////////////////////////////////
//����1�����ַ����������ջ���������ȡ��
//////////////////////////////////////////////////////////////////////////
UINT8 uart1_getchar(void)
{
    UART1_RBUF_ST *p = &uart1_rbuf;

    return	(p->buf [(p->out++) & (UART1_RBUF_SIZE - 1)]);
}

void uart1_getline(UINT8 *c)
{
    int i = 0;
    do
    {
	c[i] = uart1_getchar();
	i++;
    }while(c[i-1] != '\n');
    c[i] = 0;
}
void uart1_putchars(UINT8* c)
{
    UINT8*	p;
    p = c;
    while( *p != 0 )
    {
	uart1_putchar(*p);
	p++;
    }
}
UINT8 uart1_getchar_pend(void)
{
	
    UART1_RBUF_ST *p = &uart1_rbuf;
    if(os_sem_p(SEM_UART1,100000))
	return	(p->buf [(p->out++) & (UART1_RBUF_SIZE - 1)]);
    else
	return 0;
}
#endif