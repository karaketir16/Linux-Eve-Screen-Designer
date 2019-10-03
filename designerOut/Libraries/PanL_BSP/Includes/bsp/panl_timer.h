/*
 * panl_timer.h
 *
 *  Created on: Oct 19, 2017
 *      Author: govind.mukundan
 */

#ifndef INCLUDES_BSP_PANL_TIMER_H_
#define INCLUDES_BSP_PANL_TIMER_H_

// Two of these are used by the library itself, 3 are available for application.
#define MAX_MS_TIMER_CALL_BACKS				5

#define TIMER_CHANNEL						timer_select_a
#define DALI_TIMER							timer_select_b

typedef void (*MSTimerCb) (void); // callback for application to access the a 1mS timer
typedef void (*uSTimerCb) (void); // callback for application to access the a 1mS timer

uint32_t panl_timer_get_time(void);
bool panl_timer_is_time_expired(uint32_t start, uint32_t period);
void panl_timer_init(void);
void panl_timer_dali_init(void);
bool panl_timer_register_ms_callback(MSTimerCb cb);
void panl_timer_register_us_callback(uSTimerCb cb);


#endif /* INCLUDES_BSP_PANL_TIMER_H_ */
