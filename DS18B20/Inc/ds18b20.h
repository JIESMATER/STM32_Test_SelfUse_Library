#ifndef _DS18B20_H_
#define _DS18B20_H_

#include "main.h"

#define DS18B20_DQ_OUT_HIGH HAL_GPIO_WritePin(GPIOG,GPIO_PIN_11,GPIO_PIN_SET)
#define DS18B20_DQ_OUT_LOW 	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_11,GPIO_PIN_RESET)

#define DS18B20_DQ_IN	 HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_11)


void DS18B20_IO_IN(void);
void DS18B20_IO_OUT(void);

uint8_t DS18B20_Init(void);
void DS18B20_Rst(void);
uint8_t DS18B20_Check(void);

uint8_t DS18B20_Read_Bit(void);
uint8_t DS18B20_Read_Byte(void);
void DS18B20_Write_Byte(uint8_t data);

void DS18B20_Start(void);
short DS18B20_Get_Temperature(void);

#endif
