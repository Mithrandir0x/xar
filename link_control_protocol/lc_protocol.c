#include "lc_protocol.h"

void lc_set_tx_broadcast_request(BASIC_RF_TX_INFO *tx)
{
	tx->destPanId = rfSettings.panId;
	tx->ackRequest = FALSE;
	tx->destAddr = 0xFFFF;
	tx->length = 1;
	tx->pPayload[0] = LC_PCK_BROADCAST_REQUEST;
}

void lc_set_tx_node_response(BASIC_RF_TX_INFO *tx, UINT16 destAddr)
{
	tx->destPanId = rfSettings.panId;
	tx->ackRequest = TRUE;
	tx->destAddr = destAddr;
	tx->length = 1;
	tx->pPayload[0] = LC_PCK_NODE_RESPONSE;
}

void lc_set_tx_start_vrt(BASIC_RF_TX_INFO *tx, UINT16 destAddr)
{
	tx->destPanId = rfSettings.panId;
	tx->ackRequest = TRUE;
	tx->destAddr = destAddr;
	tx->length = 1;
	tx->pPayload[0] = LC_PCK_START_VRT;
}

void lc_set_tx_vrt_started(BASIC_RF_TX_INFO *tx, UINT16 destAddr)
{
	tx->destPanId = rfSettings.panId;
	tx->ackRequest = TRUE;
	tx->destAddr = destAddr;
	tx->length = 1;
	tx->pPayload[0] = LC_PCK_VRT_STARTED;
}

void lc_set_tx_data_request(BASIC_RF_TX_INFO *tx, UINT16 destAddr)
{
	tx->destPanId = rfSettings.panId;
	tx->ackRequest = FALSE;
	tx->destAddr = destAddr;
	tx->length = 1;
	tx->pPayload[0] = LC_PCK_DATA_REQUEST;
}

void lc_set_tx_data_response(BASIC_RF_TX_INFO *tx, UINT16 destAddr, UINT32 humidity, UINT32 temperature, UINT32 luminiscense, UINT32 radiation)
{
	tx->destPanId = rfSettings.panId;
	tx->ackRequest = TRUE;
	tx->destAddr = destAddr;
	tx->length = 17;
	tx->pPayload[0] = LC_PCK_DATA_RESPONSE;
	tx->pPayload[1] = humidity;
	tx->pPayload[5] = temperature;
	tx->pPayload[9] = luminiscense;
	tx->pPayload[13] = radiation;

}
