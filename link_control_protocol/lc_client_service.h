/*
 * lc_client_service.h
 *
 *  Created on: 15/01/2015
 *      Author: David Camacho
 */
#include "lc_common.h"

#ifndef LC_CLIENT_SERVICE_H_
#define LC_CLIENT_SERVICE_H_


void lc_initialize_client(LinkControlClientManager *manager);
void lc_set_work_mode_client(LinkControlClientManager *manager, UINT8 work_mode);
void lc_register_device_client(LinkControlClientManager *manager, UINT16 addr);

void lc_sending_service_update_client(LinkControlClientManager *manager, BASIC_RF_TX_INFO *tx, BASIC_RF_RX_INFO *rx);
void lc_reception_service_update_client(LinkControlClientManager *manager, BASIC_RF_TX_INFO *tx, BASIC_RF_RX_INFO *rx);




#endif /* LC_CLIENT_SERVICE_H_ */
