/*
 * panl_motion_sensor.h
 *
 *  Created on: Aug 25, 2017
 *      Author: prabhakaran.d
 */
/** @file panl_motion_sensor.h  PanL Motion sensor interface */
#include <panl.h>


/**
 *  
 *  @brief Initalize the motion sensor
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_motion_sensor_init();

/**
 *  
 *  @brief Release the motion sensor
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_motion_sensor_exit();

/**
 *  
 *  @brief API to get the motion detection status
 *  @return 1 on motion detection otherwisw 0
 *   
 */
uint8_t panl_is_motion_detected();
/**
 *
 *  @brief API to get the motion detection status
 *  @return Motion detection status
 *
 */
uint8_t panl_get_motionsensor_status();

/**
 *
 *  @brief Clear the motion detection status
 *  @return PanL error code PANL_OK on success
 *
 */
panl_result panl_clear_motionsensor_status();

