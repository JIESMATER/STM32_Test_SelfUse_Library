#include "wifi.h"

//wifi模块通信，调用UART3，专用的prinf函数
void WIFI_printf(char*fmt,...)
{
	char buff[USART3_REC_LEN+1];
	uint16_t i=0;
	va_list arg_ptr;
	va_start(arg_ptr,fmt);
	vsnprintf(buff,USART3_REC_LEN+1,fmt,arg_ptr);
	i = strlen(buff);
	if(strlen(buff)>USART3_REC_LEN)i=USART3_REC_LEN;
	HAL_UART_Transmit(&huart3,(uint8_t *)buff,i,0xffff);
	va_end(arg_ptr);
}
//wifi模块在TCP模式下的数据发送，
void WIFI_TCP_SEND(char*fmt,...)
{
	char buff[USART3_REC_LEN+1];
	uint16_t i = 0;
	va_list arg_ptr;
	va_start(arg_ptr,fmt);
	vsnprintf(buff,USART3_REC_LEN+1,fmt,arg_ptr);
	i = strlen(buff);
	if(strlen(buff)>USART3_REC_LEN)i=USART3_REC_LEN;
	WIFI_printf("AT+CIPSEND=%d\r\n",i);
	HAL_Delay(100);
	HAL_UART_Transmit(&huart3,(uint8_t *)buff,i,0xffff);
	va_end(arg_ptr);
}
//wifi模块在UDP模式下的数据发送
void WIFI_UDP_SEND(char*fmt,...)
{
	char buff[USART3_REC_LEN+1];
	uint16_t i = 0;
	va_list arg_ptr;
	va_start(arg_ptr,fmt);
	vsnprintf(buff,USART3_REC_LEN+1,fmt,arg_ptr);
	i = strlen(buff);
	if(strlen(buff)>USART3_REC_LEN)i=USART3_REC_LEN;
	WIFI_printf("AT+CIPSEND=%d\r\n",i);
	HAL_Delay(100);
	HAL_UART_Transmit(&huart3,(uint8_t *)buff,i,0xffff);
	va_end(arg_ptr);
}

