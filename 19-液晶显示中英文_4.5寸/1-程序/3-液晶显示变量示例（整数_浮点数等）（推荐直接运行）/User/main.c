/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   3-液晶显示变量示例（整数_浮点数等）（推荐直接运行）
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include <stdlib.h>
#include "./led/bsp_led.h" 
#include "./lcd/bsp_ili9806g_lcd.h"

static void LCD_Test(void);	
static void Delay ( __IO uint32_t nCount );

void Printf_Charater(void)   ;

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main ( void )
{
  SystemClock_Config();
  
  LED_GPIO_Config();
	
	
	/* USART config */
	DEBUG_USART_Config();		

	//LCD 初始化
	ILI9806G_Init ();         

	printf("\r\n ********** 液晶屏中文显示程序（字库在外部FLASH）*********** \r\n"); 
	printf("\r\n 若汉字显示不正常，请阅读工程中的readme.txt文件说明，根据要求给FLASH重刷字模数据\r\n"); 

 //其中0、3、5、6 模式适合从左至右显示文字，
 //不推荐使用其它模式显示文字	其它模式显示文字会有镜像效果			
 //其中 6 模式为大部分液晶例程的默认显示方向  
	ILI9806G_GramScan ( 6 );
	ILI9806G_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */

	Printf_Charater();
	
	while ( 1 )
	{
		LCD_Test();
	}
	
	
}



/*用于测试各种液晶的函数*/
void LCD_Test(void)
{
	/*演示显示变量*/
	static uint8_t testIntCNT = 0;	
	static double testFloatCNT = 0;	
	char dispBuff[100];
	char *pStr = 0;
	
	testIntCNT++;	
	testFloatCNT += 0.1;
	
	LCD_SetFont(&Font16x32);	
	LCD_SetColors(RED,BLACK);
	

	/********显示变量示例*******/
	/*使用c标准库把变量转化成字符串*/
	sprintf(dispBuff,"显示整型变量： %d ",testIntCNT);
  LCD_ClearLine(LINE(5));	/* 清除单行文字 */
	
	/*然后显示该字符串即可，其它变量也是这样处理*/
	ILI9806G_DispStringLine_EN_CH(LINE(5),dispBuff);
	
	
	/*使用c标准库把变量转化成字符串*/
	sprintf(dispBuff,"显示浮点型变量： %f ",testFloatCNT);
  LCD_ClearLine(LINE(6));	/* 清除单行文字 */
	
	/*然后显示该字符串即可，其它变量也是这样处理*/
	ILI9806G_DispStringLine_EN_CH(LINE(6),dispBuff);


	/*使用c标准库把变量转化成字符串*/
	sprintf(dispBuff,"浮点型(保留2位小数)： %.2f ",testFloatCNT);
  LCD_ClearLine(LINE(7));	/* 清除单行文字 */
	
	/*然后显示该字符串即可，其它变量也是这样处理*/
	ILI9806G_DispStringLine_EN_CH(LINE(7),dispBuff);
	
	/********居中显示示例*******/
	LCD_SetTextColor(GREEN);

	pStr = "插入2个英文空格示例";
	//使用 %*c 在字符串前插入指定个数的英文空格
	sprintf(dispBuff,"%*c%s",2,' ',pStr);
	
	LCD_ClearLine(LINE(9));												// 清除单行文字 
	ILI9806G_DispStringLine_EN_CH(LINE(9),dispBuff);//显示格式化后的字符串

	ILI9806G_DispStringLine_EN_CH(LINE(11),"纯英文24x32居中示例：");
	LCD_SetFont(&Font24x48);	
	pStr = "ABCDEF";
	//居中时，要插入的空格个数 = (液晶宽度/单个字体宽度 - 字符串长度)/2
	sprintf(dispBuff,"%*c%s",((LCD_X_LENGTH/(((sFONT *)LCD_GetFont())->Width))-strlen(pStr))/2,' ',pStr);
 
	LCD_ClearLine(LINE(8));													// 清除单行文字 
	ILI9806G_DispStringLine_EN_CH(LINE(8),dispBuff);//显示格式化后的字符串
	
	LCD_SetFont(&Font16x32);	
	pStr = "中文居中示例";
	//居中时，要插入的空格个数 = (液晶宽度/字体宽度 - 字符串长度)/2
	//strlen计算长度时，一个中文等于2个字节，即2个英文字符，而且插入的是英文空格
	//所以用(WIDTH_CH_CHAR/2)来计算字体宽度
	sprintf(dispBuff,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen(pStr))/2,' ',pStr);
	
	LCD_ClearLine(LINE(15));													// 清除单行文字 
	ILI9806G_DispStringLine_EN_CH(LINE(15),dispBuff);	//显示格式化后的字符串
	
	
	pStr = "含英文居中示例ABCDEFG";
	//居中时，要插入的空格个数 = (液晶宽度/字体宽度 - 字符串长度)/2
	//strlen计算长度时，一个中文等于2个字节，即2个英文字符，而且插入的是英文空格
	//所以用(WIDTH_CH_CHAR/2)来计算字体宽度
	sprintf(dispBuff,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen(pStr))/2,' ',pStr);
 
	LCD_ClearLine(LINE(16));												// 清除单行文字 
	ILI9806G_DispStringLine_EN_CH(LINE(16),dispBuff);//显示格式化后的字符串
	
	Delay(0x4ffffff);
}


/**
  * @brief  简单延时函数
  * @param  nCount ：延时计数值
  * @retval 无
  */	
static void Delay ( __IO uint32_t nCount )
{
  for ( ; nCount != 0; nCount -- );
	
}



/*"当"字符的字模16x16 */ 	  
unsigned char charater_matrix[] =    
{ /*"当",0*/
0x01,0x00,0x21,0x08,0x11,0x08,0x09,0x10,0x09,0x20,0x01,0x00,0x7F,0xF8,0x00,	0x08,
0x00,0x08,0x00,0x08,0x3F,0xF8,0x00,0x08,0x00,0x08,0x00,0x08,0x7F,0xF8,0x00,0x08,

};   
  
/**
  * @brief  使用串口在上位机打印字模
	*					演示字模显示原理
  * @retval 无
  */
void Printf_Charater(void)   
{   
	int i,j;   
	unsigned char kk; 
  
	/*i用作行计数*/
  for ( i=0;i<16;i++)   
	{   
		/*j用作一字节内数据的移位计数*/
		/*一行像素的第一个字节*/
    for(j=0; j<8; j++)   
    {      
			/*一个数据位一个数据位地处理*/
      kk = charater_matrix[2*i] << j ;  //左移J位          
			if( kk & 0x80)     
			{   
				printf("*"); //如果最高位为1，输出*号，表示笔迹
			}   
			else  
			{   
				printf(" "); //如果最高位为0，输出空格，表示空白  
			}   
    }   
		/*一行像素的第二个字节*/
    for(j=0; j<8; j++)   
		{              
      kk = charater_matrix[2*i+1] << j ;  //左移J位   
       
			if( kk & 0x80)         
			{   
				printf("*"); //如果最高位为1，输出*号，表示笔迹   
			}   
			else  
			{   
				printf(" "); //如果最高位为0，输出空格，表示空白 
			}   
		}         
    printf("\n");    //输出完一行像素，换行 
	}   
	printf("\n\n"); 		//一个字输出完毕
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
