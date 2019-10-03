
/** @file panl_temperature_sensor.h  PanL Temperature sensor interface */
#ifndef INCLUDES_BSP_TEMPERATURE_H_
#define INCLUDES_BSP_TEMPERATURE_H_
#include <stdint.h>
/**
 *  
 */
typedef enum
{
	humidity_resolution,
	temperature_resolution, 	
	acquisition_mode,
}sensor_options_t;

/**
 *  
 */
typedef enum
{
	humidity_resolution_14bit = 0,
	humidity_resolution_11bit, 	
	humidity_resolution_8bit , 	
}humidity_resolution_t;

/**
 *  
 */
typedef enum
{
	temperature_resolution_14bit = 0,
	temperature_resolution_11bit, 		
}temperature_resolution_t;


/**
 *  
 */
typedef enum
{
	normal = 0,
	temperature_first, 		
}acquisition_mode_t;

/**
 *  
 */
typedef struct
{
	uint16_t ManufacturerID ;
	uint16_t DeviceID;
}sensor_info_t;

/**
 *  
 *  @brief Intialize the sensor
 *  @return PanL error code, PANL_OK on success
 *   
 */
panl_result panl_temp_sensor_init();
/**
 *  
 *  @brief
 *  @param option sensor option to write into
 *  @param val    value of the option to write into
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_temp_sensor_options(sensor_options_t option,uint8_t val);
/**
 *  
 *  @brief Read the temperature
 *  @param t pointer to read into
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_get_temperature(float *t);
/**
 *  
 *  @brief Read the humidity
 *  @param t pointer to read into
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_get_humidity(float *h);
/**
 *  
 *  @brief Reset the sensor
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_temp_sensor_reset();
/**
 *
 *  @brief Check the temperature sensor is present or not
 *  @return PanL error code PANL_OK on success
 *
 */
panl_result panl_temp_sensor_ispresent();

/**
 *
 *  @brief Read the temperature sensor info
 *  @param info pointer to read into
 *  @return PanL error code PANL_OK on success
 *
 */
panl_result panl_temp_sensor_info(sensor_info_t *info);

#endif
