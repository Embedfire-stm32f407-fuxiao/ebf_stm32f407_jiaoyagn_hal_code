/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   WWDG�����ڿ��Ź�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx.h"
#include "./key/bsp_key.h" 
#include "./wwdg/bsp_wwdg.h"   
#include "./led/bsp_led.h"   

static void SystemClock_Config(void);

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	uint8_t wwdg_tr, wwdg_wr;
    /* ϵͳʱ�ӳ�ʼ����168 MHz */
    SystemClock_Config();
	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();	   

	//��鴰�ڿ��Ź���λ��־λ
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST) != RESET)
	{
		// ���Ź���λ��������ɫ����
		LED2_ON;

		//�����λ��־λ
		__HAL_RCC_CLEAR_RESET_FLAGS();
	}
	else
	{
		// ������������ɫ����
		LED1_ON;
	}
	HAL_Delay(500);
	LED_ALLON;
	HAL_Delay(500);

	// WWDG����	
	// ��ʼ��WWDG�����ü�������ʼֵ�������ϴ���ֵ������WWDG��ʹ����ǰ�����ж�
	WWDG_Config(127,80,WWDG_PRESCALER_8);	
	
	// ����ֵ�����ڳ�ʼ����ʱ�����ó�0X5F�����ֵ����ı�
	wwdg_wr = WWDG->CFR & 0X7F;

	while(1)
	{	

		//-----------------------------------------------------
		// �ⲿ��Ӧ��д��Ҫ��WWDG��صĳ�����γ������е�ʱ��
		// �����˴���ֵӦ�����óɶ��
		//-----------------------------------------------------
		// ��ʱ��ֵ����ʼ�������0X7F��������WWDGʱ�����ֵ�᲻�ϼ�С
		// ����������ֵ���ڴ���ֵʱι���Ļ����Ḵλ�������������ٵ�0X40
		// ��û��ι���Ļ��ͷǳ��ǳ�Σ���ˣ��������ټ�һ�ε���0X3Fʱ�͸�λ
		// ����Ҫ����������ֵ�ڴ���ֵ��0X40֮���ʱ��ι��������0X40�ǹ̶��ġ�
		wwdg_tr = WWDG->CR & 0X7F;
		if( wwdg_tr == wwdg_wr)
		{
			// ι�����������ü�������ֵΪ���0X7F
			WWDG_Feed();
			// ����ι������ɫ����˸
			LED3_TOGGLE;
		}
	}
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 25
  *            PLL_N                          = 336
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
 void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    while(1) {};
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    while(1) {};
  }

  /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
  if (HAL_GetREVID() == 0x1001)
  {
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
