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
#define ServerIPaddress		"192.168.10.104"
#define ServerIPPort		"5868"
#define ClientIPPort		"9098"


void WIFI_printf(char*fmt,...);//WIFIÄ£¿é·¢ËÍ
void WIFI_TCP_SEND(char*fmt,...);//
void WIFI_UDP_SEND(char*fmt,...);


#endif 

