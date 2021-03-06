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

static int get_device(LinkControlManager *manager, UINT16 addr)
{
	UINT8 i;
	for ( i = 0 ; i < LC_MAX_DEVICES ; i++ )
	{
		if ( manager->devices[i].addr == addr ) return i;
	}

	return -1;
}

void lc_initialize(LinkControlManager *manager)
{
	UINT8 i;

	manager->work_mode = LC_WM_SYNC;
	manager->connected_devices = 0;
	manager->next_device_ask = 0;
	manager->all_devices_slept = FALSE;

	for ( i = 0 ; i < LC_MAX_DEVICES ; i++ )
	{
		manager->devices[i].addr = 0x0000;
		manager->devices[i].ready = FALSE;
		manager->devices[i].temperature = 0x00000000;
	}
}

void lc_register_device(LinkControlManager *manager, UINT16 addr)
{
	if ( manager->connected_devices < LC_MAX_DEVICES && !is_device_in_routing_table(manager, addr) )
	{
		manager->devices[manager->connected_devices].addr = addr;
		manager->connected_devices++;
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
	}
	else if ( manager->work_mode == LC_WM_DATA )
	{
		if ( manager->all_devices_slept )
		{
			FASTSPI_UPD_STATUS(status);

			lc_set_tx_data_request(tx, manager->devices[manager->next_device_ask].addr);
			hal_cc2420_rf_send_packet(tx);
		}
		else
		{
			FASTSPI_UPD_STATUS(status);

			lc_set_tx_start_vrt(tx, manager->devices[manager->next_device_ask].addr);
			hal_cc2420_rf_send_packet(tx);
		}

		manager->next_device_ask++;
		if ( manager->next_device_ask >= manager->connected_devices )
		{
			manager->next_device_ask = 0;
		}
	}

	TOGGLE_BLED();
}

void lc_reception_service_update(LinkControlManager *manager, BASIC_RF_TX_INFO *tx, BASIC_RF_RX_INFO *rx)
{
	RoutingTableEntry *entry;

	if ( manager->work_mode == LC_WM_SYNC )
	{
		if ( rx->pPayload[0] == LC_PCK_NODE_RESPONSE )
		{
			lc_register_device(manager, rx->srcAddr);
		}
	}
	else if ( manager->work_mode == LC_WM_DATA )
	{
		if ( rx->pPayload[0] == LC_PCK_VRT_STARTED )
		{
			int i = get_device(manager, rx->srcAddr);
			if ( i != -1 )
			{
				entry = &(manager->devices[i]);
				entry->ready = TRUE;

				for ( i = 0 ; i < manager->connected_devices ; i++ ) {
					if ( manager->devices[i].ready == FALSE ) return;
				}

				manager->all_devices_slept = TRUE;
			}
		}

		if ( rx->pPayload[0] == LC_PCK_DATA_RESPONSE )
		{
			int i = get_device(manager, rx->srcAddr);
			if ( i != -1 )
			{
				int i = get_device(manager, rx->srcAddr);
				if ( i != -1 )
				{
					entry = &(manager->devices[i]);
					entry->temperature = rx->pPayload[1] << 8 | rx->pPayload[2];
				}
			}
		}
	}
}
