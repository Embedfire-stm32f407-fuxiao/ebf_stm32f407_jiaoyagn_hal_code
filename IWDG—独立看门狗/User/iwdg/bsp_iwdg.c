/**
  ******************************************************************************
  * @file    bsp_iwdg.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   独立看门狗驱动
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */


#include "./iwdg/bsp_iwdg.h"  

IWDG_HandleTypeDef IWDG_Handle;
/*
 * 设置 IWDG 的超时时间
 * Tout = prv/40 * rlv (s)
 *      prv可以是[4,8,16,32,64,128,256]
 * prv:预分频器值，取值如下：
 *     @arg IWDG_PRESCALER_4: IWDG prescaler set to 4
 *     @arg IWDG_PRESCALER_8: IWDG prescaler set to 8
 *     @arg IWDG_PRESCALER_16: IWDG prescaler set to 16
 *     @arg IWDG_PRESCALER_32: IWDG prescaler set to 32
 *     @arg IWDG_PRESCALER_64: IWDG prescaler set to 64
 *     @arg IWDG_PRESCALER_128: IWDG prescaler set to 128
 *     @arg IWDG_PRESCALER_256: IWDG prescaler set to 256
 *
 *		独立看门狗使用LSI作为时钟。
 *		LSI 的频率一般在 30~60KHZ 之间，根据温度和工作场合会有一定的漂移，我
 *		们一般取 40KHZ，所以独立看门狗的定时时间并一定非常精确，只适用于对时间精度
 *		要求比较低的场合。
 *
 * rlv:预分频器值，取值范围为：0-0XFFF
 * 函数调用举例：
 * IWDG_Config(IWDG_PRESCALER_64 ,625);  // IWDG 1s 超时溢出 
 *						（64/40）*625 = 1s
 */

void IWDG_Config(uint8_t prv ,uint16_t rlv)
{	
	IWDG_Handle.Instance = IWDG;
	// 设置预分频器值
	IWDG_Handle.Init.Prescaler = prv;
	// 设置重装载寄存器值
	IWDG_Handle.Init.Reload = rlv;
//	// 设置要与向下计数器进行比较的窗口值
//	IWDG_Handle.Init.Window = IWDG_WINDOW_DISABLE;
	// 初始化IWDG
	HAL_IWDG_Init(&IWDG_Handle);	
	// 启动 IWDG
	__HAL_IWDG_START(&IWDG_Handle);
}

// 喂狗
void IWDG_Feed(void)
{
	// 把重装载寄存器的值放到计数器中，喂狗，防止IWDG复位
	// 当计数器的值减到0的时候会产生系统复位
	HAL_IWDG_Refresh(&IWDG_Handle);
}
/*********************************************END OF FILE**********************/
