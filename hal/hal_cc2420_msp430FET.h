/************************************************************************************

  Filename:     hal_msp430FET.h

  Description:  HAL interface for the MSP430FET/CC2400EB combination referred to in
                AN033. The Hardware Abstraction Library is a collection of functions,
                macros and constants, which can be used to ease access to the
                hardware on the CC2420 and the target microcontroller.

                This file contains all definitions that are specific for the
                MSP-FET430P140 development board.

************************************************************************************/
#ifndef HAL_MSP430FET_H
#define HAL_MSP430FET_H

/////////////////////////////////////////////////////////////////////////////////////
// PUBLIC MACROS
//

#define ENABLE_GLOBAL_INT()     _EINT()
#define DISABLE_GLOBAL_INT()    _DINT()

#define V_REF		3

/////////////////////////////////////////////////////////////////////////////////////
// MSP430 IO ports
//

//-----------------------------------------------------------------------------------
// Port 1

#define FIFO_P         0  // P1.0 - Input: FIFOP from CC2420
#define FIFO           3  // P1.3 - Input: FIFO from CC2420
#define CCA            4  // P1.4 - Input:  CCA from CC2420




//-----------------------------------------------------------------------------------
// Port 2


//-----------------------------------------------------------------------------------
// Port 3

#define MOSI           1  // P3.1 - Output: SPI Master out - slave in (MOSI)
#define MISO           2  // P3.2 - Input:  SPI Master in - slave out (MISO)
#define SCK            3  // P3.3 - Output: SPI Serial Clock (SCLK)

//-----------------------------------------------------------------------------------
// Port 4

#define SFD            1  // P4.1 - Input:  SFD from CC2420
#define CSN            2  // P4.2 - Output: SPI Chip Select (CS_N)
#define MSN            4  // P4.4 - Output: MEMORY SPI Chip Select (S_N)
#define VREG_EN        5  // P4.5 - Output: VREG_EN to CC2420
#define RESET_N        6  // P4.6 - Output: RESET_N to CC2420
#define MHDN           7  // P4.7 - Output: MEMORY HOLD (HOLD_N)

//-----------------------------------------------------------------------------------
// Port 5

#define RLED		   4  // P5.4 - Output Blue LED
#define YLED		   5  // P5.5 - Output Red LED
#define BLED		   6  // P5.6 - Output Yellow LED


#define PORT_INIT()\
    do {\
        WDTCTL = WDTPW + WDTHOLD;\
        P5DIR |= BM(RLED) | BM(YLED) | BM(BLED); ;\
		P4DIR |= BM(CSN) | BM(MSN) | BM(VREG_EN) | BM(RESET_N) | BM(MHDN);\
		P4OUT |= BM(MSN) | BM(MHDN);\
		P5OUT |= BM(RLED);\
	} while (0)

#define SPI_INIT()  halSpiInit()


// Enables/disables the SPI CC2420 interface

#define SPI_ENABLE()    ( P4OUT &= ~BM(CSN) ) // ENABLE CSn (active low)
#define SPI_DISABLE()	( P4OUT |=  BM(CSN) ) // DISABLE CSn (active low)

//-----------------------------------------------------------------------------------
// LED
//
#define SET_YLED()      ( P5OUT |=  BM(YLED) )
#define CLR_YLED()      ( P5OUT &= ~BM(YLED) )
#define TOGGLE_YLED()   ( P5OUT ^=  BM(YLED) )

#define SET_RLED()      ( P5OUT |=  BM(RLED) )
#define CLR_RLED()      ( P5OUT &= ~BM(RLED) )
#define TOGGLE_RLED()   ( P5OUT ^=  BM(RLED) )

#define SET_BLED()      ( P5OUT |=  BM(BLED) )
#define CLR_BLED()      ( P5OUT &= ~BM(BLED) )
#define TOGGLE_BLED()   ( P5OUT ^=  BM(BLED) )

/////////////////////////////////////////////////////////////////////////////////////
// CC2420 PIN ACCESS
//

// Pin status
#define FIFO_IS_1       (!!(P1IN & BM(FIFO)))
#define CCA_IS_1        (!!(P1IN & BM(CCA) ))
#define RESET_IS_1      (!!(P4IN & BM(RESET_N)))
#define VREG_IS_1       (!!(P4IN & BM(VREG_EN)))
#define FIFOP_IS_1      (!!(P1IN & BM(FIFO_P)))
#define SFD_IS_1        (!!(P4IN & BM(SFD)))

// The CC2420 reset pin
#define SET_RESET_INACTIVE()    ( P4OUT |=  BM(RESET_N) )
#define SET_RESET_ACTIVE()      ( P4OUT &= ~BM(RESET_N) )

// CC2420 voltage regulator enable pin
#define SET_VREG_ACTIVE()       ( P4OUT |=  BM(VREG_EN) )
#define SET_VREG_INACTIVE()     ( P4OUT &= ~BM(VREG_EN) )
//-----------------------------------------------------------------------------------



/////////////////////////////////////////////////////////////////////////////////////
// EXTERNAL INTERRUPTS
//




//-----------------------------------------------------------------------------------

// Rising edge trigger for external interrupt 0 (FIFOP)
#define FIFOP_INT_INIT()            do { P1IES &= ~BM(FIFO_P); CLEAR_FIFOP_INT(); } while (0)

// FIFOP on external interrupt 0
#define ENABLE_FIFOP_INT()          do { P1IE |= BM(FIFO_P); } while (0)
#define DISABLE_FIFOP_INT()         do { P1IE &= ~BM(FIFO_P); } while (0)
#define CLEAR_FIFOP_INT()           do { P1IFG &= ~BM(FIFO_P); } while (0)
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// SFD interrupt on timer 1 capture pin
#define ENABLE_SFD_CAPTURE_INT()    do { TIMSK |= BM(TICIE1); } while (0)
#define DISABLE_SFD_CAPTURE_INT()   do { TIMSK &= ~BM(TICIE1); } while (0)
#define CLEAR_SFD_CAPTURE_INT()     do { TIFR = BM(ICF1); } while (0)
//-----------------------------------------------------------------------------------



/////////////////////////////////////////////////////////////////////////////////////
// BUTTONS
//


//-----------------------------------------------------------------------------------
// S2 button
//#define BUTTON_PRESSED()            !(P1IN & BM(BUTTON))


//-----------------------------------------------------------------------------------


/////////////////////////////////////////////////////////////////////////////////////
// APPLICATION DEBUGGING
//



//-----------------------------------------------------------------------------------
// Controlled application crash (flashes the LEDs forever to indicate an error code)
#define EXCEPTION(n) \
    do { \
        DISABLE_GLOBAL_INT(); \
        TOGGLE_YLED(); \
        halWait(20000); \
        TOGGLE_YLED(); \
        halWait(50000); \
    } while (TRUE)
//-----------------------------------------------------------------------------------


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
