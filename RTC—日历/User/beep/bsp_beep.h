#ifndef __BEEP_H
#define	__BEEP_H


#include "stm32f4xx.h"


/* ������������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����Ĵ��뼴�ɸı���Ƶķ��������� */
#define BEEP_GPIO_PORT    	    GPIOG		              /* GPIO�˿� */
#define BEEP_GPIO_CLK_ENABLE()  __GPIOG_CLK_ENABLE()  /* GPIO�˿�ʱ�� */
#define BEEP_GPIO_PIN		  	GPIO_PIN_7			          /* ���ӵ���������GPIO */


/* ���κ꣬��������������һ��ʹ�� */
#define BEEP(a)	HAL_GPIO_WritePin(BEEP_GPIO_PORT,BEEP_GPIO_PIN,a)
					
/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	beep_digitalHi(p,i)			{p->BSRR = i;}			        //����Ϊ�ߵ�ƽ		
#define beep_digitalLo(p,i)			{p->BSRR = (uint32_t)i << 16;;}  //����͵�ƽ
#define beep_digitalToggle(p,i)	    {p->ODR ^=i;}			//�����ת״̬


/* �������IO�ĺ� */
#define BEEP_TOGGLE		beep_digitalToggle(BEEP_GPIO_PORT,BEEP_GPIO_PIN)
#define BEEP_ON	        beep_digitalHi(BEEP_GPIO_PORT,BEEP_GPIO_PIN)
#define BEEP_OFF		beep_digitalLo(BEEP_GPIO_PORT,BEEP_GPIO_PIN)

void BEEP_GPIO_Config(void);
					
#endif /* __BEEP_H */
