/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "rtc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */	
	HAL_UART_Receive_IT(&huart1,(uint8_t *)&USART1_NewData,1);
	RTC_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  		if(USART1_RX_STA&0xC000){ //如果标志位是0x8000表示收到数据串完成，可以处理。
		   if((USART1_RX_STA&0x3FFF)==0){ //单独的回车键再显示一次欢迎词
			   RTC_Get();//读出当前RTC日期与时间，放入全局变量
			   printf(" 洋桃IoT开发板RTC实时时钟测试   \r\n");
			   printf(" 实时时间：%04d-%02d-%02d  %02d:%02d:%02d  \r\n",
					   ryear, rmon, rday, rhour, rmin, rsec);//显示日期时间
			   printf(" 单按回车键更新时间，输入字母C初始化时钟 \r\n");
			   printf(" 请输入设置时间，格式20170806120000，按回车键确定！ \r\n");
		   }
		   else if((USART1_RX_STA&0x3FFF)==1){  //判断数据是不是1个
			   if(USART1_RX_BUF[0]=='c' ||  USART1_RX_BUF[0]=='C'){
				   MX_RTC_Init(); //键盘输入c或C，初始化时钟（调用HAL库自带的初始化函数）
				   printf("初始化成功！       \r\n");//显示初始化成功
			   }
			   else printf("指令错误！           \r\n"); //显示指令错误！
		   }
		   else if((USART1_RX_STA&0x3FFF)==14){ //判断数据是不是14个
			   //将超级终端发过来的数据换算并写入RTC
			   ryear = (USART1_RX_BUF[0]-0x30)*1000 + (USART1_RX_BUF[1]-0x30)*100 +
					   (USART1_RX_BUF[2]-0x30)*10 + (USART1_RX_BUF[3]-0x30);//减0x30得到十进制0~9的数据
			   rmon =  (USART1_RX_BUF[4]-0x30)*10 + (USART1_RX_BUF[5]-0x30);
			   rday =  (USART1_RX_BUF[6]-0x30)*10 + (USART1_RX_BUF[7]-0x30);
			   rhour = (USART1_RX_BUF[8]-0x30)*10 + (USART1_RX_BUF[9]-0x30);
			   rmin =  (USART1_RX_BUF[10]-0x30)*10 + (USART1_RX_BUF[11]-0x30);
			   rsec =  (USART1_RX_BUF[12]-0x30)*10 + (USART1_RX_BUF[13]-0x30);
			   if (RTC_Set(ryear,rmon,rday,rhour,rmin,rsec) != HAL_OK)//将数据写入RTC程序
			   {
				   printf("写入时间失败！        \r\n"); //显示写入失败
			   }else printf("写入成功！       \r\n");//显示写入成功
		   }
		   else{ //如果以上都不是，即是错误的指令。
			   printf("指令错误！          \r\n");  //如果不是以上正确的操作，显示指令错误！
		   }
		   USART1_RX_STA=0; //将串口数据标志位清0
		}

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
