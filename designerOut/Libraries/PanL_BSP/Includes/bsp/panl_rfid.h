/*
 * panl_rfid.h
 *
 *  Created on: Aug 26, 2017
 *      Author: prabhakaran.d
 */

/** @file panl_rfid.h  PanL RFID interface */
#ifndef INCLUDES_PANL_RFID_H_
#define INCLUDES_PANL_RFID_H_

#include <panl.h>

#define RXBUFFER_SIZE				16
#define START_OF_TEXT				0x02
#define CARRIAGE_RETURN				0x0D
#define LINE_FEED					0x0A
#define END_OF_TEXT					0x03
#define SERIAL_NUMBER_LEN			10

/**
 *
 *  @brief Intialize RFID Module
 *  @param cb callback function to register
 *  @return PanL error code PANL_OK on success
 *
 */
panl_result panl_rfid_init(panl_register_callback cb);

/**
 *  @brief Get the RFID Card serial number information
 *  @param str Parameter_Description
 *  @return PanL error code PANL_OK on success
 */
panl_result panl_get_rfid_card_info(char* str);

/**
 *  @brief De-Intialize the RFID Port
 *  @return PanL error code PANL_OK on Success
 */
panl_result panl_rfid_exit();


#endif /* INCLUDES_PANL_RFID_H_ */
