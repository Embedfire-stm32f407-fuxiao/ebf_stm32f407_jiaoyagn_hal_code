
/*
	ʹ�üĴ����ķ�������LED��
  */
#include "stm32f4xx.h" 


/**
  *   ������
  */
int main(void)
{	
	/*���� GPIOA ʱ�ӣ�ʹ������ʱ��Ҫ�ȿ�������ʱ��*/
	RCC_AHB1ENR |= ((unsigned int)1<<0);	
	
	/* LED �˿ڳ�ʼ�� */
	
	/*GPIOA MODER15���*/
	GPIOA_MODER  &= ~( (unsigned int)0x3<< (2*15));	
	/*PA15 MODER15 = 01b ���ģʽ*/
	GPIOA_MODER |= ((unsigned int)1<<2*15);
	
	/*GPIOA OTYPER15���*/
	GPIOA_OTYPER &= ~((unsigned int)1<<1*15);
	/*PA15 OTYPER15 = 0b ����ģʽ*/
	GPIOA_OTYPER |= ((unsigned int)0<<1*15);
	
	/*GPIOA OSPEEDR15���*/
	GPIOA_OSPEEDR &= ~((unsigned int)0x3<<2*15);
	/*PA15 OSPEEDR15 = 0b ����2MHz*/
	GPIOA_OSPEEDR |= ((unsigned int)0<<2*15);
	
	/*GPIOA PUPDR15���*/
	GPIOA_PUPDR &= ~((unsigned int)0x3<<2*15);
	/*PA15 PUPDR15 = 01b ����ģʽ*/
	GPIOA_PUPDR |= ((unsigned int)1<<2*15);
	
	/*PA15 BSRR�Ĵ����� BR15��1��ʹ��������͵�ƽ*/
	GPIOA_BSRR |= ((unsigned int)1<<16<<15);
	
	/*PA15 BSRR�Ĵ����� BS15��1��ʹ��������ߵ�ƽ*/
//	GPIOA_BSRR |= ((unsigned int)1<<15);

	while(1);

}

// ����Ϊ�գ�Ŀ����Ϊ��ƭ��������������
void SystemInit(void)
{	
}






/*********************************************END OF FILE**********************/

