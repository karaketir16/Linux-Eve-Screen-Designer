/*
 * panl35.h
 *
 *  Created on: Aug 14, 2017
 *      Author: prabhakaran.d
 */

#ifndef INCLUDES_BSP_PANL_H_
#define INCLUDES_BSP_PANL_H_

#include <stdint.h>
#include "bsp/panl_boards.h"

#define PANL_LIB_VERSION		   "0.2_alpha"
#define PANL_GPIO_UNUSED		   0xFF
/**
 *
 */
#define RTC_ADDRESS					0xD0
#define TEMP_SENSOR_ADDRESS			0x80
#define SENSOR_ID					0x1050
#define RADIO_ADDRESS				0x22
#define RADIO_ID					0x5804
/**
 *  RGB LED Pin configuration
 */
#define RGB_LED_CLK_PIN				56
#define RGB_LED_DATA_PIN_V1			6
#define RGB_LED_DATA_PIN_V2			57


/**
 * RTC Pin configuration
 */
#define RTC_INT_PIN					43

/**
 *  RS485 Pin configuration
 */
#if defined(__FT900__)
#define RS485_TX					52
#define RS485_RX					53
#define RS485_DATA_ENABLE			10
#elif defined(__FT930__)
#define RS485_TX					27
#define RS485_RX					26
#define RS485_DATA_ENABLE			38
#endif
#define PANL_BAUD_RATE				UART_DIVIDER_921600_BAUD

/**
 *  MOTION SENSOR PIN CONFIGURATION
 */
#define MOTION_SENSOR_PIN			9	


/**
 *  RFID Pin configuration
 */
#if defined(__FT900__)
#define RFID_TX						48
#define RFID_RX						49
#elif defined(__FT930__)
#define RFID_TX						23
#define RFID_RX						22
#endif
#define RFID_BAUD_RATE				UART_DIVIDER_921600_BAUD

/**
 *  PANL ID Pin configuration
 */
#if defined(__FT900__)
#define PANL_ID0_PIN				6
#define PANL_ID1_PIN				7
#define PANL_ID2_PIN				8
#elif defined(__FT930__)
#define PANL_ID0_PIN				34
#define PANL_ID1_PIN				35
#define PANL_ID2_PIN				36
#endif

/**
 *  EFLASH CONFIGURATION
 */
#define PANL_EFLASH_SELECT			3
#define PANL_EFLASH_BUS_WIDTH		4
#define PANL_EFLASH_SELECT_PIN		35
#define PANL_EFLASH_JEDEC_ID_SPI 	0xef4017
#define PANL_EFLASH_JEDEC_ID_QPI    0xef6017

/**
 * DALI CONFIGURATION
 */
#define PANL_DALI_TX			54
#define PANL_DALI_RX			55
#define DALI_TX_MASK 			0x00400000
#define DALI_RX_MASK 			0x00800000

/**
 * CURTAIN CONFIGURATION
 */
#define PANL_CURTAIN_OPEN_CLOSE_TIME			3000	//ms
#define PANL_CURTAIN_OPEN						29
#define PANL_CURTAIN_CLOSE						28
#define PANL_CURTAIN_STOP						25



/**
 *
 */
typedef enum
{
	PANL_NONE,
	PANL_OK,
	PANL_RTC_TIMEOUT,
	PANL_RTC_BUSY,
	PANL_RTC_OSCILLATOR_STOPPED,
	PANL_RTC_ALARM_1,
	PANL_RTC_ALARM_2,
	PANL_RTC_MULTIPLE_ALARM,
	PANL_RTC_WRITE_ERROR,
	PANL_RTC_READ_ERROR,
	PANL_RTC_ERROR,
	PANL_EFLASH_ERROR,
	PANL_EFLASH_ERASE_ERROR,
	PANL_EFLASH_PAGEPROGRAM_ERROR,
	PANL_EFLASH_READ_ERROR,
	PANL_EFLASH_BUSY,
	PANL_EFLASH_QPI_ERROR,
	PANL_RESOURCE_NOT_FOUND,
	PANL_RESOURCE_FOUND,
	PANL_INVALID_OPTION,
	PANL_INVALID_PARAMETER,
	PANL_CARD_INVALID,				
	PANL_INIT_ERROR,
	PANL_EXIT_ERROR,
	PANL_OTHER_ERROR,
	PANL_TIME_OUT,	
}panl_result;



#include <string.h>
#include <ft900.h>
#include "bsp/panl_pin_map.h"

#include "ftfs/ftfs.h"
#include "fatfs/ff.h"
#include "fatfs/diskio.h"


/* Disable the by-default overrides used by tinyprintf
#define TINYPRINTF_DEFINE_TFP_PRINTF 0
#define TINYPRINTF_DEFINE_TFP_SPRINTF 0
#define TINYPRINTF_OVERRIDE_LIBC 0
*/
#include "tinyprintf/tinyprintf.h"
#include "bsp/panl_config.h"
#include "bsp/panl_rgbled.h"
#include "bsp/panl_temperature_sensor.h"
#include "bsp/panl_rtc.h"
#include "bsp/panl_radio.h"
#include "bsp/panl_motion_sensor.h"
#include "bsp/panl_rfid.h"
#include "bsp/panl_eflash.h"
#include "bsp/panl_mic.h"
#include "bsp/panl_timer.h"
#include "bsp/panl_dali.h"
#include "bsp/panl_curtain.h"
#include "bacnet/panl_bacnet.h"
#include "utils/panl_util.h"
#include "utils/heap_4.h"



#endif /* INCLUDES_BSP_PANL_H_ */
