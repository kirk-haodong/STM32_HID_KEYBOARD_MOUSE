/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * 此程序功能为摇杆控制鼠标，三个按键从第一到第三分别是B、TAB、ALT按键
	* 用户若需要定制按键功能请跳转app.c
  *
  ******************************************************************************
  */
	
#include "main.h"
#include "usb_device.h"
#include "app.h"

#define	RangJudge(val,min,max)   (val>min? (val<max? 1 : 0) : 0)
#define LOOSE 						0
#define	PRESS 						1
#define SEND_KEYBOARD 		2
#define INFRARED_CONTROL  3

typedef struct T9_Infrared
{
  uint32_t  KEY_VALUE;   
	uint16_t  TriTime[2];  
	uint8_t   DataBit;
	uint8_t   FrameStart;
  uint8_t   KEY_Count;	
	uint8_t   TriPolarity;
}T9_InfraredDev;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim9;

T9_InfraredDev  T9Infra_S, *T9Infra = &T9Infra_S;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM9_Init(void);
uint8_t  GetInfraredKey(void);

static int STATE = LOOSE;
static uint8_t CountNum;
uint8_t hid_Buffer [9]={0x01,0,0,0,0,0,0,0,0};
static uint8_t countNum = 0;

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_USB_DEVICE_Init();
  MX_TIM3_Init();
  MX_TIM9_Init();
  HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_IC_Start_IT(&htim9, TIM_CHANNEL_1);
	
	T9Infra->TriPolarity = 1;
  T9Infra->FrameStart = 1;
	T9Infra->TriTime[0] = 0;
	T9Infra->TriTime[1] = 0;
	
  while (1)
  {
		switch(STATE)
		{
			case LOOSE:
					break;
			case PRESS:
					Control();
					STATE = LOOSE;
					break;
			case SEND_KEYBOARD:
					USBD_HID_SendReport (&hUsbDeviceFS, hid_Buffer,9);
					STATE = LOOSE;
					break;
			case INFRARED_CONTROL:
				  InfraredControl ();
					STATE = LOOSE;
					break;
			default:
					STATE = LOOSE;
					break;
		}
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}


static void MX_TIM2_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 50;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}


static void MX_TIM3_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 1600-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 250-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}


static void MX_TIM9_Init(void)
{
  TIM_IC_InitTypeDef sConfigIC = {0};
  htim9.Instance = TIM9;
  htim9.Init.Prescaler = 32-1;
  htim9.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim9.Init.Period = 10000-1;
  htim9.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim9.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(&htim9) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim9, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
}


static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC0 PC1 PC2 PC3 
                           PC10 PC11 PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB5 PB6 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//遥控捕获
{
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		T9Infra->TriPolarity = !T9Infra->TriPolarity;
		if(!T9Infra->TriPolarity)  //上升沿触发
		{		
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim9, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
			T9Infra->TriTime[T9Infra->TriPolarity] =  HAL_TIM_ReadCapturedValue(&htim9, TIM_CHANNEL_1);			
		}
		else  //下降沿触发
		{
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim9, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
       if(T9Infra->FrameStart == 1) 
			{
        T9Infra->TriTime[T9Infra->TriPolarity] = HAL_TIM_ReadCapturedValue(&htim9, TIM_CHANNEL_1);				
			}					            
		}	
		TIM9->CNT = 0; 
		
  		if(RangJudge(T9Infra->TriTime[0],4000,5000) &&    RangJudge(T9Infra->TriTime[1],2000,3000) )   //同步码
		{
			 T9Infra->KEY_VALUE = 0;  
			 T9Infra->KEY_Count = 0;  
       T9Infra->DataBit = 0;  	
		}		
			else if(RangJudge(T9Infra->TriTime[0],4000,5000) && RangJudge(T9Infra->TriTime[1],7000,8000) ) 			//连发码
		{
			 T9Infra->KEY_Count = 1;
		}
	  /********************************* 接收数据 *********************************/		
		if( T9Infra->TriPolarity == 1)
		{
      if(RangJudge(T9Infra->TriTime[0],200,300) && RangJudge(T9Infra->TriTime[1],200,300) )              //0
			{			
			   T9Infra->KEY_VALUE &= ~(1<T9Infra->DataBit++); 
					countNum++;
			}
      else if(RangJudge(T9Infra->TriTime[0],200,300) && RangJudge(T9Infra->TriTime[1],800,900) ) 				//1
			{
			   T9Infra->KEY_VALUE |= 1 << T9Infra->DataBit++;
				countNum++;
			}
		}	
		if(countNum == 32||(T9Infra->KEY_Count == 1))
		{
			STATE = INFRARED_CONTROL;			//进入红外遥控
			T9Infra->KEY_Count = 0;
			countNum = 0;
		}	
	}
}	

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//按键防抖、按键检测、键盘空键值发送
{ 
	if(htim->Instance == TIM2)//按键防抖
		{
			if(!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_12 )  ||  (!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_11))  ||  \
				(!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_10))  ||  (!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_0) )  ||  \
				(!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1) )  ||  (!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_2) )  ||  \
			  (!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_3)))
				{
					if(CountNum >2)
					{
						CountNum = 0;
						STATE = PRESS;
					}
					else CountNum++;
				}	
		}
		if(htim->Instance == TIM3) //键盘空按键发送
		{
			STATE = SEND_KEYBOARD;
		}
}

uint8_t  GetInfraredKey(void)
{
   //通过与反码异或，验证按键码的正确性
   if( (uint8_t)( (T9Infra->KEY_VALUE >> 24) ^ (T9Infra->KEY_VALUE >> 16) ) == 0xff  )
   return (uint8_t)(T9Infra->KEY_VALUE>>16);
	 
   else  return 0;
}
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
