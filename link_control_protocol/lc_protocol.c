#include "lc_protocol.h"

static inline void set_default_tx_settings(BASIC_RF_TX_INFO *tx)
{
	tx->destPanId = rfSettings.panId;
	tx->ackRequest = TRUE;
}

BOOL lc_send_broadcast_request(BASIC_RF_TX_INFO *tx)
{
	set_default_tx_settings(tx);

	tx->ackRequest = FALSE;
	tx->destAddr = 0xFFFF;
	tx->length = 1;
	tx->pPayload[0] = LC_PCK_BROADCAST_REQUEST;

	return hal_cc2420_rf_send_packet(tx);
}

BOOL lc_send_node_response(BASIC_RF_TX_INFO *tx, UINT8 destAddr)
{
	set_default_tx_settings(tx);

	tx->destAddr = destAddr;
	tx->length = 1;
	tx->pPayload[0] = LC_PCK_NODE_RESPONSE;

	return hal_cc2420_rf_send_packet(tx);
}
