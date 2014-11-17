#include "link_control.h"

void lc_initialize(LinkControlManager *manager)
{
	manager->work_mode = LC_WM_SYNC;
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

BOOL lc_send_node_response(BASIC_RF_TX_INFO *tx)
{
	tx->destPanId = rfSettings.panId;
	tx->destAddr = 0xFFFF;
	tx->ackRequest = TRUE;
	tx->length = 1;
	tx->pPayload[0] = 0x0F;

	return hal_cc2420_rf_send_packet(tx);
}
