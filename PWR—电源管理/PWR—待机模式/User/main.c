/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   PVD���
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */

#include "main.h"
#include "stm32f4xx.h"
#include "./led/bsp_led.h" 
#include "./usart/bsp_debug_usart.h"
#include "./key/bsp_key.h" 

static uint8_t KEY2_LongPress(void);

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void) 
{
	/* ��ʼ��ϵͳʱ��Ϊ168MHz */
	SystemClock_Config();
	/* ��ʼ��LED */
	LED_GPIO_Config();	
	/* ��ʼ�����Դ��ڣ�һ��Ϊ����1 */
	DEBUG_USART_Config();	
	/*��ʼ������������Ҫ�ж�,����ʼ��KEY2���ɣ�ֻ���ڻ��ѵ�PA0���Ų���Ҫ������ʼ��*/
	Key_GPIO_Config(); 
	
	printf("\r\n ��ӭʹ��Ұ��  STM32 F407 �����塣\r\n");
	printf("\r\n Ұ��F407 ����ģʽ����\r\n");
	
	printf("\r\n ʵ��˵����\r\n");

	printf("\r\n 1.�������У�LED1�Ʊ�ʾ���θ�λ���ϵ�����Ÿ�λ��LED3��ʾ�����������״̬��LED2��ʾ�����Ǵ������ѵĸ�λ\r\n");
	printf("\r\n 2.����KEY3�����󣬻�������ģʽ\r\n");
	printf("\r\n 3.�ڴ���ģʽ�£���KEY1�����ɻ��ѣ����Ѻ�ϵͳ����и�λ�������ͷ��ʼִ��\r\n");
	printf("\r\n 4.��ͨ�����WU��־λȷ����λ��Դ\r\n");
	
	printf("\r\n 5.�ڴ���״̬�£�DAP�������޷���STM32���س�����Ҫ���Ѻ��������");

	//��⸴λ��Դ
	if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB) == SET)
	{
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
		LED2_ON;
		printf("\r\n �������Ѹ�λ \r\n");
	}
	else
	{
		LED1_ON;
		printf("\r\n �Ǵ������Ѹ�λ \r\n");
	}
	
  while(1)
  {			
		// K2 ���������������ģʽ
		if(KEY2_LongPress())
		{
			
			printf("\r\n �����������ģʽ���������ģʽ��ɰ�KEY1���ѣ����Ѻ����и�λ�������ͷ��ʼִ��\r\n");
			LED3_ON;	
			HAL_Delay(1000);
			
			/*���WU״̬λ*/
			__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
			
			/* ʹ��WKUP���ŵĻ��ѹ��� ��ʹ��PA0*/
			HAL_PWR_EnableWakeUpPin( 0x00000100U);
			
			//��ͣ�δ�ʱ�ӣ���ֹͨ���δ�ʱ���жϻ���
			HAL_SuspendTick();			
			/* �������ģʽ */
			HAL_PWR_EnterSTANDBYMode();
		}
  }
}
/**
  * @brief  ���ڼ�ⰴ���Ƿ񱻳�ʱ�䰴��
  * @param  ��
  * @retval 1 ����������ʱ�䰴��  0 ������û�б���ʱ�䰴��
  */
static uint8_t KEY2_LongPress(void)
{			
	uint8_t downCnt =0;	//��¼���µĴ���
	uint8_t upCnt =0;	//��¼�ɿ��Ĵ���			

	while(1)																										//��ѭ������return����
	{	
		HAL_Delay(20);	//�ӳ�һ��ʱ���ټ��

		if(HAL_GPIO_ReadPin( KEY2_GPIO_PORT, KEY2_PIN ) == SET)	//��⵽���°���
		{
			downCnt++;	//��¼���´���
			upCnt=0;	//��������ͷż�¼

			if(downCnt>=100)	//����ʱ���㹻
			{
				return 1; 		//��⵽������ʱ�䳤����
			}
		}
		else 
		{
			upCnt++; 			//��¼�ͷŴ���
			if(upCnt>5)			//������⵽�ͷų���5��
			{
				return 0;		//����ʱ��̫�̣����ǰ�����������
			}
		}//	if(HAL_GPIO_ReadPin 
	}//while
}



/**
  * @brief  PWR PVD interrupt callback
  * @param  None 
  * @retval None
  */
void HAL_PWR_PVDCallback(void)
{
  /* ����ƣ�ʵ��Ӧ����Ӧ�������״̬���� */
  LED_ALLON;
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
  *            HSE Frequency(Hz)              = 25000000
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
