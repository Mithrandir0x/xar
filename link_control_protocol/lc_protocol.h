#include <hal.h>

#include "lc_common.h"

#ifndef _LC_PROTOCOL_H_
#define _LC_PROTOCOL_H_

BOOL lc_send_broadcast_request(BASIC_RF_TX_INFO *tx);
BOOL lc_send_node_response(BASIC_RF_TX_INFO *tx, UINT8 destAddr);

#endif
