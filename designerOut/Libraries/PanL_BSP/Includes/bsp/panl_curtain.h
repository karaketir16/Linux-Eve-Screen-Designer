/*
 * panl_curtain.h
 *
 *  Created on: Oct 17, 2017
 *      Author: prabhakaran.d
 */

#ifndef INCLUDES_PANL_CURTAIN_H_
#define INCLUDES_PANL_CURTAIN_H_
/**
 *  
 *  @brief Intailze the curtain control
 *  @return None
 *   
 */
void panl_curtain_init();

/**
 *  
 *  @brief API to use to open the curtain 
 *  @param val percentage to open [0 - 100]
 *  @return 0 on Idle otherwise -1
 *   
 */
int8_t panl_curtain_open(uint8_t val);
/**
 *  
 *  @brief API to use to close the curtain
 *  @param val percentage to close [0~100]
 *  @return None
 *   
 */
uint8_t panl_curtain_actual_position();
/**
 *  
 *  @brief De-Intailze the curtain control
 *  @return PanL error code PANL_OK on success
 *   
 */
void panl_curtain_exit();

/**
 *  @brief API to get the state of the curtain
 *  @return true if busy otherwise false
 */
bool panl_curtain_is_busy();

#endif /* INCLUDES_PANL_CURTAIN_H_ */
