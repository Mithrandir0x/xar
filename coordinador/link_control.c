#include "link_control.h"

void lc_initialize(LinkControlManager *manager, BASIC_RF_SETTINGS *rfSettings)
{
	manager->work_mode = LC_WM_SYNC;
	manager->rfSettings = rfSettings;
}

BOOL lc_send_broadcast_request(BASIC_RF_TX_INFO *tx)
{
	tx->destPanId = rfSettings.panId;
	tx->destAddr = 0xFFFF;
	tx->ackRequest = TRUE;
	tx->length = 1;
	tx->pPayload[0] = 0xFF;

	return hal_cc2420_rf_send_packet(tx);
}
