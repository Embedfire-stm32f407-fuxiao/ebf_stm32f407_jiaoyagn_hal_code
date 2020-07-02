/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2017-xx-xx
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
#include "./key/bsp_exti.h" 
static void SYSCLKConfig_STOP(void);
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void) 
{
	uint32_t SYSCLK_Frequency=0; 
	uint32_t HCLK_Frequency=0;
	uint32_t PCLK1_Frequency=0;
	uint32_t PCLK2_Frequency=0;
	uint32_t SYSCLK_Source=0;
	
	/* ��ʼ��ϵͳʱ��Ϊ168MHz */
	SystemClock_Config();
	/* ��ʼ��LED */
	LED_GPIO_Config();	
	/* ��ʼ�����Դ��ڣ�һ��Ϊ����1 */
	DEBUG_USART_Config();	
	/* ��ʼ������Ϊ�ж�ģʽ�������жϺ������жϷ�����	*/
	EXTI_Key_Config(); 
	
	printf("\r\n ��ӭʹ��Ұ��  STM32 F407 �����塣\r\n");
    printf("\r\n Ұ��F407 ֹͣģʽ����\r\n");
	
	printf("\r\n ʵ��˵����\r\n");

	printf("\r\n 1.�������У�LED1��ʾSTM32�������У�LED2��ʾ˯��״̬��LEDALL��ʾ�մ�ֹͣ״̬������\r\n");
	printf("\r\n 2.��������һ��ʱ����Զ�����ֹͣ״̬����ֹͣ״̬�£���ʹ��KEY1��KEY2����\r\n");
	printf("\r\n 3.��ʵ��ִ������һ��ѭ����\r\n ------��LED1(��������)->LED2(ֹͣģʽ)->��KEY1��KEY2����->LEDALL(�ձ�����)-----��\r\n");
	printf("\r\n 4.��ֹͣ״̬�£�DAP�������޷���STM32���س���\r\n �ɰ�KEY1��KEY2���Ѻ����أ�\r\n �򰴸�λ��ʹоƬ���ڸ�λ״̬��Ȼ���ڵ����ϵ�����ذ�ť�����ͷŸ�λ��������������\r\n");

	while(1)
	{	
		/*********ִ������***************************/
		printf("\r\n STM32��������\r\n");

		LED1_ON;	
		HAL_Delay(2000);		
		/*****����ִ����ϣ�����˯�߽��͹���***********/

		printf("\r\n ����ֹͣģʽ����KEY1��KEY2�����ɻ���\r\n");

		//ʹ�ú��ָʾ������˯��״̬
		LED2_ON;
		//��ͣ�δ�ʱ�ӣ���ֹͨ���δ�ʱ���жϻ���
		HAL_SuspendTick();
		/*����ֹͣģʽʱ��FLASH�������״̬*/
		HAL_PWREx_EnableFlashPowerDown();
		/* ����ֹͣģʽ�����õ�ѹ������Ϊ�͹���ģʽ���ȴ��жϻ��� */
		HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON,PWR_STOPENTRY_WFI);
		//�ȴ��жϻ���  K1��K2�����ж�	
		/***�����ѣ�������ָʾ***/
		LED_ALLON;
		//����ʱ�ӼĴ�����ֵ����SystemCoreClock����
		SystemCoreClockUpdate();
		//��ȡ���Ѻ��ʱ��״̬	
		SYSCLK_Frequency = HAL_RCC_GetSysClockFreq(); 
		HCLK_Frequency   = HAL_RCC_GetHCLKFreq();
		PCLK1_Frequency  = HAL_RCC_GetPCLK1Freq();
		PCLK2_Frequency  = HAL_RCC_GetPCLK2Freq();
		SYSCLK_Source    = __HAL_RCC_GET_SYSCLK_SOURCE();
		//�������ڴ���ֱ��ʹ��HSIʱ�ӣ������Ӱ�촮�ڲ�����
		printf("\r\n�ջ��ѵ�ʱ��״̬��\r\n");
		printf(" SYSCLKƵ��:%d,\r\n HCLKƵ��:%d,\r\n PCLK1Ƶ��:%d,\r\n PCLK2Ƶ��:%d,\r\n ʱ��Դ:%d (0��ʾHSI��8��ʾPLLCLK)\n", 
		SYSCLK_Frequency,HCLK_Frequency,PCLK1_Frequency,PCLK2_Frequency,SYSCLK_Source);		  
		/* ��ֹͣģʽ���Ѻ�����ϵͳʱ��:����HSE��PLL*/
		/* ѡ��PLL��Ϊϵͳʱ��Դ(HSE��PLL��ֹͣģʽ�±�����)*/
		SYSCLKConfig_STOP();
		
		//�����Ѻ󣬻ָ��δ�ʱ��			
		HAL_ResumeTick();
		//��ȡ�������ú��ʱ��״̬
		SYSCLK_Frequency = HAL_RCC_GetSysClockFreq(); 
		HCLK_Frequency   = HAL_RCC_GetHCLKFreq();
		PCLK1_Frequency  = HAL_RCC_GetPCLK1Freq();
		PCLK2_Frequency  = HAL_RCC_GetPCLK2Freq();
		SYSCLK_Source    = __HAL_RCC_GET_SYSCLK_SOURCE();
		
		//��������ʱ��Դ��ʼ��״̬
		printf("\r\n�������ú��ʱ��״̬��\r\n");
		printf(" SYSCLKƵ��:%d,\r\n HCLKƵ��:%d,\r\n PCLK1Ƶ��:%d,\r\n PCLK2Ƶ��:%d,\r\n ʱ��Դ:%d (0��ʾHSI��8��ʾPLLCLK)\n", 
		SYSCLK_Frequency,HCLK_Frequency,PCLK1_Frequency,PCLK2_Frequency,SYSCLK_Source);

		HAL_Delay(2000);	
			
		printf("\r\n ���˳�ֹͣģʽ\r\n");
		//����ִ��whileѭ��
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

/**
  * @brief  ��ֹͣģʽ���Ѻ�����ϵͳʱ��:����HSE��PLL��ѡ��PLL��Ϊϵͳʱ��Դ��

  * @param  ��
  * @retval ��
  */
static void SYSCLKConfig_STOP(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	uint32_t pFLatency = 0;

	/* ���õ�Դ����ʱ�� */
	__HAL_RCC_PWR_CLK_ENABLE();

	/* �����ڲ�RCC�Ĵ�����ȡ�������� */
	HAL_RCC_GetOscConfig(&RCC_OscInitStruct);

	/* ��ֹͣģʽ���Ѻ���������ϵͳʱ��: ����HSE��PLL */
	RCC_OscInitStruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState        = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_ON;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
	while(1) { ; }
	}

	/* �����ڲ�RCC�Ĵ�����ȡʱ������ */
	HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &pFLatency);

	/* ѡ�� PLL ��Ϊϵͳʱ��Դ, ������ HCLK��PCLK1 �� PCLK2ʱ�ӷ�Ƶϵ�� */
	RCC_ClkInitStruct.ClockType     = RCC_CLOCKTYPE_SYSCLK;
	RCC_ClkInitStruct.SYSCLKSource  = RCC_SYSCLKSOURCE_PLLCLK;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, pFLatency) != HAL_OK)
	{
	while(1) { ; }
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
