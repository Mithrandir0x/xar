/************************************************************************************

  Filename:     hal.h

  Description:  This file contains MCU definitons for the MSP430F1611.

************************************************************************************/

#ifndef HAL_H
#define HAL_H


/************************************************************************************
	SPI: Low level functions
************************************************************************************/

#define	SPI_WAITFOREOTx()	while ((U0TCTL & TXEPT) == 0)  // USART0 Tx buffer ready?
#define	SPI_WAITFOREORx()	while ((IFG1 & URXIFG0) == 0)  // USART0 Rx buffer ready?
// LPM4 Mode
#define __SR_GIE     (1<<3)
#define __SR_CPU_OFF (1<<4)
#define __SR_OSC_OFF (1<<5)
#define __SR_SCG0    (1<<6)
#define __SR_SCG1    (1<<7)

// LPM4 Mode
#define __low_power_mode_4()                    \
  (__bis_SR_register(  __SR_GIE                 \
                     | __SR_CPU_OFF             \
                     | __SR_SCG0                \
                     | __SR_SCG1                \
                     | __SR_OSC_OFF))
                     
                     
                     
#define __low_power_mode_3()                    \
  (__bis_SR_register(  __SR_GIE                 \
                     | __SR_CPU_OFF             \
                     | __SR_SCG0                \
                     | __SR_SCG1))
                     
 // Exit Power Mode                    

#define __low_power_mode_off_on_exit()          \
  (__bic_SR_register_on_exit(  __SR_CPU_OFF     \
                             | __SR_SCG0        \
                             | __SR_SCG1        \
                             | __SR_OSC_OFF))

#define FASTSPI_TX(x)\
	do {\
		U0TXBUF = x;\
		SPI_WAITFOREOTx();\
	} while(0)

#define FASTSPI_RX(x)\
    do {\
        U0TXBUF = 0;\
	    SPI_WAITFOREORx();\
		x = U0RXBUF;\
    } while(0)

#define FASTSPI_RX_GARBAGE()\
	do {\
    	U0TXBUF = 0;\
		SPI_WAITFOREORx();\
		U0RXBUF;\
	} while(0)

#define FASTSPI_TX_MANY(p,c)\
	do {\
		for (UINT8 spiCnt = 0; spiCnt < (c); spiCnt++) {\
			FASTSPI_TX(((UINT8*)(p))[spiCnt]);\
		}\
	} while(0)


#define FASTSPI_RX_WORD(x)\
	 do {\
	    U0TXBUF = 0;\
        SPI_WAITFOREORx();\
		x = U0RXBUF << 8;\
	    U0TXBUF = 0;\
		SPI_WAITFOREORx();\
		x |= U0RXBUF;\
    } while (0)

#define FASTSPI_TX_ADDR(a)\
	 do {\
		  U0TXBUF = a;\
		  SPI_WAITFOREOTx();\
	 } while (0)

#define FASTSPI_RX_ADDR(a)\
	 do {\
		  U0TXBUF = (a) | 0x40;\
		  SPI_WAITFOREOTx();\
	 } while (0)



/***********************************************************
	FAST SPI: Radio register access
***********************************************************/
// 	  s = command strobe
// 	  a = register address
// 	  v = register value

#define FASTSPI_STROBE(s) \
    do {\
		  SPI_ENABLE();\
		  FASTSPI_TX_ADDR(s);\
		  SPI_DISABLE();\
    } while (0)

#define FASTSPI_SETREG(a,v)\
	 do {\
		  SPI_ENABLE();\
		  FASTSPI_TX_ADDR(a);\
		  FASTSPI_TX((UINT8) ((v) >> 8));\
		  FASTSPI_TX((UINT8) (v));\
		  SPI_DISABLE();\
	 } while (0)


#define FASTSPI_GETREG(a,v)\
	 do {\
		  SPI_ENABLE();\
		  FASTSPI_RX_ADDR(a);\
		  v= (UINT8)U0RXBUF;\
		  FASTSPI_RX_WORD(v);\
		  halWait(1);\
		  SPI_DISABLE();\
	 } while (0)

