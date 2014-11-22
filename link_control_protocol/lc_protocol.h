#include <hal.h>

#include "lc_common.h"

#ifndef _LC_PROTOCOL_H_
#define _LC_PROTOCOL_H_

void lc_set_tx_broadcast_request(BASIC_RF_TX_INFO *tx);
void lc_set_tx_node_response(BASIC_RF_TX_INFO *tx, UINT16 destAddr);

#endif
