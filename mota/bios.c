#include <msp430.h>
#include <hal.h>
#include <lc.h>

#define PAYLOAD_SIZE	10
#define RF_CHANNEL		15
#define TX_PERIOD       50  // Packet sent each n'th cycle

#define TICK_RATE 50000		//aprox 50ms

#define PANID			0x2420
#define MYADDR			0xD0D0

#ifdef __ICC430__
#define FILL_UINT8    0xFF
#else
#define FILL_UINT8    0xEE
#endif

volatile BASIC_RF_SETTINGS rfSettings;
static LinkControlClientManager manager;
static BASIC_RF_RX_INFO rfRxInfo;
static BASIC_RF_TX_INFO rfTxInfo;
static UINT8 pTxBuffer[BASIC_RF_MAX_PAYLOAD_SIZE];
static UINT8 pRxBuffer[BASIC_RF_MAX_PAYLOAD_SIZE];

static BOOL update_lc_service = TRUE;

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
	sht11_init();

	halTimer_a_initialize(TIMER_CLKSRC_SMCLK, TIMER_MODE_UP);
	// Each 1000 microseconds or each millisecond, a timer interruption will be risen
	halTimer_a_setTicks(TICK_RATE);

	// Wait for the user to select node address, and initialize for basic RF operation
	halWait(1000);

	lc_initialize_client(&manager);

	hal_cc2420_rf_init(&rfRxInfo, RF_CHANNEL, PANID, MYADDR);

	// Initalize common protocol parameters
	rfTxInfo.destPanId = PANID;
	rfTxInfo.length = PAYLOAD_SIZE;
	rfTxInfo.ackRequest = FALSE;
	rfTxInfo.pPayload = pTxBuffer;
	rfRxInfo.pPayload = pRxBuffer;

	for (n = 0; n < PAYLOAD_SIZE; n++) {
		pTxBuffer[n] = FILL_UINT8;
	}

	hal_cc2420_rf_set_receive_on();

	SET_YLED(); 	//Yellow LED: Indicates received ACK from node response
	SET_BLED();		//Blue LED: Low energy mode activated
	SET_RLED();		//Red LED: Sent temperature packet

	//Enable interrupts
    _enable_interrupt();
    halTimer_a_enableInterrupt();



	while ( TRUE ) {
		if ( update_lc_service ) {
			lc_sending_service_update_client(&manager, &rfTxInfo, &rfRxInfo);
			update_lc_service = FALSE;
		}
	}

}

BASIC_RF_RX_INFO* hal_cc2420_rf_on_receive_packet(BASIC_RF_RX_INFO *rx)
{
	lc_reception_service_update_client(&manager, &rfTxInfo, rx);

	return rx;
}

BASIC_RF_RX_INFO* hal_cc2420_rf_on_receive_ack_packet(BASIC_RF_RX_INFO *rx)
{
	if ( manager.work_mode == LC_WM_CLIENT_SYNC )
	{
		lc_set_work_mode_client(&manager, LC_WM_CLIENT_WAIT_VRT);
		CLR_YLED();
	}

	return rx;
}


/**
 * Interruption Handler for Timer A
 *
 * This interruption handler uses TACCR0 and CCIFG
 */
#pragma vector=TIMERA0_VECTOR
__interrupt void timer_a_interrupt_handler()
{
	halTimer_a_disableInterrupt();
	update_lc_service = TRUE;
	halTimer_a_enableInterrupt();
}


#pragma vector=PORT1_VECTOR
__interrupt void fifo_rx(void){
	hal_cc2420_rf_manage_interruption();
}


// Button 2 pressed event
#pragma vector=PORT2_VECTOR
__interrupt void P2_ISR(void)
{
	P2IFG &= 0x7F;//clear flag from user button

//	if (manager->work_mode == LC_WM_CLIENT_WAIT)
//		lc_set_work_mode_client(&manager, LC_WM_CLIENT_SEND_DATA);

}

