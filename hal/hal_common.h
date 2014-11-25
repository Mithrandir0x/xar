/************************************************************************************

  Filename:     hal_common.h

  Description:  Common include file.

  This file shall be included in all source files (*.c). No other library inclusions
  are required.

************************************************************************************/
#ifndef INCLUDES_H
#define INCLUDES_H


//-----------------------------------------------------------------------------------
// Common data types
typedef unsigned char		BOOL;
typedef unsigned char		BYTE;
typedef unsigned char		UINT8;
typedef unsigned short		UINT16;
typedef unsigned long		UINT32;
//typedef unsigned long long	UINT64;

typedef signed char			INT8;
typedef signed short		INT16;
typedef signed long			INT32;
//typedef signed long long	INT64;

// Common values
#ifndef FALSE
	#define FALSE 0
#endif
#ifndef TRUE
	#define TRUE 1
#endif
#ifndef NULL
	#define NULL 0
#endif

// Useful stuff
#define BM(n) (1 << (n))
#define BF(x,b,s) (((x) & (b)) >> (s))
#define MIN(n,m) (((n) < (m)) ? (n) : (m))
#define MAX(n,m) (((n) < (m)) ? (m) : (n))
#define ABS(n) ((n < 0) ? -(n) : (n))

// Dynamic function pointer
typedef void (*VFPTR)(void);

#include <string.h>



/////////////////////////////////////////////////////////////////////////////////////
// MSP430-GCC C/C++ compiler for MSP430 (__MSP430__)
// IAR C/C++ compiler for MSP430 (__ICC430__)
#if defined(__ICC430__) || defined(__MSP430__)
// Include files for MSP430
#include <msp430x16x.h>
#ifdef __MSP430__
    #include <signal.h>
#endif

#include "hal_cc2420_msp430FET.h"
#include "hal_spi.h"
#endif

/////////////////////////////////////////////////////////////////////////////////////
// Common include files
#include "hal_cc2420.h"
#include "hal_cc2420_basic_rf.h"

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
