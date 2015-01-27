/*
 * lc_client_service.c
 *
 *  Created on: 15/01/2015
 *      Author: David Camacho
 */




#include "lc_client_service.h"
#include "lc_protocol.h"


void lc_initialize_client(LinkControlClientManager *manager)
{
	manager->work_mode = LC_WM_CLIENT_STARTED;
}


void lc_set_work_mode_client(LinkControlClientManager *manager, UINT8 work_mode)
{
	manager->work_mode = work_mode;
}


void lc_sending_service_update_client(LinkControlClientManager *manager, BASIC_RF_TX_INFO *tx, BASIC_RF_RX_INFO *rx)
{
	UINT8 status;

	if ( manager->work_mode == LC_WM_CLIENT_SYNC )
	{
		FASTSPI_UPD_STATUS(status);

		lc_set_tx_node_response(tx, manager->coordinator_address);
		hal_cc2420_rf_send_packet(tx);


	}
	else if ( manager->work_mode == LC_WM_CLIENT_SEND_VRT)
	{
		FASTSPI_UPD_STATUS(status);

		lc_set_tx_vrt_started(tx, manager->coordinator_address);
		hal_cc2420_rf_send_packet(tx);
		lc_set_work_mode_client(manager, LC_WM_CLIENT_WAIT);
		CLR_BLED();
	}
	else if ( manager->work_mode == LC_WM_CLIENT_SEND_DATA)
	{
		FASTSPI_UPD_STATUS(status);

		lc_set_tx_data_response(tx, manager->coordinator_address, sht11_temp());
		hal_cc2420_rf_send_packet(tx);
		lc_set_work_mode_client(manager, LC_WM_CLIENT_WAIT);


	}


}


void lc_reception_service_update_client(LinkControlClientManager *manager, BASIC_RF_TX_INFO *tx, BASIC_RF_RX_INFO *rx)
{

	if ( rx->pPayload[0] == LC_PCK_BROADCAST_REQUEST )
	{
		if (manager->work_mode == LC_WM_CLIENT_STARTED){
			manager->coordinator_address = rx->srcAddr;
			lc_set_work_mode_client(manager, LC_WM_CLIENT_SYNC);
		}

	}else if ( rx->pPayload[0] == LC_PCK_START_VRT ) //Low energy mode(not implemented)
	{
		if (manager->work_mode == LC_WM_CLIENT_WAIT_VRT)
			lc_set_work_mode_client(manager, LC_WM_CLIENT_SEND_VRT);

	}
	else if ( rx->pPayload[0] == LC_PCK_DATA_REQUEST )
	{
		if (manager->work_mode == LC_WM_CLIENT_WAIT)
			lc_set_work_mode_client(manager, LC_WM_CLIENT_SEND_DATA);

	}

}
