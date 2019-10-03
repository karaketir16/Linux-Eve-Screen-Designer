/*
 * panl_mic.h
 *
 *  Created on: Aug 29, 2017
 *      Author: prabhakaran.d
 */

/** @file panl_mic.h  PanL35 MIC control API */
#ifndef INCLUDES_PANL_MIC_H_
#define INCLUDES_PANL_MIC_H_

#include <panl.h>
/**
 *  @brief Initialize the microphone
 *  @return PanL error code PANL_OK on success
 */
panl_result panl_mic_init();
/**
 *
 *  @brief read out the samples
 *  @param buff to read in
 *  @param len number of bytes to read
 *  @return NONE
 *
 */
void panl_mic_readn(uint8_t *buff,uint16_t len);

/**
 *  @brief get available bytes
 *  @return PanL error code PANL_OK on success
 */
uint32_t panl_mic_available();
/**
 *  @brief De-initialize microphone
 *  @return PanL error code PANL_OK on success
 *
 */
panl_result panl_mic_exit();


#endif /* INCLUDES_PANL_MIC_H_ */
