/*
 * panl_config.h
 *
 *  Created on: Aug 24, 2017
 *      Author: prabhakaran.d
 */

/** @file panl_config.h  PanL35 board configuration */
#ifndef INCLUDES_PANL_CONFIG_H_
#define INCLUDES_PANL_CONFIG_H_

#include <panl.h>


uint16_t PANL_STATUS;


#define MASK_PANL_INIT		0x00ff
#define BIT_RS485_INIT		0
#define MASK_RS485_INIT		(1 << BIT_RS485_INIT)
#define BIT_I2CM_INIT		1
#define MASK_I2CM_INIT		(1 << BIT_I2CM_INIT)
#define BIT_SPI_INIT		2
#define MASK_SPI_INIT		(1 << BIT_SPI_INIT)
#define BIT_SDCARD_INIT		3
#define MASK_SDCARD_INIT	(1 << BIT_SDCARD_INIT)


#define MASK_PANL_ERROR		0xff00
#define BIT_RS485_ERROR		8
#define MASK_RS485_ERROR	(1 << BIT_RS485_ERROR)
#define BIT_I2CM_ERROR		9
#define MASK_I2CM_ERROR		(1 << BIT_I2CM_ERROR)
#define BIT_SPI_ERROR		10
#define MASK_SPI_ERROR		(1 << BIT_SPI_ERROR)
#define BIT_SDCARD_ERROR	11
#define MASK_SDCARD_ERROR	(1 << BIT_SDCARD_ERROR)

typedef void (*panl_register_callback)(void);


typedef enum
{
	panl_rtc_interrupt,
	panl_motion_sensor_interrupt,
	panl_rfid_interrupt,
#if defined(__FT900__)
	panl_mic_interrupt,
#endif
}panl_interrupt_options_t;

typedef struct
{
	panl_register_callback rtc_callback;
	panl_register_callback motionsensor_callback;
	panl_register_callback rfid_callback;
	panl_register_callback mic_callback;
}panl_register_callbacks_t;

panl_register_callbacks_t reg_cb;
/**
 *  @brief Intialize the PanL peripheral devices
 *  @return PanL error code PANL_OK on success
 */ 
panl_result panl_init();

/**
 *  @brief API to get PanL ID
 *  @param id to read in 
 *  @return PanL error code PANL_OK on success
 */
panl_result panl_get_panl_id(uint8_t *id);

/**
 *  @brief Initailze RS485 PORT
 *  @return PanL error code PANL_OK on success
 */
panl_result panl_rs485_init();
/**
 *
 *  @brief Intialize the I2C Channel
 *  @return PanL error code PANL_OK on success
 *
 */
panl_result panl_i2c_init();
/**
 *  
 *  @brief Initialize SD Card 
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_sdcard_init();
/**
 *
 *  @brief Initialize SPI Channel
 *  @param buswidth spi channel buswidth
 *  @return PanL error code PANL_OK on success
 *
 */
panl_result panl_spi_init(uint8_t buswidth);
/** 
 *  @brief Enable interrupts to fire 
 *  @return PanL error code PANL_OK on success  
 */
panl_result panl_interrupt_enable_globally();

/** 
 *  @brief Disable all interrupts
 *  @return PanL error code PANL_OK on success  
 */
panl_result panl_interrupt_disable_globally();

/** 
 *  @brief attach the peripheral interrupts
 *  @return PanL error code PANL_OK on success  
 */
panl_result panl_attach_interrupt(panl_interrupt_options_t panl_interrupt,panl_register_callback cb);

/** 
 *  @brief detach the peripheral interrupts
 *  @return PanL error code PANL_OK on success  
 */
panl_result panl_detach_interrupt(panl_interrupt_options_t panl_interrupt);

/**
 *  @brief Exit PANL
 *  @return NONE
 */
void panl_exit();

/**
 * Get the version of the PANL Library
 * @return A pointer to a string with the version of the panl library
 */
char* panl_get_version(void);

/**
 * Get the features configured
 * @return
 */
uint32_t panl_get_configured_features(void);

panl_board_cfg* panl_get_board_configuration(void);

/**
 * Sets the default pin configuration and feature enable/disable status for supported PANL boards
 * @param board_type [in] Supported board types see board_types.h
 * @param cfg [out] Pointer to uninitialized configuration
 * @return true if a valid configuration was loaded, false if an unrecognized board_type was specified
 */
bool panl_board_cfg_set_defaults(uint32_t board_type, panl_board_cfg* cfg);

/* Additions for Doxygen documenting */
/**
 * @mainpage PanL35 BSP Documentation
 * This documents the PanL35 BSP components. <br>
 *
 *  The BSP is built on the the FT900 MCU Hardware library (libft900)
 *  and contains the following components:
 *  - /bsp : Library that provides a high level access of all the perepherals on PanL35
 *  - /bacnet : Library that provides bacnet-mstp communication with the Raspberry Pi PanLServer
 *  - /fatfs : FAT32 library for the SD card, based on the open source library: http://elm-chan.org/fsw/ff/00index_e.html
 *  - /ftfs : A custom filesystem for the on-board SPI Serial flash
 *  - /tinyprintf : A minimal printf implementation for debugging and logging from http://www.sparetimelabs.com/tinyprintf/tinyprintf.php
 *
 *
 */

#endif /* INCLUDES_PANL_CONFIG_H_ */
