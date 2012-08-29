//////////////////////////////////////////////////////////////////////////
//LPC21xx CAN�������� 
//�ļ�: dev_can.c
//����:	����
//����: [08/16/2006]
//////////////////////////////////////////////////////////////////////////
#define	__DEV_NRF_C
#include "ztos.h"

UINT8 flag = 0;

const UINT8 nrf9e5_4_config[10] = {	
	0x28,			//Ƶ������

	0x0c,			//�Զ��ط��أ����ͽڵ�ģʽ�أ��������10dBm��433MHZ
				
	0x44,			//�շ���ַ��Ϊ1�ֽ�
				
	0x20,			//�������ݳ��ȣ�32�ֽ�
				
	0x20,			//�������ݳ��ȣ�32�ֽ�
				
	0xe7,
	0xe7,
	0xe7,
	0xe7,			//��ַ
							
	0x50			//CRC����16λУ�飬16M�����ⲿʱ��ʹ��500KHZ���
};

UINT8 spi_read_write(UINT8 data)
{
	//while( (S0SPSR & 0x01) == 0 );			 
	SSPDR	=	data;
 
	while((SSPSR & 0x10 ) != 0 );
	//delay();	
	return SSPDR;
}
const int HEX_TABLE[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
void char2hex(UINT8 *c, UINT8* p, UINT8 n)
{
	UINT8 i;
	for ( i = 0 ; i < n ; i++ )
	{
		p[i*2] = HEX_TABLE[c[i]>>4 & 0x0f];
		p[i*2+1] = HEX_TABLE[c[i] & 0x0f];
	}
}

void nrf905_4_send(unsigned char *c)
{
    //TRX_CE = 1;
    //Delay400us(1);
    //TRX_CE = 0;
//	UINT8 buf[2];
//	UINT8 tmp;
	UINT32 j;	
	UINT8 i;	
	flag = 1;		
	    	 		
						   
	IOSET0 = 1 << TXEN4;
	IOCLR0 = 1 << RACSN4;
	spi_read_write(WTP);
	for( i = 0 ; i < nrf9e5_4_config[4] ; i++ )
	{
		spi_read_write(c[i]);//д�뷢�����ݻ�����
	}
	IOSET0= 1 << RACSN4;
    	
	//IOSET0 = 1 << TXEN4;
	IOSET0 = 1 << CE4;				 
	for	( j = 0 ; j < 10000 ; j++ );
	//delay
	IOCLR0 = 1 << CE4;
 /*
	IOCLR0 = 1 << RACSN4;
	spi_read_write(RTP);
	for( i = 0 ; i < nrf9e5_4_config[4] ; i++ ){
	  uart0_putchars("\r\nRTP:");
	  uart0_putchar(spi_read_write(c[i]));//д�뷢�����ݻ�����
	}
	IOSET0= 1 << RACSN4;
  */
					
	for	( j = 0 ; j < 100000 ; j++ );

	//IOCLR0 = 1 << TXEN4;
	//while((IOPIN & ( 1 << DR4)) == 0);
			
}


//////////////////////////////////////////////////////////////////////////
//nRF905 433MHz �жϴ�����
//////////////////////////////////////////////////////////////////////////
void nrf905_4_irq_c (void)
{
	if (flag == 1) {
		flag = 0;
	} else {
		os_sem_vn(OS_SEM_N433,1);
	}
	EXTINT	=	( 1 << 1 );
	VICVectAddr = 0;	//֪ͨ
}



void nrf905_4_init()
{
	UINT8	i;
	// Configure Radio:
	
	IOCLR0	= 1 << CE4;
	IOCLR0	= 1 << TXEN4;			
	IOCLR0 = 1 << RACSN4;	
	spi_read_write(WRC);	
	for( i = 0; i < 10; i++ )
	{
		spi_read_write(nrf9e5_4_config[i]); 
	}
	IOSET0 = 1 << RACSN4;
	
	VICVectAddr2 = (UINT32)NRF905_4_IRQ;
	VICVectCntl2 = 0x20 | 15;
}


#undef	__DEV_NRF_C	

//#endif
