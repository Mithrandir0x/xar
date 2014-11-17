#include <msp430.h>
#include <hal.h>

#include "link_control.h"

volatile BASIC_RF_SETTINGS rfSettings;

static LinkControlManager manager;

static BASIC_RF_RX_INFO rfRxInfo;
static BASIC_RF_TX_INFO rfTxInfo;
static UINT8 pTxBuffer[BASIC_RF_MAX_PAYLOAD_SIZE];
static UINT8 pRxBuffer[BASIC_RF_MAX_PAYLOAD_SIZE];

#define PAYLOAD_SIZE	10
#define RF_CHANNEL		15
#define TX_PERIOD       50  // Packet sent each n'th cycle

#define PANID			0x2420
#define MYADDR			0xCAFE

#ifdef __ICC430__
#define FILL_UINT8    0xFF
#else
#define FILL_UINT8    0xEE
#endif

void InitP2_7(void){
	P2DIR &= 0x7F;
	P2SEL &= BIT7;
	P2IES |= 0x80;
	P2IE |= BIT7;
	_EINT();
}

int main(void)
{
	int n;

    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer (good dog)

    _disable_interrupt();

    // Initalize ports for communication with CC2420 and other peripheral units
	PORT_INIT();
	SPI_INIT();
	InitP2_7();

	halTimer_a_initialize(TIMER_CLKSRC_SMCLK, TIMER_MODE_UP);
	// Each 1000 microseconds or each millisecond, a timer interruption will be risen
	halTimer_a_setTicks(1000);

	// Wait for the user to select node address, and initialize for basic RF operation
	halWait(1000);

	hal_cc2420_rf_init(&rfRxInfo, RF_CHANNEL, PANID, MYADDR);
	//rfTxInfo.destAddr = DESADDR;
	rfTxInfo.destPanId = PANID;


	// Initalize common protocol parameters
	rfTxInfo.length = PAYLOAD_SIZE;
	rfTxInfo.ackRequest = TRUE;
	rfTxInfo.pPayload = pTxBuffer;
	rfRxInfo.pPayload = pRxBuffer;

	lc_initialize(&manager);

	for (n = 0; n < PAYLOAD_SIZE; n++) {
		pTxBuffer[n] = FILL_UINT8;
	}

	hal_cc2420_rf_set_receive_on();

    _enable_interrupt();

	while ( TRUE ) {
	}

	return 0;
}

BASIC_RF_RX_INFO* basicRfReceivePacket(BASIC_RF_RX_INFO *rxInfo)
{
	return rxInfo;
}

/**
 * Interruption Handler for Timer A
 *
 * This interruption handler uses TACCR0 and CCIFG
 */
#pragma vector=TIMERA0_VECTOR
__interrupt void timer_a_interrupt_handler(){
	// Do something
}

#pragma vector=PORT1_VECTOR
__interrupt void fifo_rx(void){
	hal_cc2420_rf_manage_interruption();
}
