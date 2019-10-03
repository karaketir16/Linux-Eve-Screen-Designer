/*
 * FT8xxHal.h
 *
 *  Created on: 10 Mar 2017
 *      Author: prabhakaran.d
 */

#ifndef FT8XXHAL_H_
#define FT8XXHAL_H_

typedef enum
{
	WRITE,
	READ,
}TRANSFER;



extern void FT8xxPinConfig(uint8_t cs,uint8_t pdn, uint8_t intPin);
extern void FT8xxBootupConfig();
uint8_t rd8(uint32_t address);
extern uint16_t rd16(uint32_t address);
extern uint32_t rd32(uint32_t address);
extern uint32_t rd8s(uint32_t address,uint8_t *buffer,uint16_t size);
extern void wr8(uint32_t address,uint8_t data);
extern void wr16(uint32_t address,uint16_t data);
extern void wr32(uint32_t address,uint32_t data);
extern void wrCmd(uint32_t data);
extern void wr8s(uint32_t address,uint8_t *data, uint32_t len);
extern void writeString(const char *string);
extern void StartTransfer(uint32_t address,TRANSFER t);
extern void EndTransfer();
extern void FT8xxSetExtended();
extern void FT8xxSetPBVolume(uint8_t volume);
extern void FT8xxSetSSVolume(uint8_t volume);
extern uint16_t FT8xxGetTouchX(uint8_t touchno);
extern uint16_t FT8xxGetTouchY(uint8_t touchno);
extern void FT8xxPlaySound(uint8_t sound, uint8_t midi,uint8_t volume);
extern void FT8xxStopSound();
extern void FT8xxSetBacklight(uint8_t val);
extern uint16_t gpu_writepointer;
extern uint16_t gpu_freespace;

#endif /* FT8XXHAL_H_ */
