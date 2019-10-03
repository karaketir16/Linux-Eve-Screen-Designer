
/** @file panl_rtc.h  PanL RTC interface */

#ifndef PANL35_RTC_H
#define PANL35_RTC_H
#include <stdint.h>



/**
 *  Square wave types
 */
typedef enum 
{
	square_wave_1hz = 0x04,
	battery_backed_sq_wave_1hz = 0x40,
	square_wave_32khz = 0x88,
	battery_backed_sq_wave_32khz = 0xc0,
}panl_rtc_square_wave_t;
/**
 *  Real time clock formats
 */
typedef enum
{
	time_format_24 = 0x00,
	time_format_12 = 0x40,
}panl_rtc_time_format_t;
/**
 *  Alarm types
 */
typedef enum
{
	once_a_second = 0x0f,
	seconds_match = 0x0e,
	mins_seconds_match = 0x0c,
	hours_mins_seconds_match = 0x08,	
	date_hours_mins_seconds_match = 0x00,
	day_hours_mins_seconds_match = 0x10,
	once_a_minute = 0x8e,
	minutes_match = 0x8c,
	hours_minutes_match = 0x88,
	date_hours_mins_match = 0x80,
	day_hours_mins_match = 0x90,
}panl_rtc_alarm_type_t;

/**
 *  Real time clock interrupts
 */
typedef enum 
{
	alarm1_interrupt = 1,
	alarm2_interrupt = 2,
	alarm_all_interrupts = 3,
}panl_rtc_interrupt_t;

/**
 *  
 */
typedef struct 
{
	uint8_t sec;
	uint8_t min;
	uint8_t hour;		
	uint8_t day;	
	uint8_t date;
	uint8_t month;
	uint8_t year;
}panl_time_t;

/**
 *  
 *  @brief Initialize Real time clock
 *  @param cb callback function to register
 *  @return PanL error code PANL_OK on success
 *
 */
panl_result panl_rtc_init(panl_register_callback cb);
/**
 *  
 *  @brief Enable the Square Wave.
 *  @param option type of square wave to enable
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_rtc_enable_squarewave(panl_rtc_square_wave_t option);
/**
 *  
 *  @brief Disable the Square wave 
 *  @param option type of square wave to disable
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_rtc_disable_squarewave(panl_rtc_square_wave_t option);
/**
 *  
 *  @brief Enable the interrupt
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_rtc_enable_interrupts_globally();
/**
 *  
 *  @brief Disable the interrupt globally 
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_rtc_disable_interrupts_globally();
/**
 *  
 *  @brief Enable the interrupt 
 *  @param _interrupt type of interrupt to enable
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_rtc_enable_interrupt(panl_rtc_interrupt_t _interrupt);
/**
 *  
 *  @brief Disbale the interrupt
 *  @param _interrupt type of interrupt to disable
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_rtc_disable_interrupt(panl_rtc_interrupt_t _interrupt);
/**
 *  
 *  @brief
 *  @param _interrupt type of interrupt to clear
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_rtc_clear_interrupt(panl_rtc_interrupt_t _interrupt);

/**
 *  
 *  @brief Set the Time and Date
 *  @param t to set the real time clock
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_rtc_set_time(panl_time_t t);
/**
 *  
 *  @brief get the current value of Real time clock
 *  @param t pointer to store the real time clock values
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_rtc_get_time(panl_time_t *t);
/**
 *  
 *  @brief
 *  @param type alarm type to set
 *  @param t    Alarm time to set
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_rtc_set_alarm(panl_rtc_alarm_type_t type,panl_time_t t);
/**
 *  
 *  @brief
 *  @param aType Parameter_Description
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_rtc_clear_alarm(panl_rtc_alarm_type_t aType);
/**
 *  
 *  @brief
 *  @param address  ram location to write
 *  @param buffer	pointer to the array to write
 *  @param len 		The number of bytes to write 
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_rtc_write_ram(uint8_t address,uint8_t *buffer,uint8_t len);
/**
 *  
 *  @brief
 *  @param address ram location to read
 *  @param buffer  pointer to the array to read into
 *  @param len     The number of bytes to write 
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_rtc_read_ram(uint8_t address,uint8_t *buffer,uint8_t len);
/**
 *  
 *  @brief Read the temperature
 *  @param t pointer to store the current temperature
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_rtc_get_temperature(int8_t *t);
/**
 *  
 *  @brief Reset the Real time clock
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_rtc_softreset();

/**
 *  
 *  @brief Read the real time clock status
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_rtc_get_status();

#endif

