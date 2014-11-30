#include "lc_common.h"

#ifndef _LC_SERVICE_H_
#define _LC_SERVICE_H_

void lc_initialize(LinkControlManager *manager);
void lc_set_work_mode(LinkControlManager *manager, UINT8 work_mode);
void lc_register_device(LinkControlManager *manager, UINT16 addr);

void lc_sending_service_update(LinkControlManager *manager, BASIC_RF_TX_INFO *tx, BASIC_RF_RX_INFO *rx);
void lc_reception_service_update(LinkControlManager *manager, BASIC_RF_TX_INFO *tx, BASIC_RF_RX_INFO *rx);

#endif
