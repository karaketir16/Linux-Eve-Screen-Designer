/*
 * ft9xx_pin_map.h
 *
 *  Created on: Oct 20, 2017
 *      Author: govind.mukundan
 */

#ifndef INCLUDES_BSP_PANL_PIN_MAP_H_
#define INCLUDES_BSP_PANL_PIN_MAP_H_

#if defined (__FT900__)
// UART
#define GPIO_UART0_TX 	(48)
#define GPIO_UART0_RX 	(49)
#define GPIO_UART1_TX 	(52)
#define GPIO_UART1_RX 	(53)

// SDHC
#define GPIO_SD_CLK  (19)
#define GPIO_SD_CMD  (20)
#define GPIO_SD_DAT3 (21)
#define GPIO_SD_DAT2 (22)
#define GPIO_SD_DAT1 (23)
#define GPIO_SD_DAT0 (24)
#define GPIO_SD_CD   (25)
#define GPIO_SD_WP   (26)

// I2CM
#define GPIO_I2CM_SCL	(44)
#define GPIO_I2CM_SDA	(45)
#define PAD_I2CM_SCL	(pad_i2c0_scl)
#define PAD_I2CM_SDA	(pad_i2c0_sda)

// SPIM
#define GPIO_SPIM_CLK	(27)
#define GPIO_SPIM_MISO	(30)
#define GPIO_SPIM_MOSI	(29)
#define GPIO_SPIM_SS0	(28)
#define GPIO_SPIM_SS1	(33)
#define GPIO_SPIM_SS2	(34)
#define PAD_SPIM_SS0    (pad_spim_ss0)
#define PAD_SPIM_SS1    (pad_spim_ss1)
#define PAD_SPIM_SS2    (pad_spim_ss2)
#define GPIO_SPIM_IO2    (31)
#define PAD_SPIM_IO2     (pad_spim_io2)
#define GPIO_SPIM_IO3    (32)
#define PAD_SPIM_IO3     (pad_spim_io3)

#elif defined (__FT930__)
// UART
#define GPIO_UART0_TX 	(23)
#define GPIO_UART0_RX 	(22)
#define GPIO_UART1_TX 	(27)
#define GPIO_UART1_RX 	(26)

// SDHC
#define GPIO_SD_CLK  (0)
#define GPIO_SD_CMD  (1)
#define GPIO_SD_DAT3 (6)
#define GPIO_SD_DAT2 (5)
#define GPIO_SD_DAT1 (4)
#define GPIO_SD_DAT0 (3)
#define GPIO_SD_CD   (2)
#define GPIO_SD_WP   (7)

// I2CM
#define GPIO_I2CM_SCL	(12)
#define GPIO_I2CM_SDA	(13)
#define PAD_I2CM_SCL	(pad_i2cm_scl)
#define PAD_I2CM_SDA	(pad_i2cm_sda)

// SPIM - not used with 930 boards for now

#endif





#endif /* INCLUDES_BSP_PANL_PIN_MAP_H_ */
