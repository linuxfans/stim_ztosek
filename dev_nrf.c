//////////////////////////////////////////////////////////////////////////
//LPC21xx CAN驱动程序 
//文件: dev_can.c
//作者:	张鑫
//日期: [08/16/2006]
//////////////////////////////////////////////////////////////////////////
#define	__DEV_NRF_C
#include "ztos.h"

UINT8 flag = 0;

const UINT8 nrf9e5_4_config[10] = {	
	0x28,			//频道设置

	0x0c,			//自动重发关，发送节电模式关，输出功率10dBm，433MHZ
				
	0x44,			//收发地址都为1字节
				
	0x20,			//接收数据长度，32字节
				
	0x20,			//发送数据长度，32字节
				
	0xe7,
	0xe7,
	0xe7,
	0xe7,			//地址
							
	0x50			//CRC开，16位校验，16M晶振，外部时钟使能500KHZ输出
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
		spi_read_write(c[i]);//写入发送数据缓冲区
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
	  uart0_putchar(spi_read_write(c[i]));//写入发送数据缓冲区
	}
	IOSET0= 1 << RACSN4;
  */
					
	for	( j = 0 ; j < 100000 ; j++ );

	//IOCLR0 = 1 << TXEN4;
	//while((IOPIN & ( 1 << DR4)) == 0);
			
}


//////////////////////////////////////////////////////////////////////////
//nRF905 433MHz 中断处理函数
//////////////////////////////////////////////////////////////////////////
void nrf905_4_irq_c (void)
{
	if (flag == 1) {
		flag = 0;
	} else {
		os_sem_vn(OS_SEM_N433,1);
	}
	EXTINT	=	( 1 << 1 );
	VICVectAddr = 0;	//通知
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
