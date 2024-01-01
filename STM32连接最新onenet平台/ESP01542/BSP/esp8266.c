
//单片机头文件
#include "main.h"

//网络设备驱动
#include "esp8266.h"

//硬件驱动
#include "usart.h"

//C库
#include <string.h>
#include <stdio.h>
#include <string.h>
 

#define ESP01S_WIFI_INFO		"AT+CWJAP=\"TT\",\"a123456a\"\r\n" //连接wifi名称和密码设置
const char* ClintID="test1";	//设备名称	
const char* username="TsUWn6659l"; //产品Id
const char* passwd="version=2018-10-31&res=products%2FTsUWn6659l%2Fdevices%2Ftest1&et=2052911776&method=md5&sign=n8xu7BvoA2%2F%2BX90QJLYA2Q%3D%3D";//通过token.exe生成的密码
const char* Url="mqtts.heclouds.com";//连接的网址
const char* pubtopic="$sys/TsUWn6659l/test1/thing/property/post";//发布指令，需要修改设备名称和产品ID为对应的内容
const char* subtopic="$sys/TsUWn6659l/test1/thing/property/post/reply";//订阅指令，需要修改同样的内容
const char* func1="power"; //参数1名称
unsigned char ESP01S_buf[256];
unsigned short ESP01S_cnt = 0, ESP01S_cntPre = 0;






uint8_t aRxBuffer;			//接收中断缓冲
//==========================================================
//	函数名称：	ESP01S_Clear
//
//	函数功能：	清空缓存
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void ESP01S_Clear(void)
{

	memset(ESP01S_buf, 0, sizeof(ESP01S_buf));
	ESP01S_cnt = 0;

}

//==========================================================
//	函数名称：	ESP01S_WaitRecive
//
//	函数功能：	等待接收完成
//
//	入口参数：	无
//
//	返回参数：	REV_OK-接收完成		REV_WAIT-接收超时未完成
//
//	说明：		循环调用检测是否接收完成
//==========================================================
_Bool ESP01S_WaitRecive(void)
{

	if(ESP01S_cnt == 0) 							//如果接收计数为0 则说明没有处于接收数据中，所以直接跳出，结束函数
		return REV_WAIT;
		
	if(ESP01S_cnt == ESP01S_cntPre)				//如果上一次的值和这次相同，则说明接收完毕
	{
		ESP01S_cnt = 0;							//清0接收计数
			
		return REV_OK;								//返回接收完成标志
	}
		
	ESP01S_cntPre = ESP01S_cnt;					//置为相同
	
	return REV_WAIT;								//返回接收未完成标志

}

//==========================================================
//	函数名称：	ESP01S_SendCmd
//
//	函数功能：	发送命令
//
//	入口参数：	cmd：命令
//				res：需要检查的返回指令
//
//	返回参数：	0-成功	1-失败
//
//	说明：		
//==========================================================
_Bool ESP01S_SendCmd(char *cmd, char *res)
{
	
	unsigned char timeOut = 200;

	Usart_SendString(huart2, (unsigned char *)cmd, strlen((const char *)cmd));
	
	while(timeOut--)
	{
		if(ESP01S_WaitRecive() == REV_OK)							//如果收到数据
		{
			if(strstr((const char *)ESP01S_buf, res) != NULL)		//如果检索到关键词
			{
				ESP01S_Clear();									//清空缓存
				
				return 0;
			}
		}
		
		HAL_Delay(10);
	}
	
	return 1;

}



//==========================================================
//	函数名称：	ESP01S_Init
//
//	函数功能：	初始化ESP01S
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void ESP01S_Init(void)
{
	char cmdBuf[512];
	
	ESP01S_Clear();
	
	UsartPrintf(USART_DEBUG, "0. AT\r\n");
	while(ESP01S_SendCmd("AT\r\n", "OK"))
		HAL_Delay(500);
	
	UsartPrintf(USART_DEBUG, "1. RST\r\n");
	ESP01S_SendCmd("AT+RST\r\n", "");
        HAL_Delay(500);	
	
	
	UsartPrintf(USART_DEBUG, "2. CWMODE\r\n");
	while(ESP01S_SendCmd("AT+CWMODE=1\r\n", "OK"))     //模式1(Station),默认保存Flash
		HAL_Delay(500);
	
	UsartPrintf(USART_DEBUG, "3. AT+CWDHCP\r\n");       //开启DHCP(获取分配IP),默认保存Flash
	while(ESP01S_SendCmd("AT+CWDHCP=1,1\r\n", "OK"))  
		HAL_Delay(500);
	
	UsartPrintf(USART_DEBUG, "4. CWJAP\r\n");           //链接WIFI
	while(ESP01S_SendCmd(ESP01S_WIFI_INFO, "OK"))
		HAL_Delay(500);

	
	UsartPrintf(USART_DEBUG, "5.MQTTUSERCFG\r\n");           //用户信息配置
	sprintf(cmdBuf,"AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"\r\n", ClintID,username,passwd);
	while(ESP01S_SendCmd(cmdBuf, "OK"))
	HAL_Delay(500);
	memset(cmdBuf,0,sizeof(cmdBuf));
	
	UsartPrintf(USART_DEBUG, "6.MQTTCONN\r\n");           //连接服务器
	sprintf(cmdBuf,"AT+MQTTCONN=0,\"%s\",1883,1\r\n",Url);
	while(ESP01S_SendCmd(cmdBuf, "OK"))
	HAL_Delay(500);
	memset(cmdBuf,0,sizeof(cmdBuf));
	
	
	UsartPrintf(USART_DEBUG, "7.MQTTSUB\r\n");           //订阅消息
	sprintf(cmdBuf,"AT+MQTTSUB=0,\"%s\",1\r\n",subtopic);
	while(ESP01S_SendCmd(cmdBuf, "OK"))
	HAL_Delay(500);
	memset(cmdBuf,0,sizeof(cmdBuf));
}


//功能：esp发送消息
//参数：无
//返回值：0：发送成功；1：发送失败
void Esp_PUB(int power)
{
	
	char cmdBuf[256];
	UsartPrintf(USART_DEBUG, "Publish\r\n");           //订阅消息
  sprintf(cmdBuf,"AT+MQTTPUB=0,\"%s\",\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"%s\\\":{\\\"value\\\":%d\\}}}\",0,0\r\n",pubtopic,func1,power);
	while(ESP01S_SendCmd(cmdBuf, "OK"))
	HAL_Delay(500);
	memset(cmdBuf,0,sizeof(cmdBuf));
	HAL_Delay(100);
	
}





/* USER CODE BEGIN 4 */
 
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  
    UNUSED(huart);
 
	if(ESP01S_cnt >= sizeof(ESP01S_buf))  //溢出判断
	{
		ESP01S_cnt = 0;
		memset(ESP01S_buf,0x00,sizeof(ESP01S_buf));
		HAL_UART_Transmit(&huart1, (uint8_t *)"接收缓存溢出", 10,0xFFFF); 	      
	}
	else
	{
		ESP01S_buf[ESP01S_cnt++] = aRxBuffer;   //接收数据转存	
//		  if(aRxBuffer=='1')  HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
//        if(aRxBuffer=='0')  HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
 	}
	
	HAL_UART_Receive_IT(&huart2, (uint8_t *)&aRxBuffer, 1);   //再开启接收中断
}


/* USER CODE END 4 */

