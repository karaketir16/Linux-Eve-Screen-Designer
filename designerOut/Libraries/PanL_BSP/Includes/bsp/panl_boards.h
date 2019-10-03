/*
 * panl_boards.h
 *
 *  Created on: Nov 2, 2017
 *      Author: govind.mukundan
 */

#ifndef INCLUDES_BSP_PANL_BOARDS_H_
#define INCLUDES_BSP_PANL_BOARDS_H_

#include <stdint.h>

/* PANL boards come in different variants, these are defined here */
#define PANL35_GENERIC_V1			1
#define PANL35_GENERIC_V2			2
#define PANL_RELAY_CONTROLLER		3
#define PANL_DALI_CONTROLLER		4
#define PANL70_GENERIC_V1			5
#define PANL70_MM900EV_LITE			6
#define PANL70_PLUS_V2				7

/* Features available on PanL */
#define FUNCTION_RS485				((1UL) << 0)
#define FUNCTION_SDIO				((1UL) << 1)
#define FUNCTION_DALI				((1UL) << 2)
#define FUNCTION_RELAY				((1UL) << 3)
#define FUNCTION_LED				((1UL) << 4)
#define FUNCTION_MOTION_SENSOR		((1UL) << 5)
#define FUNCTION_RTC				((1UL) << 6)
#define FUNCTION_EFLASH				((1UL) << 7)
#define FUNCTION_CARD_READER		((1UL) << 8)
#define FUNCTION_PDM_MIC			((1UL) << 9)

#define FUNCTION_XXX				((1UL) << 31)
#define FUNCTION_PANL35_DEFAULT		(FUNCTION_RS485 | FUNCTION_SDIO | FUNCTION_LED | FUNCTION_MOTION_SENSOR | FUNCTION_RTC | FUNCTION_EFLASH \
									 FUNCTION_CARD_READER | FUNCTION_PDM_MIC)



typedef struct{
	uint8_t ID0;
	uint8_t ID1;
	uint8_t ID2;
	uint8_t LED_CLK;
	uint8_t LED_DATA;
	uint8_t RS485_DE;
	uint8_t MOTION_SENSOR;
	uint8_t DALI_TX;
	uint8_t DALI_RX;
	uint8_t CURTAIN_CLOSE;
	uint8_t CURTAIN_OPEN;
	uint8_t CURTAIN_STOP;
	uint8_t EFLASH_SEL;
	uint8_t RTC_INT;
	uint8_t MIC_INT;
	uint8_t SDIO_PRESENT;
	uint8_t RFID_PRESENT;
	uint8_t RTC_IS_INTERNAL;
}panl_board_cfg;

#endif /* INCLUDES_BSP_PANL_BOARDS_H_ */
