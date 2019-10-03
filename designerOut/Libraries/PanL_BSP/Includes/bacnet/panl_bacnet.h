/*
 * panl_bacnet.h
 *
 *  Created on: Sep 7, 2017
 *      Author: govind.mukundan
 */

#ifndef INCLUDES_BACNET_PANL_BACNET_H_
#define INCLUDES_BACNET_PANL_BACNET_H_

#include <stdint.h>
#include <stdbool.h>
#include "dlmstp.h"

/* Called when we receive a message from the server or another node */
typedef bool (*bacnet_msg_received_cb)(uint8_t src,
										const uint8_t* indata,
										const size_t inlen,
										uint8_t* outdata,
										size_t* outlen );

/* Called when we receive an ACK from the server on receiving a notification */
typedef void (*bacnet_notification_acked_cb)(uint8_t id);

/* Called when we receive an unconfirmed message (these could be unicast or broadcast) */
typedef void (*bacnet_unconfirmed_msg_received_cb)(uint8_t src,
										const uint8_t* indata,
										const size_t inlen );

/* Interface data */
typedef struct{
	mstp_config_t config;
	uint8_t mac;
	bacnet_msg_received_cb msg_rxed_cb;
	bacnet_notification_acked_cb notif_ack_cb;
	bacnet_unconfirmed_msg_received_cb unconf_msg_rxed_cb;
}panl_bacnet_cfg_s;


bool panl_bacnet_init(panl_bacnet_cfg_s* cfg);

void panl_bacnet_task(void);

uint8_t panl_bacnet_send_notification(uint8_t dst, uint8_t* pdata, size_t len);


bool panl_bacnet_conf_tx_slot_available(void);
uint8_t panl_bacnet_get_server_mac(void);
uint8_t panl_bacnet_get_my_mac(void);


#endif /* INCLUDES_BACNET_PANL_BACNET_H_ */
