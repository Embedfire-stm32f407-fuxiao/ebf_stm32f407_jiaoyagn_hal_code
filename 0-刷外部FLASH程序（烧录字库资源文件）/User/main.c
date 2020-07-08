/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   USB�ļ�ϵͳ���̣�����SD����¼���̺�USB��ȡU�������޸�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407-���� ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include "stm32f4xx.h"
#include "./led/bsp_led.h" 
#include "./usart/bsp_debug_usart.h"
#include "./flash/bsp_spi_flash.h"
#include "./key/bsp_key.h" 
//#include "./delay/core_delay.h"  
/* FatFs includes component */
#include "ff.h"
#include "./res_mgr/RES_MGR.h"
#include "./USBAPP/usbh_bsp.h"
/**
  ******************************************************************************
  *                              �������
  ******************************************************************************
  */
DIR dir; 
FIL fnew;													/* �ļ����� */
UINT fnum;            			  /* �ļ��ɹ���д���� */
BYTE ReadBuffer[1024]={0};        /* �������� */
BYTE WriteBuffer[] =              /* д������*/
"��ӭʹ��Ұ��STM32 F407���������� �����Ǹ������ӣ��½��ļ�ϵͳ�����ļ�\r\n";  
char SDPath[4]; /* SD�߼�������·�� */
extern FATFS sd_fs[];	
FRESULT res_sd;                /* �ļ�������� */
extern char src_dir[];
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
uint8_t state = 0;

/* ������U���ж���������ļ�ϵͳĬ�Ϲ��صķ���û���ֿ��ļ����붨����������(#define Other_Part)(#define Cheak_file) */
/* ͨ�����ز�ͬ���̷�(��main����Other_Part�й��ص�"1:")���ҵ��ֿ��ļ����Ž���ע�͵������޸���ȷ���̷�������¼ */
/* ע��Ҫͬ���޸�RES_MGR.h�е�RESOURCE_DIR�̷�������ɨ���޸ĺ���̷�·���µ��ļ� */
#if 1
PARTITION VolToPart[]=
{
	{0,1},/* "0:" */
	{0,2},/* "1:" */
	{0,3}	/* "2:" */
};
#endif
int main(void)
{
    /* ϵͳʱ�ӳ�ʼ����400MHz */
  
    LED_GPIO_Config();
    LED1_ON;	
    /* ��ʼ��USART1 ����ģʽΪ 115200 8-N-1 */
    Debug_USART_Config();	
	/* ��ʼ��USB */
	USBH_Init(&USB_OTG_Core,
		USB_OTG_FS_CORE_ID,
		&USB_Host,
		&USBH_MSC_cb,
		&USR_cb);
	
    Key_GPIO_Config();
    SPI_FLASH_Init();
	printf("����U�̺���ʾ��Device Attached����������ʾU���Ѽ�⵽������KEY1��������\n");
	 while(1)
	 {
		 /* USB״̬��⴦�������ڼ���豸����ʱʹ�� */
		USBH_Process(&USB_OTG_Core, &USB_Host);
		 
		if( Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON  )
		{
			//���ⲿSD�������ļ�ϵͳ���ļ�ϵͳ����ʱ���SD����ʼ��
			res_sd = f_mount(&sd_fs[0],"0:",1);  
			if(res_sd != FR_OK)
			{
			  printf("f_mount 1ERROR!������������U��Ȼ�����¸�λ������!");
			  LED3_ON;
			  while(1);
			}
#ifdef Other_Part
			res_sd = f_mount(&sd_fs[1],"1:",1);
		  
			if(res_sd != FR_OK)
			{
			  printf("f_mount 2ERROR!������������U��Ȼ�����¸�λ������!");
			  LED3_ON;
			  while(1);
			}

			res_sd = f_chdrive("1:");
			if(res_sd != FR_OK)
			{
			  printf("f_chdir ERROR!!");
			  LED3_ON;
			  while(1);
			}

			FRESULT fr;
			TCHAR str [100];
			fr = f_getcwd(str,100);  /*��ȡ��ǰĿ¼·��*/
			printf("%s\n",str);
#endif
#if 1    
			printf("\r\n ��һ��KEY1��ʼ��д�ֿⲢ�����ļ���FLASH�� \r\n"); 
			printf("\r\n ע��ò������FLASH��ԭ���ݻᱻɾ������ \r\n"); 

			while(Key_Scan(KEY1_GPIO_PORT, KEY1_PIN) == KEY_OFF);
			printf("\r\n ���ڽ�����Ƭ������ʱ��ܳ��������ĵȺ�...\r\n"); 
#ifndef Cheak_file
			SPI_FLASH_BulkErase();
#endif
			/* ��һ��ִ��Make_Catalog����ʱɾ���ɵ���¼��Ϣ�ļ� */
			res_sd = f_unlink(BURN_INFO_NAME_FULL);
			if ( res_sd == FR_OK )
			{
			  printf("����ɾ���ļ��ɹ���(%d)\n",res_sd);
			}
			else
			{
			  printf("����ɾ���ļ�ʧ�ܣ�(%d)\n",res_sd);
			}
			
			/* ������¼Ŀ¼��Ϣ�ļ� */
			Make_Catalog(src_dir,0);
#ifndef Cheak_file
			/* ��¼ Ŀ¼��Ϣ��FLASH*/
			Burn_Catalog();
			 /* ���� Ŀ¼ ��¼������FLASH*/
			Burn_Content();
			/* У����¼������ */
			Check_Resource(); 
#endif
			LED_ALLTOGGLE;
		}
	}
#endif    
}
	  

