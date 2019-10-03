/*
 * dllmstp.h
 *
 *  Created on: Apr 25, 2017
 *      Author: govind.mukundan
 */

#ifndef INCLUDES_DLLMSTP_H_
#define INCLUDES_DLLMSTP_H_

#include "dlmstp.h"

typedef struct{
	uint32_t dev_instance; // 22 bit system wide unique device address
	uint8_t mac;	// Your network wide unique mac address
	uint8_t max_master_mac; // Highest address of a master node
	uint8_t max_info_frames; // Max number of frames you can send when you have a token
	uint32_t baud; // pre-configured baud rate
	uint8_t dest; // a fixed destination that we will message
	uint32_t packet_len; // Length of the packets sent to the fixed destination
	mstp_config_t config_type; // configuration loaded from SD card or auto detected

}bacnet_cfg;

/* At boot-up / after reset, the node should always start at Learning */
typedef enum{
	Learning,
	Configured
}auto_baud_states;
auto_baud_states AutoBaudState;


/* Disable the slave FSM code if you're a MASTER device */
#define SUPPORT_SLAVE_MODE	0

 uint32_t get_silence_timer(void* arg);
 void reset_silence_timer(void* arg);
 uint32_t dlmstp_get_cnt(void);
 auto_baud_states dlmstp_get_auto_baud_state(void);
 void dlmstp_on_invalid_data(int num);
 void dlmstp_on_valid_data(int num);
 bacnet_cfg* dlmstp_get_cfg(void);


#endif /* INCLUDES_DLLMSTP_H_ */
