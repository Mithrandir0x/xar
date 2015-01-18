#ifndef _LC_COMMON_H_
#define _LC_COMMON_H_

#include <hal.h>

#define LC_MAX_DEVICES 16

/**
 * Working Modes
 */
#define LC_WM_SYNC 1 				// Synchro coordinator mode
#define LC_WM_DATA 2 				// Reception coordinator mode

#define LC_WM_CLIENT_STARTED 1 		// Start mode
#define LC_WM_CLIENT_SYNC 2 		// Synchro client mode
#define LC_WM_CLIENT_WAIT_VRT 3		// Wait VRT client mode
#define LC_WM_CLIENT_SEND_VRT 4		// VRT send mode
#define LC_WM_CLIENT_WAIT 5 		// Wait client mode
#define LC_WM_CLIENT_SEND_DATA 6 	// Send client mode

/**
 * Link Control Packet Definitions
 */
#define LC_PCK_BROADCAST_REQUEST 0xFF
#define LC_PCK_NODE_RESPONSE     0x0F
#define LC_PCK_START_VRT         0xAE
#define LC_PCK_VRT_STARTED       0xAA
#define LC_PCK_DATA_REQUEST      0x13
#define LC_PCK_DATA_RESPONSE     0x14

typedef struct {
	UINT16 addr;
	BOOL ready;
	UINT16 humidity;
	UINT16 temperature;
	UINT16 luminiscense;
	UINT16 radiation;
} RoutingTableEntry;

typedef struct {
	UINT8 work_mode;
	BOOL coordinator_address;
} LinkControlClientManager;

typedef struct {
	UINT8 work_mode;
	UINT8 next_device_ask;
	BOOL all_devices_slept;
	UINT8 connected_devices;
	RoutingTableEntry devices[LC_MAX_DEVICES];
} LinkControlManager;



#endif
