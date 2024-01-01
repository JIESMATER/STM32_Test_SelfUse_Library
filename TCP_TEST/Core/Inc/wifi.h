#ifndef WIFI_WIFI_H
#define WIFI_WIFI_H

#include "stm32f1xx_hal.h"
#include "usart.h"
#include "main.h"
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "stdio.h"

extern UART_HandleTypeDef huart3;

void WIFI_printf(char*fmt,...);//WIFIÄ£¿é·¢ËÍ
void WIFI_TCP_SEND(char*fmt,...);//



#endif 

