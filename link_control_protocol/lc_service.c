#include "lc_service.h"
#include "lc_protocol.h"

static BOOL is_device_in_routing_table(LinkControlManager *manager, UINT16 addr)
{
	UINT8 i;
	for ( i = 0 ; i < LC_MAX_DEVICES ; i++ )
	{
		if ( manager->devices[i].addr == addr ) return TRUE;
	}

	return FALSE;
}

void lc_initialize(LinkControlManager *manager)
{
	UINT8 i;

	manager->work_mode = LC_WM_SYNC;
	manager->connected_devices = 0;

	for ( i = 0 ; i < LC_MAX_DEVICES ; i++ )
	{
		manager->devices[i].addr = 0x0000;
	}
}

void lc_set_work_mode(LinkControlManager *manager, UINT8 work_mode)
{
	manager->work_mode = work_mode;
}

void lc_sending_service_update(LinkControlManager *manager, BASIC_RF_TX_INFO *tx, BASIC_RF_RX_INFO *rx)
{
	UINT8 status;

	if ( manager->work_mode == LC_WM_SYNC )
	{
		FASTSPI_UPD_STATUS(status);

		lc_set_tx_broadcast_request(tx);
		hal_cc2420_rf_send_packet(tx);

		TOGGLE_BLED();
	}
	else if ( manager->work_mode == LC_WM_DATA )
	{
	}
}

void lc_reception_service_update(LinkControlManager *manager, BASIC_RF_TX_INFO *tx, BASIC_RF_RX_INFO *rx)
{
	if ( manager->work_mode == LC_WM_SYNC )
	{
		if ( rx->pPayload[0] == LC_PCK_NODE_RESPONSE )
		{
			if ( manager->connected_devices < LC_MAX_DEVICES )
			{
				if ( !is_device_in_routing_table(manager, rx->srcAddr) )
				{
					manager->devices[manager->connected_devices].addr = rx->srcAddr;
					manager->connected_devices++;
				}
			}
		}
	}
	else if ( manager->work_mode == LC_WM_DATA )
	{
	}
}
