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
	BASIC_RF_SETTINGS *rfSettings;
	UINT8 work_mode;
	UINT16 devices[LC_MAX_DEVICES];
} LinkControlManager;

void lc_initialize(LinkControlManager *manager, BASIC_RF_SETTINGS *rfSettings);

BOOL lc_send_broadcast_request(BASIC_RF_TX_INFO *tx);

#endif
