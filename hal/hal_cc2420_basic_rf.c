#include "hal_common.h"
#include "hal_cc2420_basic_rf.h"

/**
 * This macro allows to apply a timeout when waiting for a register going low or high.
 *
 * @param c The condition that when set to TRUE it will break the wait
 * @param t The time to wait before updating the timeout counter
 * @param x The timeout counter that will be incremented each iteration
 * @param m The maximum value that the timeout counter will get before breaking the loop
 */
#define TIMEOUT(c, t, x, m) \
	do { \
		x = 0; \
		while (c && x < m) { \
			halWait(t); \
			x++; \
		} \
	} while ( 0 )

void hal_cc2420_rf_init(BASIC_RF_RX_INFO *pRRI, UINT8 channel, UINT16 panId, UINT16 myAddr){
    UINT8 n;

    //Make sure that the voltage regulator is on, and that the reset pin is inactive
    SET_VREG_ACTIVE();
    halWait(1000);
    SET_RESET_ACTIVE();
    halWait(1000);
    SET_RESET_INACTIVE();
    halWait(500);
    //DISABLE_GLOBAL_INT();
    // Register modifications
    FASTSPI_STROBE(CC2420_SXOSCON);
    FASTSPI_SETREG(CC2420_MDMCTRL0, 0x0AF2); // Turn on automatic packet acknowledgment // 0000 1010 1111 0010
    FASTSPI_SETREG(CC2420_MDMCTRL1, 0x0500); // Set the correlation threshold = 20      // 0000 0101 0000 0000
    FASTSPI_SETREG(CC2420_IOCFG0, 0x007F);   // Set the FIFOP threshold to maximum      // 0000 0000 0111 1111
    FASTSPI_SETREG(CC2420_SECCTRL0, 0x01C4); // Turn off "Security enable"              // 0000 0001 1100 0100

    // Set the RF channel
    halRfSetChannel(channel);

    // Turn interrupts back on
	//ENABLE_GLOBAL_INT();

	// Set the protocol configuration
	rfSettings.pRxInfo = pRRI;
	rfSettings.panId = panId;
	rfSettings.myAddr = myAddr;
	rfSettings.txSeqNumber = 0;
    rfSettings.receiveOn = FALSE;

	// Wait for the crystal oscillator to become stable
    halRfWaitForCrystalOscillator();

	// Write the short address and the PAN ID to the CC2420 RAM (requires that the XOSC is on and stable)
   	DISABLE_GLOBAL_INT();
    FASTSPI_WRITE_RAM_LE(&myAddr, CC2420RAM_SHORTADDR, 2, n);
    FASTSPI_WRITE_RAM_LE(&panId, CC2420RAM_PANID, 2, n);
    _EINT();
}

