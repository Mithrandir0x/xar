#include <msp430.h>
#include <hal.h>
#include <lc.h>

#define PAYLOAD_SIZE	10
#define RF_CHANNEL		15
#define TX_PERIOD       50  // Packet sent each n'th cycle

#define PANID			0x2420
#define MYADDR			0xD0D0

#ifdef __ICC430__
#define FILL_UINT8    0xFF
#else
#define FILL_UINT8    0xEE
#endif

volatile BASIC_RF_SETTINGS rfSettings;

static BASIC_RF_RX_INFO rfRxInfo;
static BASIC_RF_TX_INFO rfTxInfo;
static UINT8 pTxBuffer[BASIC_RF_MAX_PAYLOAD_SIZE];
static UINT8 pRxBuffer[BASIC_RF_MAX_PAYLOAD_SIZE];

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

	// Wait for the user to select node address, and initialize for basic RF operation
	halWait(1000);

	hal_cc2420_rf_init(&rfRxInfo, RF_CHANNEL, PANID, MYADDR);
	rfTxInfo.destPanId = PANID;

	// Initalize common protocol parameters
	rfTxInfo.length = PAYLOAD_SIZE;
	rfTxInfo.ackRequest = TRUE;
	rfTxInfo.pPayload = pTxBuffer;
	rfRxInfo.pPayload = pRxBuffer;

	for (n = 0; n < PAYLOAD_SIZE; n++) {
		pTxBuffer[n] = FILL_UINT8;
	}

	hal_cc2420_rf_set_receive_on();

    _enable_interrupt();

	while ( TRUE ) {
	}

	return 0;
}

BASIC_RF_RX_INFO* hal_cc2420_rf_on_receive_packet(BASIC_RF_RX_INFO *rx)
{
	if ( rx->pPayload[0] == LC_PCK_BROADCAST_REQUEST )
	{
		lc_send_node_response(&rfTxInfo, (UINT8) MYADDR);
	}

	return rx;
}

BASIC_RF_RX_INFO* hal_cc2420_rf_on_receive_ack_packet(BASIC_RF_RX_INFO *rx)
{
	return rx;
}

#pragma vector=PORT1_VECTOR
__interrupt void fifo_rx(void){
	hal_cc2420_rf_manage_interruption();
}
