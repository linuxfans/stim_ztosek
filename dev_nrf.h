//////////////////////////////////////////////////////////////////////////
//LPC21xx nRF驱动程序 
//文件: dev_nrf.h
//作者:	张鑫
//日期: [01/26/2007]
//////////////////////////////////////////////////////////////////////////

#ifndef __DEV_NRF_H
#define __DEV_NRF_H

//////////////////////////////////////////////////////////////////////////
//EXTREN宏定义　防止多重定义
//////////////////////////////////////////////////////////////////////////
#ifdef	__DEV_NRF_C
#define	EXTERN
#else
#define EXTERN	extern
#endif

// SPI Commands
#define WRC     0x00 //W_RF_CONFIG
#define RRC     0x10 //R_RF_CONFIG
#define WTP     0x20 //W_TX_PAYLOAD
#define RTP     0x21 //R_TX_PAYLOAD
#define WTA     0x22 //W_TX_ADDRESS
#define RTA     0x23 //R_TX_ADDRESS
#define RRP     0x24 //R_RX_PAYLOAD
#define RAD     0x40 //R_ADC_DATA 
#define WAC     0x44 //W_ADC_CONFIG
#define RAC     0x46 //R_ADC_CONFIG
#define WTU     0x50 //W_TEST_UNLOCK (use with data A5)
#define WTR     0x52 //W_TEST_REGISTER
#define RTR     0x53 //R_TEST_REGISTER
#define CC      0x80 //CHANNEL_CONFIG
#define SAV     0xC0 //START_ADC_CONV
#define NOP     0xFF //NO_OP

#define HFREQ	0

#define POWER	3




#define RACSN4	16		// SPI Sel
#define PWR4	21		// Power 
#define CE4		22		// Chip Enable
#define TXEN4	28		// Transmitte Enable

#define RACSN8	24		// SPI Sel
#define AM8		17		// Address Match
#define CD8		18		// Carrier Detected
#define PWR8	19		// Power 
#define CE8		20		// Chip Enable
#define TXEN8	21		// Transmitte Enable



EXTERN	void nrf905_4_init();	
EXTERN	void nrf905_8_init();		   
EXTERN	void nrf905_4_send(unsigned char *c);
EXTERN	void nrf905_8_send(unsigned char *c);
EXTERN	UINT8 spi_read_write(UINT8 data);
extern	void NRF905_4_IRQ();
extern	void NRF905_8_IRQ();

#undef EXTERN
#endif
