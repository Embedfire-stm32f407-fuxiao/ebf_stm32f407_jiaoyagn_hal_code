
/*Ƭ���������ַ  */
#define PERIPH_BASE           ((unsigned int)0x40000000)                          

/*���߻���ַ */
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000)	

/*GPIO�������ַ*/
#define GPIOA_BASE            (AHB1PERIPH_BASE + 0x0000)


/* GPIOA�Ĵ�����ַ,ǿ��ת����ָ�� */
#define GPIOA_MODER				*(unsigned int*)(GPIOA_BASE+0x00)
#define GPIOA_OTYPER			*(unsigned int*)(GPIOA_BASE+0x04)
#define GPIOA_OSPEEDR			*(unsigned int*)(GPIOA_BASE+0x08)
#define GPIOA_PUPDR				*(unsigned int*)(GPIOA_BASE+0x0C)
#define GPIOA_IDR					*(unsigned int*)(GPIOA_BASE+0x10)
#define GPIOA_ODR					*(unsigned int*)(GPIOA_BASE+0x14)
#define GPIOA_BSRR					*(unsigned int*)(GPIOA_BASE+0x18)
#define GPIOA_LCKR					*(unsigned int*)(GPIOA_BASE+0x1C)
#define GPIOA_AFRL					*(unsigned int*)(GPIOA_BASE+0x20)
#define GPIOA_AFRH					*(unsigned int*)(GPIOA_BASE+0x24)

/*RCC�������ַ*/
#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800)

/*RCC��AHB1ʱ��ʹ�ܼĴ�����ַ,ǿ��ת����ָ��*/
#define RCC_AHB1ENR				*(unsigned int*)(RCC_BASE+0x30)
	

