
//��Ƭ��ͷ�ļ�
#include "main.h"

//�����豸����
#include "esp8266.h"

//Ӳ������
#include "usart.h"

//C��
#include <string.h>
#include <stdio.h>
#include <string.h>
 

#define ESP01S_WIFI_INFO		"AT+CWJAP=\"TT\",\"a123456a\"\r\n" //����wifi���ƺ���������
const char* ClintID="test1";	//�豸����	
const char* username="TsUWn6659l"; //��ƷId
const char* passwd="version=2018-10-31&res=products%2FTsUWn6659l%2Fdevices%2Ftest1&et=2052911776&method=md5&sign=n8xu7BvoA2%2F%2BX90QJLYA2Q%3D%3D";//ͨ��token.exe���ɵ�����
const char* Url="mqtts.heclouds.com";//���ӵ���ַ
const char* pubtopic="$sys/TsUWn6659l/test1/thing/property/post";//����ָ���Ҫ�޸��豸���ƺͲ�ƷIDΪ��Ӧ������
const char* subtopic="$sys/TsUWn6659l/test1/thing/property/post/reply";//����ָ���Ҫ�޸�ͬ��������
const char* func1="power"; //����1����
unsigned char ESP01S_buf[256];
unsigned short ESP01S_cnt = 0, ESP01S_cntPre = 0;






uint8_t aRxBuffer;			//�����жϻ���
//==========================================================
//	�������ƣ�	ESP01S_Clear
//
//	�������ܣ�	��ջ���
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void ESP01S_Clear(void)
{

	memset(ESP01S_buf, 0, sizeof(ESP01S_buf));
	ESP01S_cnt = 0;

}

//==========================================================
//	�������ƣ�	ESP01S_WaitRecive
//
//	�������ܣ�	�ȴ��������
//
//	��ڲ�����	��
//
//	���ز�����	REV_OK-�������		REV_WAIT-���ճ�ʱδ���
//
//	˵����		ѭ�����ü���Ƿ�������
//==========================================================
_Bool ESP01S_WaitRecive(void)
{

	if(ESP01S_cnt == 0) 							//������ռ���Ϊ0 ��˵��û�д��ڽ��������У�����ֱ����������������
		return REV_WAIT;
		
	if(ESP01S_cnt == ESP01S_cntPre)				//�����һ�ε�ֵ�������ͬ����˵���������
	{
		ESP01S_cnt = 0;							//��0���ռ���
			
		return REV_OK;								//���ؽ�����ɱ�־
	}
		
	ESP01S_cntPre = ESP01S_cnt;					//��Ϊ��ͬ
	
	return REV_WAIT;								//���ؽ���δ��ɱ�־

}

//==========================================================
//	�������ƣ�	ESP01S_SendCmd
//
//	�������ܣ�	��������
//
//	��ڲ�����	cmd������
//				res����Ҫ���ķ���ָ��
//
//	���ز�����	0-�ɹ�	1-ʧ��
//
//	˵����		
//==========================================================
_Bool ESP01S_SendCmd(char *cmd, char *res)
{
	
	unsigned char timeOut = 200;

	Usart_SendString(huart2, (unsigned char *)cmd, strlen((const char *)cmd));
	
	while(timeOut--)
	{
		if(ESP01S_WaitRecive() == REV_OK)							//����յ�����
		{
			if(strstr((const char *)ESP01S_buf, res) != NULL)		//����������ؼ���
			{
				ESP01S_Clear();									//��ջ���
				
				return 0;
			}
		}
		
		HAL_Delay(10);
	}
	
	return 1;

}



//==========================================================
//	�������ƣ�	ESP01S_Init
//
//	�������ܣ�	��ʼ��ESP01S
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
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
	while(ESP01S_SendCmd("AT+CWMODE=1\r\n", "OK"))     //ģʽ1(Station),Ĭ�ϱ���Flash
		HAL_Delay(500);
	
	UsartPrintf(USART_DEBUG, "3. AT+CWDHCP\r\n");       //����DHCP(��ȡ����IP),Ĭ�ϱ���Flash
	while(ESP01S_SendCmd("AT+CWDHCP=1,1\r\n", "OK"))  
		HAL_Delay(500);
	
	UsartPrintf(USART_DEBUG, "4. CWJAP\r\n");           //����WIFI
	while(ESP01S_SendCmd(ESP01S_WIFI_INFO, "OK"))
		HAL_Delay(500);

	
	UsartPrintf(USART_DEBUG, "5.MQTTUSERCFG\r\n");           //�û���Ϣ����
	sprintf(cmdBuf,"AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"\r\n", ClintID,username,passwd);
	while(ESP01S_SendCmd(cmdBuf, "OK"))
	HAL_Delay(500);
	memset(cmdBuf,0,sizeof(cmdBuf));
	
	UsartPrintf(USART_DEBUG, "6.MQTTCONN\r\n");           //���ӷ�����
	sprintf(cmdBuf,"AT+MQTTCONN=0,\"%s\",1883,1\r\n",Url);
	while(ESP01S_SendCmd(cmdBuf, "OK"))
	HAL_Delay(500);
	memset(cmdBuf,0,sizeof(cmdBuf));
	
	
	UsartPrintf(USART_DEBUG, "7.MQTTSUB\r\n");           //������Ϣ
	sprintf(cmdBuf,"AT+MQTTSUB=0,\"%s\",1\r\n",subtopic);
	while(ESP01S_SendCmd(cmdBuf, "OK"))
	HAL_Delay(500);
	memset(cmdBuf,0,sizeof(cmdBuf));
}


//���ܣ�esp������Ϣ
//��������
//����ֵ��0�����ͳɹ���1������ʧ��
void Esp_PUB(int power)
{
	
	char cmdBuf[256];
	UsartPrintf(USART_DEBUG, "Publish\r\n");           //������Ϣ
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
 
	if(ESP01S_cnt >= sizeof(ESP01S_buf))  //����ж�
	{
		ESP01S_cnt = 0;
		memset(ESP01S_buf,0x00,sizeof(ESP01S_buf));
		HAL_UART_Transmit(&huart1, (uint8_t *)"���ջ������", 10,0xFFFF); 	      
	}
	else
	{
		ESP01S_buf[ESP01S_cnt++] = aRxBuffer;   //��������ת��	
//		  if(aRxBuffer=='1')  HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
//        if(aRxBuffer=='0')  HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
 	}
	
	HAL_UART_Receive_IT(&huart2, (uint8_t *)&aRxBuffer, 1);   //�ٿ��������ж�
}


/* USER CODE END 4 */

