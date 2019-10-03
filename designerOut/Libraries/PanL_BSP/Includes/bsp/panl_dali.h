/*
 * panl_dali.h
 *
 *  Created on: Oct 12, 2017
 *      Author: prabhakaran.d
 */

#ifndef INCLUDES_PANL_DALI_H_
#define INCLUDES_PANL_DALI_H_
/**
 *  Commnad options
 */
typedef enum
{
	direct_arc_power = 0,
	command,
}dali_selector_t;

/**
 *  Address types  
 */
typedef enum
{
	short_address,
	group_address = 0x80,
	broadcast = 0xfe,
}dali_address_options_t;

/**
 * Ballast config options 
 */
typedef enum
{
	config_max_level,
	config_min_level,
	config_system_failure_level,
	config_power_on_level,
	config_fade_time,
	config_fade_rate,
	config_scene0,
	config_scene1,
	config_scene2,
	config_scene3,
	config_scene4,
	config_scene5,
	config_scene6,
	config_scene7,
	config_scene8,
	config_scene9,
	config_scene10,
	config_scene11,
	config_scene12,
	config_scene13,
	config_scene14,
	config_scene15,
}dali_config_options_t;
/**
 *  Scene Options
 */
typedef enum
{
	scene0,
	scene1,
	scene2,
	scene3,
	scene4,
	scene5,
	scene6,
	scene7,
	scene8,
	scene9,
	scene10,
	scene11,
	scene12,
	scene13,
	scene14,
	scene15,
}dali_scene_options_t;
/**
 * Query options 
 */
typedef enum
{
	query_device_status = 0,
	query_ballast,
	query_lamp_failure,
	query_lamp_power_on,
	query_limit_error,
	query_reset_state,
	query_missing_short_address,
	query_version_number,
	query_content_dtr,
	query_device_type,
	query_physical_minimum_level,
	query_power_failure,
	query_actual_level,
	query_max_level,
	query_min_level,
	query_power_on_level,
	query_system_failure_level,
	query_fade_time_rate,
	query_scene0,
	query_scene1,
	query_scene2,
	query_scene3,
	query_scene4,
	query_scene5,
	query_scene6,
	query_scene7,
	query_scene8,
	query_scene9,
	query_scene10,
	query_scene11,
	query_scene12,
	query_scene13,
	query_scene14,
	query_scene15,
	query_groups_0_7,
	query_groups_8_15,
	query_random_address_h,
	query_random_address_m,
	query_random_address_l,
}dali_query_options_t;

/**
 *  
 *  @brief DALI Protocol intialization
 *  @return None
 *   
 */
void panl_dali_init();
/**
 *  
 *  @brief De-INITIALISE Dali Protocol
 *  @return PanL error code PANL_OK on success
 *   
 */
void panl_dali_exit();

/**
 * @brief Get the total number of devices discovered
 * @return
 */
uint16_t panl_dali_get_device_count(void);
/**
 * @brief Set the total number of devices discovered
 * @return none
 */
void panl_dali_set_device_count(uint8_t d);
/**
 *  
 *  @brief Check the DALI bus lines are in idle state
 *  @return 1 on Idle otherwise 0
 *   
 */
uint8_t panl_dali_is_idle();
/**
 *  
 *  @brief API to check the Dali received data is available
 *  @return 1 on available otherwise 0
 *   
 */
uint8_t panl_dali_is_data_available();
/**
 *  
 *  @brief Read the available received data 
 *  @return 8bit data 
 *   
 */
uint8_t panl_dali_read();
/**
 *  
 *  @brief API to scan the devices conneted in the netowrk
 *  @return number of devices conneted
 *   
 */
uint8_t panl_dali_scan();
/**
 *  
 *  @brief API to test the bus lines
 *  @return None
 *   
 */
void panl_dali_bus_test();
/**
 *  
 *  @brief Query the Ballast parameters
 *  @param address device address to request from
 *  @param opt     query options
 *  @return return 8bit data 
 *   
 */
uint8_t panl_dali_query(uint8_t address,dali_query_options_t opt);
/**
 *  
 *  @brief API to write the DALI Parameters
 *  @param address device address to write into
 *  @param opt     Parameter options to write 
 *  @param val     value to write into
 *  @return None
 *   
 */
void panl_dali_write(uint8_t address,dali_config_options_t opt,uint8_t val);
/**
 *  
 *  @brief API to update scene levels
 *  @param address device address to write into
 *  @param no      scene number
 *  @param val     value to write into 
 *  @return None
 *   
 */

void panl_dali_update_scene(uint8_t address,dali_scene_options_t no,uint8_t val);
/**
 *  
 *  @brief API to set the ballast scene
 *  @param address device address to set
 *  @param no      Scene no
 *  @return None
 *   
 */
void panl_dali_goto_scene(uint8_t address,dali_scene_options_t no);
/**
 *  
 *  @brief API to get the ballast scene levels
 *  @param address device address to read from
 *  @param no      Scene number
 *  @return 8bit data
 *   
 */
uint8_t panl_dali_get_scene(uint8_t address,dali_scene_options_t no);
/**
 *  
 *  @brief API to get the BALLAST type
 *  @param device address to read from
 *  @return 8bti device type
 *   
 */
uint8_t panl_dali_get_device_type(uint8_t address);
/**
 *  
 *  @brief API to get the BALLAST Status
 *  @param address device address to read from
 *  @return 8bit status
 *   
 */
uint8_t panl_dali_get_device_status(uint8_t address);
/**
 *  
 *  @brief API to get the Version number
 *  @param device address to read from
 *  @return 8bit version number
 *   
 */
uint8_t panl_dali_get_version_number(uint8_t address);
/**
 *  
 *  @brief API to Switch on the Lamp [Switch to max level]
 *  @param device address to write into
 *  @return None
 *   
 */
void panl_dali_lamp_on(uint8_t address);
/**
 *  
 *  @brief API to switch off the lamp
 *  @param address device address to write into
 *  @return None
 *   
 */
void panl_dali_lamp_off(uint8_t address);
/**
 *  
 *  @brief Set the lamp brightness
 *  @param address device address to write into
 *  @param val     value to write
 *  @return None
 *   
 */
void panl_dali_lamp_set_brightness(uint8_t address,uint8_t val);
/**
 *
 *  @brief Add the Device to group form 0 - 15
 *  @param address to add into group
 *  @param group_no to add into
 *  @return None
 *
 */
void panl_dali_add_to_group(uint8_t address,uint8_t group_no);
/**
 *
 *  @brief Remove device from group form 0 - 15
 *  @param address to remove from group
 *  @param group_no to remove from
 *  @return None
 *
 */
void panl_dali_remove_from_group(uint8_t address,uint8_t group_no);
/**
 *
 *  @brief Add the Device to group form 0 - 15
 *  @param group_no to add into
 *  @param number_of_ballasts number of lights to add
 *  @param ... Device address
 *  @return none
 *
 */
void panl_dali_add_to_groupn(uint8_t group_no,uint8_t number_of_ballasts, ...);
/**
 *
 *  @brief remove device from group form 0 - 15
 *  @param group_no to remove from
 *  @param number_of_ballasts to remove
 *  @param ... Device address
 *  @return none
 *
 */
void panl_dali_remove_from_groupn(uint8_t group_no,uint8_t number_of_ballasts, ...);

#endif /* INCLUDES_PANL_DALI_H_ */