//-----------------------------------------------------------------------------------
//  UINT8 basicRfSendPacket(BASIC_RF_TX_INFO *pRTI)
//
//  DESCRIPTION:
//		Transmits a packet using the IEEE 802.15.4 MAC data packet format with short addresses. CCA is
//		measured only once before backet transmission (not compliant with 802.15.4 CSMA-CA).
//		The function returns:
//			- When pRTI->ackRequest is FALSE: After the transmission has begun (SFD gone high)
//			- When pRTI->ackRequest is TRUE: After the acknowledgment has been received/declared missing.
//		The acknowledgment is received through the FIFOP interrupt.
//
//  ARGUMENTS:
//      BASIC_RF_TX_INFO *pRTI
//          The transmission structure, which contains all relevant info about the packet.
//
//  RETURN VALUE:
//		BOOL
//			Successful transmission (acknowledgment received)
//-----------------------------------------------------------------------------------
BOOL hal_cc2420_rf_send_packet(BASIC_RF_TX_INFO *pRTI)
{
	UINT16 frameControlField;
    UINT8 packetLength;
    BOOL success;
    UINT8 spiStatusUINT8;
    UINT8 timeout_counter;

    // Wait until the transceiver is idle
    while (FIFOP_IS_1 || SFD_IS_1);

    // Turn off global interrupts to avoid interference on the SPI interface
    DISABLE_GLOBAL_INT();

	// Flush the TX FIFO just in case...
	FASTSPI_STROBE(CC2420_SFLUSHTX);

    // Turn on RX if necessary
    if (!rfSettings.receiveOn) FASTSPI_STROBE(CC2420_SRXON);

    // Wait for the RSSI value to become valid
    do {
        FASTSPI_UPD_STATUS(spiStatusUINT8);
    } while (!(spiStatusUINT8 & BM(CC2420_RSSI_VALID)));

    packetLength = pRTI->length + BASIC_RF_PACKET_OVERHEAD_SIZE;
    frameControlField = pRTI->ackRequest ? BASIC_RF_FCF_ACK : BASIC_RF_FCF_NOACK;


    FASTSPI_WRITE_FIFO((UINT8*)&packetLength, 1);             // Packet length
    FASTSPI_WRITE_FIFO((UINT8*)&frameControlField, 2);         // Frame control field
    FASTSPI_WRITE_FIFO((UINT8*)&rfSettings.txSeqNumber, 1);    // Sequence number
    FASTSPI_WRITE_FIFO((UINT8*)&rfSettings.panId, 2);          // Dest. PAN ID
    FASTSPI_WRITE_FIFO((UINT8*)&pRTI->destAddr, 2);            // Dest. address
    FASTSPI_WRITE_FIFO((UINT8*)&rfSettings.myAddr, 2);         // Source address
	FASTSPI_WRITE_FIFO((UINT8*)pRTI->pPayload, pRTI->length);  // Payload

	// Wait for the transmission to begin before exiting (makes sure that this
    // function cannot be calld a second time, and thereby cancelling the first
    // transmission (observe the FIFOP + SFD test above).

	FASTSPI_STROBE(CC2420_STXONCCA);

	// while (!SFD_IS_1);

	// We have found that while waiting for SFD flag to go low is not detected
	// and we get deadlocked at this instruction. Therefore, a timeout mechanism
	// has been applied to ignore the flag and continue with the operation.
	TIMEOUT(!SFD_IS_1, 5000, timeout_counter, 10);

	if ( timeout_counter >= 10 ) {
		TOGGLE_RLED();
	}

	// Turn interrupts back on
	ENABLE_GLOBAL_INT();

    // Wait for the acknowledge to be received, if any
    if (pRTI->ackRequest) {
		rfSettings.ackReceived = FALSE;

		// Wait for the SFD to go low again
		while (SFD_IS_1);

        // We'll enter RX automatically, so just wait until we can be sure that the
        // ack reception should have finished. The timeout consists of a 12-symbol
        // turnaround time, the ack packet duration, and a small margin
        halWait((12 * BASIC_RF_SYMBOL_DURATION) + (BASIC_RF_ACK_DURATION) +
                (2 * BASIC_RF_SYMBOL_DURATION) + 100);

		// If an acknowledgment has been received (by the FIFOP interrupt),
        // the ackReceived flag should be set
		success = rfSettings.ackReceived;
    } else {
        success= TRUE;
    }

	// Turn off the receiver if it should not continue to be enabled
    DISABLE_GLOBAL_INT();
	if (!rfSettings.receiveOn) FASTSPI_STROBE(CC2420_SRFOFF);
    ENABLE_GLOBAL_INT();

    // Increment the sequence number, and return the result
    rfSettings.txSeqNumber++;
    return success;

}

//-----------------------------------------------------------------------------------
//  void halRfReceiveOn(void)
//
//  DESCRIPTION:
//      Enables the CC2420 receiver and the FIFOP interrupt. When a packet is received through this
//      interrupt, it will call halRfReceivePacket(...), which must be defined by the application
//-----------------------------------------------------------------------------------
void hal_cc2420_rf_set_receive_on(void)
{
    rfSettings.receiveOn = TRUE;
	FASTSPI_STROBE(CC2420_SRXON);
	FASTSPI_STROBE(CC2420_SFLUSHRX);
	FIFOP_INT_INIT();
    ENABLE_FIFOP_INT();
}

