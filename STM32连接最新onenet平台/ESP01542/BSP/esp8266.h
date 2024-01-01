#ifndef _ESP01S_H_
#define _ESP01S_H_
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define REV_OK		0	//接收完成标志
#define REV_WAIT	1	//接收未完成标志

void ESP01S_Init(void);
void ESP01S_Clear(void);
//void ESP01S_SendData(unsigned char *data, unsigned short len);
//unsigned char *ESP01S_GetIPD(unsigned short timeOut);
void Esp_PUB(int power);
#endif

