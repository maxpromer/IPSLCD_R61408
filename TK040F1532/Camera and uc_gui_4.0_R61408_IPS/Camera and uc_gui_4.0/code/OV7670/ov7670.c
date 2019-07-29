
/*
GUANFU_WANG
*/


#include "ov7670.h"
#include "delay.h"
#include "ov7670config.h"
#include "stm32f10x.h"


extern const char change_reg[CHANGE_REG_NUM][2];
////////////////////////////
//���ܣ��ṩ7660ʱ��
//���أ���
void XCLK_init_ON(void)
{
  
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    RCC_MCOConfig(RCC_MCO_HSE);//hsi	   //ָ�����������ʱ��Դ PA8
}

void XCLK_init_OFF(void)
{  
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void ov7670_GPIO_Init(void)
{  
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin =  OV7670_VSYNC_BIT;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 

  GPIO_InitStructure.GPIO_Pin =  OV7670_HREF_BIT;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
}

/********************************************************************
�������ܣ�7670�ж�����
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void ov7670_Interrupts_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure one bit for preemption priority */
  //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
  /* Enable the EXTI9_5 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//��ռ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//�����ȼ�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}




void OV7670_EXTI_Config(void)	     //
{
  EXTI_InitTypeDef EXTI_InitStructure;	

  GPIO_EXTILineConfig(GPIO_PORT_SOURCE_VSYNC_CMOS, GPIO_PIN_SOURCE_VSYNC_CMOS);
  EXTI_InitStructure.EXTI_Line = EXTI_LINE_VSYNC_CMOS;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  EXTI_GenerateSWInterrupt(EXTI_LINE_VSYNC_CMOS);
}


////////////////////////////
//���ܣ�дOV7660�Ĵ���
//���أ�1-�ɹ�	0-ʧ��
unsigned char wrOV7670Reg(unsigned char regID, unsigned char regDat)
{
	startSCCB();
	if(0==SCCBwriteByte(0x42))
	{
		stopSCCB();
		return(0);
	}
	OV7670_delay_us(100);
  	if(0==SCCBwriteByte(regID))
	{
		stopSCCB();
		return(0);
	}
	OV7670_delay_us(100);
  	if(0==SCCBwriteByte(regDat))
	{
		stopSCCB();
		return(0);
	}
  	stopSCCB();
	
  	return(1);
}
////////////////////////////
//���ܣ���OV7660�Ĵ���
//���أ�1-�ɹ�	0-ʧ��
unsigned char rdOV7670Reg(unsigned char regID, unsigned char *regDat)
{
	//ͨ��д�������üĴ�����ַ
	startSCCB();
	if(0==SCCBwriteByte(0x42))
	{
		stopSCCB();
		return(0);
	}
	OV7670_delay_us(100);
  	if(0==SCCBwriteByte(regID))
	{
		stopSCCB();
		return(0);
	}
	stopSCCB();
	
	OV7670_delay_us(100);
	
	//���üĴ�����ַ�󣬲��Ƕ�
	startSCCB();
	if(0==SCCBwriteByte(0x43))
	{
		stopSCCB();
		return(0);
	}
	OV7670_delay_us(100);
  	*regDat=SCCBreadByte();
  	noAck();
  	stopSCCB();
  	return(1);
}


/* OV7670_init() */
//����1�ɹ�������0ʧ��
unsigned char OV7670_init(void)
{
	unsigned char temp;
	
	unsigned int i=0;

	//uchar ovidmsb=0,ovidlsb=0;
	ov7670_GPIO_Init();
	SCCB_GPIO_Config();//io init..

    XCLK_init_ON();
	temp=0x80;

	if(0==wrOV7670Reg(0x12, temp)) //Reset SCCB
	{
          return 0 ;
	}
	OV7670_delay_ms(50);

  
        for(i=0;i<CHANGE_REG_NUM;i++)
	{
		if( 0==wrOV7670Reg(change_reg[i][0],change_reg[i][1]))
		{
			return 0;
		}
	}

	return 1; //ok
} 

