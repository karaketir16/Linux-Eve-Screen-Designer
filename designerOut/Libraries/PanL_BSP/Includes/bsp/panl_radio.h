#ifndef INCLUDES_BSP_RADIO_H_
#define INCLUDES_BSP_RADIO_H_

/** @file panl_radio.h  PanL Radiointerface */
#include <stdint.h>
/**
 *  
 */
typedef enum
{
	radio_option_channel_band,
	radio_option_channel_space,	
	radio_option_clock_mode,
	radio_option_audio_ctrl,
	radio_option_deemphasis,
}radio_options;

/**
 *  
 */
typedef enum
{
	fm_usa_europe = 0,
	fm_japan = 1,
	fm_world = 2,
	fm_east_europe = 3,
}channel_band_t;
/**
 *  
 */
typedef enum
{
	channel_space_100khz = 0,
	channel_space_200khz = 1,
	channel_space_50khz = 3,
	channel_space_25khz = 4,
}channel_space_t;
/**
 *  
 */
typedef enum
{
	clock_mode_32768hz = 0,
	clock_mode_12mhz,
	clock_mode_13mhz,
	clock_mode_19200khz,
	clock_mode_24mhz = 5,
	clock_mode_26mhz,
	clock_mode_38400khz,
}clock_mode_t;
/**
 *  
 */
typedef enum
{
	audio_disable = 0,
	audio_enable,
	audio_mute,
	audio_unmute,	
	audio_stereo,
	audio_mono,
	audio_bass_on,
	audio_bass_off,	
}audio_option_t;
 
typedef enum
{
	deemphasis_75us = 0,
	deemphasis_50us,
}deemphasis_t;

/**
 *  
 *  @brief Initialize the radio
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_radio_init();

/**
 *  
 *  @brief De Initialize radio [Power off]
 *  @return PanL error code PANL_OK on success
 *
 */
panl_result panl_radio_exit();
/**
 *
 *  @brief Check radio is available
 *  @return PanL error code PANL_OK on success
 *
 */
panl_result panl_radio_ispresent();
/**
 *
 *  @brief Configure the radio options
 *  @param opt   function options
 *  @param value option value to write into
 *  @return PanL error code PANL_OK on success
 *
 */
panl_result panl_radio_options(radio_options opt,uint8_t val);
/**
 *
 *  @brief Set the Radio volume
 *  @param val volume range to set
 *  @return PanL error code PANL_OK on success
 *
 */
panl_result panl_radio_set_volume(uint8_t val);
/**
 *
 *  @brief Set thr channel frequency
 *  @param freq to configure FM receiver
 *  @return PanL error code PANL_OK on success
 *
 */
panl_result panl_radio_set_frequency(uint16_t freq);

/**
 *  
 *  @brief Print FM receiver registers
 *  @return PanL error code PANL_OK on success
 *   
 */
void panl_radio_report();

#endif
