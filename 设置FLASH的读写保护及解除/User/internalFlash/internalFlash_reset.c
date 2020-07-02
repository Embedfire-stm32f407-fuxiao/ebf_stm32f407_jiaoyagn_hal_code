/**
  ******************************************************************************
  * @file    bsp_internalFlash.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   �ָ�ѡ���ֽ�ΪĬ��ֵ����(�������д����)
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./internalFlash/internalFlash_reset.h" 

FLASH_OBProgramInitTypeDef OBInit;

/**
  * @brief  InternalFlash_Reset,�ָ��ڲ�FLASH��Ĭ������
  * @param  None
  * @retval None
  */
HAL_StatusTypeDef InternalFlash_Reset(void)
{

	/* ʹ�ܷ���ѡ���ֽڼĴ��� */
	HAL_FLASH_OB_Unlock();

	FLASH_INFO("\r\n");
	FLASH_INFO("����׼���ָ��������������ĵȴ�...");
	//ѡ���ֽ�ȫ���ָ�Ĭ��ֵ
	OBInit.OptionType = OPTIONBYTE_WRP|OPTIONBYTE_RDP|OPTIONBYTE_USER|OPTIONBYTE_BOR;
						
	OBInit.WRPSector  = OB_WRP_SECTOR_All;
	OBInit.RDPLevel  = OB_RDP_LEVEL_0;
	OBInit.USERConfig  = OB_IWDG_SW|OB_STOP_NO_RST|OB_STDBY_NO_RST;
//					             OB_DUAL_BOOT_DISABLE;
	OBInit.BORLevel = OB_BOR_OFF;
//	OBInit.BootAddr0 = OB_BOOTADDR_ITCM_FLASH;
//	OBInit.BootAddr1 = OB_BOOTADDR_SYSTEM;
	HAL_FLASHEx_OBProgram(&OBInit);

	if (HAL_FLASH_OB_Launch() != HAL_OK)
	{
		FLASH_ERROR("��ѡ���ֽڱ�̳����ָ�ʧ��");
		return HAL_ERROR;
	}
	FLASH_INFO("�ָ�ѡ���ֽ�Ĭ��ֵ�ɹ���");
	//��ֹ����
	HAL_FLASH_OB_Lock();
			
	return HAL_OK;
}




#define FLASH_WRP_SECTORS   (OB_WRP_SECTOR_0|OB_WRP_SECTOR_1) 
__IO uint32_t SectorsWRPStatus = 0xFFF;

/**
  * @brief  WriteProtect_Test,��ͨ��д��������
  * @param  ���б�������������FLASH_WRP_SECTORS����д���������ظ�һ�λ���н�д����
  * @retval None
  */
void WriteProtect_Test(void)
{
	/* ��ȡ������д����״̬ */
	HAL_FLASHEx_OBGetConfig(&OBInit);
	SectorsWRPStatus = OBInit.WRPSector & FLASH_WRP_SECTORS;

	if (SectorsWRPStatus == 0x00)
	{
		/* �����ѱ�д������ִ�нⱣ������*/

		/* ʹ�ܷ���OPTCR�Ĵ��� */
		HAL_FLASH_OB_Unlock();

		HAL_FLASH_Unlock();
		/* ���ö�Ӧ��nWRPλ�����д���� */
		OBInit.OptionType = OPTIONBYTE_WRP;
		OBInit.WRPState   = OB_WRPSTATE_DISABLE;
		OBInit.WRPSector  = FLASH_WRP_SECTORS;
		HAL_FLASHEx_OBProgram(&OBInit);
		/* ��ʼ��ѡ���ֽڽ��б�� */  
		if (HAL_FLASH_OB_Launch() != HAL_OK)
		{
			FLASH_ERROR("��ѡ���ֽڱ�̳������д����ʧ��");
			while (1)
			{
			}
		}
		/* ��ֹ����OPTCR�Ĵ��� */
		HAL_FLASH_OB_Lock();
		HAL_FLASH_Lock();
		/* ��ȡ������д����״̬ */
		HAL_FLASHEx_OBGetConfig(&OBInit);
		SectorsWRPStatus = OBInit.WRPSector & FLASH_WRP_SECTORS;

		/* ����Ƿ����óɹ� */
		if (SectorsWRPStatus == FLASH_WRP_SECTORS)
		{
			FLASH_INFO("���д�����ɹ���");
		}
		else
		{
			FLASH_ERROR("δ���д������");
		}
	}
	else
	{ /* ������δ��д����������д�������� */

		/* ʹ�ܷ���OPTCR�Ĵ��� */
		HAL_FLASH_OB_Unlock();

		HAL_FLASH_Unlock();
		/*ʹ�� FLASH_WRP_SECTORS ����д���� */
		OBInit.OptionType = OPTIONBYTE_WRP;
		OBInit.WRPState   = OB_WRPSTATE_ENABLE;
		OBInit.WRPSector  = FLASH_WRP_SECTORS;
		HAL_FLASHEx_OBProgram(&OBInit); 

		/* ��ʼ��ѡ���ֽڽ��б�� */  
		if (HAL_FLASH_OB_Launch() != HAL_OK)
		{
			FLASH_ERROR("��ѡ���ֽڱ�̳������д����ʧ��");
			while (1)
			{
			}
		}

		/* ��ֹ����OPTCR�Ĵ��� */
		HAL_FLASH_OB_Lock();

		HAL_FLASH_Lock();

		/* ��ȡ������д����״̬ */
		HAL_FLASHEx_OBGetConfig(&OBInit);
		SectorsWRPStatus = OBInit.WRPSector & FLASH_WRP_SECTORS;

		/* ����Ƿ����óɹ� */
		if (SectorsWRPStatus == 0x00)
		{
			FLASH_INFO("����д�����ɹ���");
		}
		else
		{
			FLASH_ERROR("����д����ʧ�ܣ�");
		}
	}
}


