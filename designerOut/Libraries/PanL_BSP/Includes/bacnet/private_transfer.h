/*
 * private_transfer.h
 *
 *  Created on: Jun 1, 2017
 *      Author: govind.mukundan
 */

#ifndef SOURCES_PRIVATE_TRANSFER_H_
#define SOURCES_PRIVATE_TRANSFER_H_

#include "apdu.h"

uint8_t Send_Private_Transfer_Request(
    uint32_t device_id,
    uint16_t vendor_id,
    uint32_t service_number,
    char block_number,
	uint8_t * block);

void handler_unconfirmed_private_transfer(
    uint8_t * service_request,
    uint16_t service_len,
    BACNET_ADDRESS * src);

bool ptransfer_send_confirmed_message(uint8_t* pdata, size_t len, uint8_t dmac, uint8_t* txid);

//void handler_confirmed_private_transfer(
//    uint8_t * service_request,
//    uint16_t service_len,
//    BACNET_ADDRESS * src,
//	BACNET_CONFIRMED_SERVICE_DATA * service_data);

#endif /* SOURCES_PRIVATE_TRANSFER_H_ */
