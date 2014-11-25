#include <hal.h>

#include "lc_common.h"

#ifndef _LC_PROTOCOL_H_
#define _LC_PROTOCOL_H_

void lc_set_tx_broadcast_request(BASIC_RF_TX_INFO *tx);
void lc_set_tx_node_response(BASIC_RF_TX_INFO *tx, UINT16 destAddr);
void lc_set_tx_start_vrt(BASIC_RF_TX_INFO *tx, UINT16 destAddr);
void lc_set_tx_vrt_started(BASIC_RF_TX_INFO *tx, UINT16 destAddr);

void lc_set_tx_data_request(BASIC_RF_TX_INFO *tx, UINT16 destAddr);
void lc_set_tx_data_response(BASIC_RF_TX_INFO *tx, UINT16 destAddr, UINT32 humidity, UINT32 temperature, UINT32 luminiscense, UINT32 radiation);

#endif
