#include "hal_common.h"

/************************************************************************************

  Filename:     hal_rf_set_channel.c

  Description:  The Chipcon Hardware Abstraction Library is a collection of functions,
                macros and constants, which can be used to ease access to the hardware
                on the CC2420 and the target microcontroller.

                This file contains a function that allows you to switch radio channels
                on the CC2420.

   EXAMPLE OF USAGE:

       // Turn off RX...
       DISABLE_GLOBAL_INT();
       FASTSPI_STROBE(CC2420_SRFOFF);
       ENABLE_GLOBAL_INT();

       // ... switch to the next channel in the loop ...
       halRfSetChannel(channel++);
       if (channel == 27) channel = 11;

       // ... and go back into RX
       DISABLE_GLOBAL_INT();
       FASTSPI_STROBE(CC2420_SRXON);
       ENABLE_GLOBAL_INT();

************************************************************************************/


//-----------------------------------------------------------------------------------
//	void halRfSetChannel(UINT8 Channel)
//
//	DESCRIPTION:
//		Programs CC2420 for a given IEEE 802.15.4 channel.
//		Note that SRXON, STXON or STXONCCA must be run for the new channel selection to take full effect.
//
//	PARAMETERS:
//		UINT8 channel
//			The channel number (11-26)
//-----------------------------------------------------------------------------------
void halRfSetChannel(UINT8 channel)
{
	UINT16 f;

	// Derive frequency programming from the given channel number
	f = (UINT16) (channel - 11); // Subtract the base channel
	f = f + (f << 2);    		 // Multiply with 5, which is the channel spacing
	f = f + 357 + 0x4000;		 // 357 is 2405-2048, 0x4000 is LOCK_THR = 1

    // Write it to the CC2420
	DISABLE_GLOBAL_INT();
	FASTSPI_SETREG(CC2420_FSCTRL, f);
	ENABLE_GLOBAL_INT();

}

/************************************************************************************

  Filename:     hal_rf_wait_for_crystal_oscillator.c

  Description:  The Chipcon Hardware Abstraction Library is a collection of functions,
                macros and constants, which can be used to ease access to the hardware
                on the CC2420 and the target microcontroller.

                This file contains a function that ensures that the CC2420 crystal
                oscillator is stable.

    EXAMPLE OF USAGE:

      // Turn the crystal oscillator on and wait for it to become stable
      DISBALE_GLOBAL_INT();
      FASTSPI_STROBE(CC2420_SXOSCON);
      ENABLE_GLOBAL_INT();
      halRfWaitForCrystalOscillator();

************************************************************************************/
//-----------------------------------------------------------------------------------
//	void rfWaitForCrystalOscillator(void)
//
//	DESCRIPTION:
//		Waits for the crystal oscillator to become stable. The flag is polled via the
//      SPI status byte.
//
//      Note that this function will lock up if the SXOSCON command strobe has not
//      been given before the function call. Also note that global interrupts will
//      always be enabled when this function returns.
//-----------------------------------------------------------------------------------
void halRfWaitForCrystalOscillator(void)
{
    UINT8 spiStatusByte;

    // Poll the SPI status byte until the crystal oscillator is stable
    do {
	    DISABLE_GLOBAL_INT();
	    FASTSPI_UPD_STATUS(spiStatusByte);
	    ENABLE_GLOBAL_INT();
    } while (!(spiStatusByte & (BM(CC2420_XOSC16M_STABLE))));

}


