#ifndef _LINK_CONTROL_H_
#define _LINK_CONTROL_H_

#include <hal.h>

#define LC_MAX_DEVICES 16

/**
 * Working Modes
 */
#define LC_WM_SYNC 1
#define LC_WM_DATA 2

typedef struct {
	UINT16 addr;
} RoutingTableEntry;

typedef struct {
	UINT8 work_mode;
	RoutingTableEntry devices[LC_MAX_DEVICES];
} LinkControlManager;

void lc_initialize(LinkControlManager *manager);

BOOL lc_send_broadcast_request(BASIC_RF_TX_INFO *tx);

#endif