// Updates the SPI status UINT8

#define FASTSPI_UPD_STATUS(s)\
	 do {\
		  SPI_ENABLE();\
		  U0TXBUF = CC2420_SNOP;\
		  SPI_WAITFOREOTx();\
		  s = U0RXBUF;\
		  SPI_DISABLE();\
	 } while (0)

/***********************************************************
	FAST SPI: FIFO Access
***********************************************************/
// 	  p = pointer to the UINT8 array to be read/written
// 	  c = the number of UINT8s to read/write
// 	  b = single data UINT8

#define FASTSPI_WRITE_FIFO(p,c)\
	do {\
		UINT8 i = 0;\
	    SPI_ENABLE();\
		FASTSPI_TX_ADDR(CC2420_TXFIFO);\
		for (i = 0; i < (c); i++) {\
		    FASTSPI_TX(((UINT8*)(p))[i]);\
		}\
		SPI_DISABLE();\
    } while (0)

#define FASTSPI_WRITE_FIFO_NOCE(p,c)\
	do {\
		UINT8 spiCnt = 0;\
		FASTSPI_TX_ADDR(CC2420_TXFIFO);\
		for (spiCnt = 0; spiCnt < (c); spiCnt++) {\
		    FASTSPI_TX(((UINT8*)(p))[spiCnt]);\
		}\
    } while (0)

#define FASTSPI_READ_FIFO_UINT8(b)\
	 do {\
		  SPI_ENABLE();\
		  FASTSPI_RX_ADDR(CC2420_RXFIFO);\
		  U0RXBUF;\
		  FASTSPI_RX(b);\
  		  halWait(1);\
		  SPI_DISABLE();\
	 } while (0)


#define FASTSPI_READ_FIFO_NO_WAIT(p,c)\
	 do {\
		  UINT8 spiCnt = 0;\
		  SPI_ENABLE();\
		  FASTSPI_RX_ADDR(CC2420_RXFIFO);\
		  U0RXBUF;\
		  for (spiCnt = 0; spiCnt < (c); spiCnt++) {\
				FASTSPI_RX(((UINT8*)(p))[spiCnt]);\
		  }\
		  halWait(1);\
		  SPI_DISABLE();\
	 } while (0)



#define FASTSPI_READ_FIFO_GARBAGE(c)\
	 do {\
		  UINT8 spiCnt = 0;\
		  SPI_ENABLE();\
		  FASTSPI_RX_ADDR(CC2420_RXFIFO);\
		  U0RXBUF;\
		  for (spiCnt = 0; spiCnt < (c); spiCnt++) {\
				FASTSPI_RX_GARBAGE();\
		  }\
  		  halWait(1);\
		  SPI_DISABLE();\
	 } while (0)



/***********************************************************
	FAST SPI: CC2420 RAM access (big or little-endian order)
***********************************************************/
//  FAST SPI: CC2420 RAM access (big or little-endian order)
// 	  p = pointer to the variable to be written
// 	  a = the CC2420 RAM address
// 	  c = the number of UINT8s to write
// 	  n = counter variable which is used in for/while loops (UINT8)
//
//  Example of usage:
// 	  UINT8 n;
// 	  UINT16 shortAddress = 0xBEEF;
// 	  FASTSPI_WRITE_RAM_LE(&shortAddress, CC2420RAM_SHORTADDR, 2);


#define FASTSPI_WRITE_RAM_LE(p,a,c,n)\
	 do {\
		  SPI_ENABLE();\
		  FASTSPI_TX(0x80 | (a & 0x7F));\
		  FASTSPI_TX((a >> 1) & 0xC0);\
		  for (n = 0; n < (c); n++) {\
				FASTSPI_TX(((UINT8*)(p))[n]);\
		  }\
		  SPI_DISABLE();\
	 } while (0)

void halWait(UINT16 timeout);
void halSpiInit(void);

#endif

/***********************************************************************************
  Copyright 2007 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
***********************************************************************************/
