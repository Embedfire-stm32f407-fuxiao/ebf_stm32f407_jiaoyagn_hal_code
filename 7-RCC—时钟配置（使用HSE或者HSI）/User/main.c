/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   RCC--ʱ������(ʹ��HSE��HSI)
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ս�濪���� 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./rcc/bsp_clkconfig.h"

void Delay(__IO uint32_t nCount); 

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{

    // ��������main����֮ǰ�������ļ���statup_stm32f407xx.s�Ѿ�����
    // SystemInit()������ϵͳʱ�ӳ�ʼ����16MHZ
    // SystemInit()��system_stm32f4xx.c�ж���
    // ����û����޸�ϵͳʱ�ӣ������б�д�����޸�	
    // ��������ϵͳʱ�ӣ���ʱ�����ѡ��ʹ��HSE����HSI
  
    //ϵͳʱ�ӱ�׼����Ϊ168M�������216M�����Ƽ�
    HSE_SetSysClock(25, 336, 2, 7);
    // ʹ��HSI������ϵͳʱ��Ϊ168M���л�ʱ��ʱ�����PLL�Ѿ�ʹ����Ϊϵͳʱ���������Ҫ�ȸ�λRCC->CFGR�Ĵ��������л�ʱ��
	// RCC->CFGR = 0x00000000;
    // HSI_SetSysClock(25, 336, 2, 7);
        
    // LED �˿ڳ�ʼ�� 
    LED_GPIO_Config();
        
    // MCO1 ���PLLCLK	
    HAL_RCC_MCOConfig(RCC_MCO1,RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_2);
        
    // MCO2 ���SYSCLK
    HAL_RCC_MCOConfig(RCC_MCO2,RCC_MCO2SOURCE_SYSCLK, RCC_MCODIV_2);

    while (1)
    {
        LED_ALLON;			 // �� 
        Delay(0x5FFFFF);
        LED_ALLOFF;		 // ��
        Delay(0x5FFFFF);
    }
}

void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
