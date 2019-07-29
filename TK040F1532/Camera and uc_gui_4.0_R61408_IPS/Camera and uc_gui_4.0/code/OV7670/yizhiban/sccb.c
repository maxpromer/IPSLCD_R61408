//-------------------------------------------------
//��ֲ���°���ĿV2
//�޸���:������
//2010/11/12
//-------------------------------------------------
/*
�޸���ʷ:
1,2010/11/20 12:11,�޸���
  void SCCB_GPIO_Config(void),
  void SCCB_SID_GPIO_OUTPUT(void),
  void SCCB_SID_GPIO_INPUT(void)
*/

#include "sccb.h"
#include "delay.h"
#include "stm32f10x_lib.h"


/*
-----------------------------------------------
   ����: ��ʼ��ģ��SCCB�ӿ�
   ����: ��
 ����ֵ: ��
-----------------------------------------------
*/
void SCCB_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
 
 #if SIC_SID_PORT_SAME

  /* Enable GPIOA  clock */
  RCC_APB2PeriphClockCmd(SIC_SID_PORT_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  SCCB_SIC_BIT|SCCB_SID_BIT;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
  GPIO_Init(SIC_SID_PORT, &GPIO_InitStructure);
 #else

  RCC_APB2PeriphClockCmd(SIC_PORT_CLK, ENABLE);  
  RCC_APB2PeriphClockCmd(SID_PORT_CLK, ENABLE);

  GPIO_InitStructure.GPIO_Pin =  SCCB_SIC_BIT;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
  GPIO_Init(SIC_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  SCCB_SID_BIT;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
  GPIO_Init(SID_PORT, &GPIO_InitStructure);
#endif

}


/*
-----------------------------------------------
   ����: ����SID�˿�Ϊ���
   ����: ��
 ����ֵ: ��
-----------------------------------------------
*/
void SCCB_SID_GPIO_OUTPUT(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
   /* Enable GPIOC  clock */
  RCC_APB2PeriphClockCmd(SID_PORT_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  SCCB_SID_BIT;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SID_PORT, &GPIO_InitStructure);
}


/*
-----------------------------------------------
   ����: ����SID�˿�Ϊ����
   ����: ��
 ����ֵ: ��
-----------------------------------------------
*/
void SCCB_SID_GPIO_INPUT(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
   /* Enable GPIOC  clock */
  RCC_APB2PeriphClockCmd(SID_PORT_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  SCCB_SID_BIT;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SID_PORT, &GPIO_InitStructure);
}

/*
-----------------------------------------------
   ����: start����,SCCB����ʼ�ź�
   ����: ��
 ����ֵ: ��
-----------------------------------------------
*/
void startSCCB(void)
{
    SCCB_SID_H();     //�����߸ߵ�ƽ
    OV7670_delay_us(500);

    SCCB_SIC_H();	   //��ʱ���߸ߵ�ʱ���������ɸ�����
    OV7670_delay_us(500);
 
    SCCB_SID_L();
    OV7670_delay_us(500);

    SCCB_SIC_L();	 //�����߻ָ��͵�ƽ��������������Ҫ
    OV7670_delay_us(500);


}
/*
-----------------------------------------------
   ����: stop����,SCCB��ֹͣ�ź�
   ����: ��
 ����ֵ: ��
-----------------------------------------------
*/
void stopSCCB(void)
{
    SCCB_SID_L();
    OV7670_delay_us(500);
 
    SCCB_SIC_H();	
    OV7670_delay_us(500);
  

    SCCB_SID_H();	
    OV7670_delay_us(500);
   
}

/*
-----------------------------------------------
   ����: noAck,����������ȡ�е����һ����������
   ����: ��
 ����ֵ: ��
-----------------------------------------------
*/
void noAck(void)
{
	
	SCCB_SID_H();	
	OV7670_delay_us(500);
	
	SCCB_SIC_H();	
	OV7670_delay_us(500);
	
	SCCB_SIC_L();	
	OV7670_delay_us(500);
	
	SCCB_SID_L();	
	OV7670_delay_us(500);

}

/*
-----------------------------------------------
   ����: д��һ���ֽڵ����ݵ�SCCB
   ����: д������
 ����ֵ: ���ͳɹ�����1������ʧ�ܷ���0
-----------------------------------------------
*/
unsigned char SCCBwriteByte(unsigned char m_data)
{
	unsigned char j,tem;

	for(j=0;j<8;j++) //ѭ��8�η�������
	{
		if((m_data<<j)&0x80)
		{
			SCCB_SID_H();	
		}
		else
		{
			SCCB_SID_L();	
		}
		OV7670_delay_us(500);
		SCCB_SIC_H();	
		OV7670_delay_us(500);
		SCCB_SIC_L();	
		OV7670_delay_us(500);

	}
	OV7670_delay_us(100);
	SCCB_SID_IN;/*����SDAΪ����*/
	OV7670_delay_us(500);
	SCCB_SIC_H();	
	OV7670_delay_us(100);
	if(SCCB_SID_STATE){tem=0;}   //SDA=1����ʧ�ܣ�����0}
	else {tem=1;}   //SDA=0���ͳɹ�������1
	SCCB_SIC_L();	
	OV7670_delay_us(500);	
    SCCB_SID_OUT;/*����SDAΪ���*/

	return (tem);  
}

/*
-----------------------------------------------
   ����: һ���ֽ����ݶ�ȡ���ҷ���
   ����: ��
 ����ֵ: ��ȡ��������
-----------------------------------------------
*/
unsigned char SCCBreadByte(void)
{
	unsigned char read,j;
	read=0x00;
	
	SCCB_SID_IN;/*����SDAΪ����*/
	OV7670_delay_us(500);
	for(j=8;j>0;j--) //ѭ��8�ν�������
	{		     
		OV7670_delay_us(500);
		SCCB_SIC_H();
		OV7670_delay_us(500);
		read=read<<1;
		if(SCCB_SID_STATE) 
		{
			read=read+1;
		}
		SCCB_SIC_L();
		OV7670_delay_us(500);
	}	
	return(read);
}