//-----------------------------------------------------------------------------------
//  void halRfReceiveOff(void)
//
//  DESCRIPTION:
//      Disables the CC2420 receiver and the FIFOP interrupt.
//-----------------------------------------------------------------------------------
void hal_cc2420_rf_set_receive_off(void)
{
    rfSettings.receiveOn = FALSE;
	FASTSPI_STROBE(CC2420_SRFOFF);
    DISABLE_FIFOP_INT();
} // basicRfReceiveOff

void hal_cc2420_rf_manage_interruption()
{
	UINT16 frameControlField;
	INT8 length;
	UINT8 pFooter[2];

	CLEAR_FIFOP_INT();

	// Clean up and exit in case of FIFO overflow, which is indicated by FIFOP = 1 and FIFO = 0
	if((FIFOP_IS_1) && (!(FIFO_IS_1))) {
		FASTSPI_STROBE(CC2420_SFLUSHRX);
		FASTSPI_STROBE(CC2420_SFLUSHRX);
		return;
	}

	// Payload length
	FASTSPI_READ_FIFO_UINT8(length);

	length &= BASIC_RF_LENGTH_MASK; // Ignore MSB

	// Ignore the packet if the length is too short
	if (length < BASIC_RF_ACK_PACKET_SIZE) {
		FASTSPI_READ_FIFO_GARBAGE(length);

		// Otherwise, if the length is valid, then proceed with the rest of the packet
	} else {
		// Register the payload length
		rfSettings.pRxInfo->length = length - BASIC_RF_PACKET_OVERHEAD_SIZE;

		// Read the frame control field and the data sequence number
		FASTSPI_READ_FIFO_NO_WAIT((UINT8*) &frameControlField, 2);
		rfSettings.pRxInfo->ackRequest = !!(frameControlField & BASIC_RF_FCF_ACK_BM);
		FASTSPI_READ_FIFO_UINT8(rfSettings.pRxInfo->seqNumber);

		// Is this an acknowledgment packet?
		if ((length == BASIC_RF_ACK_PACKET_SIZE) && (frameControlField == BASIC_RF_ACK_FCF) && (rfSettings.pRxInfo->seqNumber == rfSettings.txSeqNumber)) {

			// Read the footer and check for CRC OK
			FASTSPI_READ_FIFO_NO_WAIT((UINT8*) pFooter, 2);

			// Indicate the successful ack reception (this flag is pol by the transmission routine)
			if (pFooter[1] & BASIC_RF_CRC_OK_BM) {
				rfSettings.ackReceived = TRUE;
				rfSettings.pRxInfo = hal_cc2420_rf_on_receive_ack_packet(rfSettings.pRxInfo);
			}

			// Too small to be a valid packet?
		} else if (length < BASIC_RF_PACKET_OVERHEAD_SIZE) {
			FASTSPI_READ_FIFO_GARBAGE(length - 3);
			return;

			// Receive the rest of the packet
		} else {

			// Read destination PAN and address (that's taken care of by harware address recognition!)
			FASTSPI_READ_FIFO_NO_WAIT((UINT8*) &rfSettings.pRxInfo->destPanId, 2);
			FASTSPI_READ_FIFO_NO_WAIT((UINT8*) &rfSettings.pRxInfo->destAddr, 2);

			// Read the source address
			FASTSPI_READ_FIFO_NO_WAIT((UINT8*) &rfSettings.pRxInfo->srcAddr, 2);

			// Read the packet payload
			FASTSPI_READ_FIFO_NO_WAIT(rfSettings.pRxInfo->pPayload, rfSettings.pRxInfo->length);

			// Read the footer to get the RSSI value
			FASTSPI_READ_FIFO_NO_WAIT((UINT8*) pFooter, 2);
			rfSettings.pRxInfo->rssi = pFooter[0];

			// Notify the application about the received _data_ packet if the CRC is OK
			if (((frameControlField & (BASIC_RF_FCF_BM)) == BASIC_RF_FCF_NOACK)
					&& (pFooter[1] & BASIC_RF_CRC_OK_BM)
					&& ( rfSettings.myAddr == rfSettings.pRxInfo->destAddr
							|| rfSettings.myAddr == 0xFFFF)
						) {
				rfSettings.pRxInfo = hal_cc2420_rf_on_receive_packet(rfSettings.pRxInfo);
			}
		}
	}
}
