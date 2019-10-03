/*
 * panl_eflash.h
 *
 *  Created on: Aug 29, 2017
 *      Author: prabhakaran.d
 */

#ifndef INCLUDES_PANL_EFLASH_H_
#define INCLUDES_PANL_EFLASH_H_



typedef struct 
{
	uint8_t ManufacturerID;
	uint8_t DeviceID;
	uint32_t JEDECID;
}eflash_info_t;

/**
 *  
 *  @brief Initailze EFlash
 *  @return unique id on sucess otherwise 0
 *   
 */
uint64_t panl_eflash_init();
/**
 *  
 *  @brief Erase sector
 *  @param address to erase
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_eflash_sector_erase(uint32_t addr);
/**
 *  
 *  @brief Erase 64k block
 *  @param address to erase
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_eflash_block_erase(uint32_t addr);
/**
 *  
 *  @brief Read data bytes from eflash
 *  @param address to read
 *  @param data    to read into
 *  @param len     number of bytes to read
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_eflash_read(uint32_t address, uint8_t *data,uint32_t len);
/**
 *  
 *  @brief Read one page
 *  @param address address to read
 *  @param data    to read bytes into
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_eflash_read_page(uint32_t address, uint8_t *data);
/**
 *  
 *  @brief Read one block of memory
 *  @param sector   address to read
 *  @param sec_buff to read into 
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_eflash_read_block(uint16_t sector,uint8_t *sec_buff );
/**
 *  
 *  @brief Program one page
 *  @param address to write
 *  @param data    to write into memory
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_eflash_write(uint32_t address,const uint8_t* data);
/**
 *  
 *  @brief Write a block of memory
 *  @param sector sector to write
 *  @param data   to write into memory
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_eflash_write_block(uint16_t sector,uint8_t* data);
/**
 *  
 *  @brief API to get the Eflash information
 *  @param info to read in 
 *  @return PanL error code PANL_OK on success
 *   
 */
panl_result panl_eflash_info(eflash_info_t *info);


#endif /* INCLUDES_PANL_EFLASH_H_ */
