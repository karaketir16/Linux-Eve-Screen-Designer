
/** @file panl_rgbled.h  PanL RGB LEDinterface */

#ifndef INCLUDES_BSP_RGBLED_H_
#define INCLUDES_BSP_RGBLED_H_
#include <stdint.h>
#include "colornames.h"


#define LED_INTENSITY_MAX		31u
#define LED_INTENSITY_HIGH		LED_INTENSITY_MAX
#define LED_INTENSITY_MEDIUM	16u
#define LED_INTENSITY_LOW		8u
#define LED_INTENSITY_OFF		0u

typedef struct{
	uint8_t i; /* Intensity [0 - 31]*/
	uint8_t r;
	uint8_t g;
	uint8_t b;
}pixel;

/**
 *  
 *  @brief Intialize the RGB LED 
 *  @return None
 *   
 */
void panl_led_init(void);

/**
 *  
 *  @brief Set the LED Color with the given intensity
 *  @param color_name color value to set. Friendly names are defined in colornames.h
 *  @param intensity varies from 0 - 31
 *  @return None
 *   
 */
void panl_led_set(uint32_t color_name, uint8_t intensity);


/**
 *
 * @param color_name The color to toggle
 */
void panl_led_toggle(uint32_t color_name);

#endif