/**
  * @brief  OptionByte_Info,��ӡѡ���ֽڵ�������Ϣ
  * @param  None
  * @retval None
  */
void OptionByte_Info(void)
{
	uint32_t temp;
	uint8_t sector_num;
	
	HAL_FLASHEx_OBGetConfig(&OBInit);	
	
	//����������
	FLASH_INFO("\r\n");
	FLASH_INFO("-------------����������-------------");
	if(OBInit.RDPLevel==OB_RDP_LEVEL_0)
	{
		FLASH_INFO("STM32�ڲ�FLASH����������ΪLEVEL0");	
	}
	else 
	{
		//����ֵ����LEVEL_0ʱ������ΪLEVEL1��LEVEL2��
		//����LEVEL2ʱ����RAM���Զ��������У����������޷����ص�оƬ��
		//��LEVEL2ʱоƬ�޷��ٽ���
		FLASH_INFO("�ڲ�FLASH����������ΪLEVEL1");
	}
	
	FLASH_DEBUG("\r\n");
	FLASH_DEBUG("-------------д���Ļ�����-------------");
	if(OBInit.WRPSector == OB_WRP_SECTOR_All)
	{
		FLASH_DEBUG("Ĭ��ֵ���޶�д����");
	}
	else
	{
		temp = OBInit.WRPSector >>16;
		for(sector_num=0;sector_num<12;sector_num++)
		{
			if(!((temp>>sector_num)&0x001))
			{
				FLASH_DEBUG("sector%d��д����",sector_num);
			}
		}
		
	}


	
	FLASH_DEBUG("\r\n");	
	FLASH_DEBUG("-------------�û�ѡ���ֽ�USER------------");
	temp = OBInit.USERConfig;
	
	if(temp&0x01)
		FLASH_DEBUG("Ĭ��ֵ������������Ź�");
	else
		FLASH_DEBUG("Ӳ���������Ź�");
	
	if(temp&(0x01<<1))
		FLASH_DEBUG("Ĭ��ֵ������ֹͣģʽʱ��������λ");
	else
		FLASH_DEBUG("����ֹͣģʽʱ������λ");

	if(temp&(0x01<<2))	
		FLASH_DEBUG("Ĭ��ֵ���������ģʽʱ��������λ");
	else
		FLASH_DEBUG("�������ģʽʱ������λ");
	
	FLASH_DEBUG("\r\n");	
	FLASH_DEBUG("-------------��λ����BOR_LEV------------");
//	temp = FLASH_OB_GetBOR();
  
	switch(OBInit.BORLevel)
	{
		case OB_BOR_LEVEL3:
				FLASH_DEBUG("��λ��ֵ��ѹΪ 2.70 V �� 3.60 V");
				break;
		
		case OB_BOR_LEVEL2:
				FLASH_DEBUG("��λ��ֵ��ѹΪ 2.40 V �� 2.70 V");
				break;
		
		case OB_BOR_LEVEL1:
				FLASH_DEBUG("��λ��ֵ��ѹΪ 2.10 V �� 2.40 V");
				break;
			
		case OB_BOR_OFF:
				FLASH_DEBUG("Ĭ��ֵ����λ��ֵ��ѹΪ 1.80 V �� 2.10 V");
				break;	
	}			
				
				

}	











