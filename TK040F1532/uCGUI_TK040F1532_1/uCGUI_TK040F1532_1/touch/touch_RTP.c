#include "gui.h"

#ifdef  use_resistance_touch_panel

#include  "touch_RTP.h"

void GUI_TOUCH_X_ActivateX(void) {}
void GUI_TOUCH_X_ActivateY(void) {}

//*************************************************
//�������� : void Touch_GPIO_Config(void)  
//�������� : ���ô�����SPI����,�����ģ��ķ���ʵ��SPI����
//������� : 
//������� : 
//����ֵ   : 
//*************************************************

void Touch_GPIO_Config(void) 
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
  SPI_InitTypeDef   SPI_InitStructure; 

  //GPIOA Periph clock enable
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);   
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE ) ;

  //Configure SPI2 pins: SCK, MISO and MOSI 
  GPIO_InitStructure.GPIO_Pin = SPI_CLK|SPI_MISO|SPI_MOSI; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //�����������
  GPIO_Init(GPIOB,&GPIO_InitStructure);  

  //Configure PF10 pin: TP_CS pin 
  GPIO_InitStructure.GPIO_Pin = SPI_CS; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//�������
  GPIO_Init(GPIOB,&GPIO_InitStructure); 
    
    /* Configure PC.04 as input floating For TP_IRQ*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz ;	  
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOD,GPIO_Pin_12);	//ʧ�ܰ���FLASHоƬ�������ͷ�SPI����

  // SPI1 Config  
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master; 
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; 
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; 
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;   //  SPI_NSS_Hard
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; 
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 
  SPI_InitStructure.SPI_CRCPolynomial = 7; 
  SPI_Init(SPI2,&SPI_InitStructure); 

  // SPI2 enable  
  SPI_Cmd(SPI2,ENABLE);  

  GPIO_ResetBits(GPIOB,SPI_CS);
}
/****************************************************************************
* ��    �ƣ�void SpiDelay(unsigned int DelayCnt) 
* ��    �ܣ�SPI1 д��ʱ����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/  
void SpiDelay(unsigned int DelayCnt)
{
 unsigned int i;
 for(i=0;i<DelayCnt;i++);
}
/****************************************************************************
* ��    �ƣ�unsigned char SPI_WriteByte(unsigned char data) 
* ��    �ܣ�SPI1 д����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/  
unsigned char SPI_WriteByte(unsigned char data) 
{ 
 unsigned char Data = 0; 

   //Wait until the transmit buffer is empty 
  while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET); 
  // Send the byte  
  SPI_I2S_SendData(SPI2,data); 

   //Wait until a data is received 
  while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET); 
  // Get the received data 
  Data = SPI_I2S_ReceiveData(SPI2); 

  // Return the shifted data 
  return Data; 
}  


/****************************************************************************
* ��    �ƣ�u16 TPReadX(void) 
* ��    �ܣ�������X�����ݶ���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/  
u16 TPReadY(void)
{ 
   u16 x=0;
   TP_CS(0);	                        //ѡ��XPT2046 
   SpiDelay(10);					//��ʱ
   SPI_WriteByte(0xD0);				//����X���ȡ��־
   SpiDelay(10);					//��ʱ
   x=SPI_WriteByte(0x00);			//������ȡ16λ������ 
   x<<=8;
   x+=SPI_WriteByte(0x00);
   SpiDelay(10);					//��ֹXPT2046
   TP_CS(1); 					    								  
   x = x>>3;						//��λ�����12λ����Ч����0-4095
   return (x);
}
/****************************************************************************
* ��    �ƣ�u16 TPReadX(void)
* ��    �ܣ�������Y�����ݶ���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/
u16 TPReadX(void)
{
   u16 y=0;
   TP_CS(0);	                        //ѡ��XPT2046 
   SpiDelay(10);					//��ʱ
   SPI_WriteByte(0x90);				//����Y���ȡ��־
   SpiDelay(10);					//��ʱ
   y=SPI_WriteByte(0x00);			//������ȡ16λ������ 
   y<<=8;
   y+=SPI_WriteByte(0x00);
   SpiDelay(10);					//��ֹXPT2046
   TP_CS(1); 					    								  
   y = y>>3;						//��λ�����12λ����Ч����0-4095
   return (y);
}

int  GUI_TOUCH_X_MeasureX(void) 
{
	unsigned char t=0,t1,count=0;
	unsigned short int databuffer[10]={5,7,9,3,2,6,4,0,3,1};//������
	unsigned short temp=0,X=0;	
 	
	while(/*GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0&&*/count<10)//ѭ������10��
	{	   	  
		databuffer[count]=TPReadX();
		count++; 
	}  
	if(count==10)//һ��Ҫ����10������,������
	{  
	    do//������X��������
		{	
			t1=0;		  
			for(t=0;t<count-1;t++)
			{
				if(databuffer[t]>databuffer[t+1])//��������
				{
					temp=databuffer[t+1];
					databuffer[t+1]=databuffer[t];
					databuffer[t]=temp;
					t1=1; 
				}  
			}
		}while(t1); 	    		 	 		  
		X=(databuffer[3]+databuffer[4]+databuffer[5])/3;	  
	}
	return(X);  
}

int  GUI_TOUCH_X_MeasureY(void) {
  	unsigned char t=0,t1,count=0;
	unsigned short int databuffer[10]={5,7,9,3,2,6,4,0,3,1};//������
	unsigned short temp=0,Y=0;	
 
    while(/*GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0&&*/count<10)	//ѭ������10��
	{	   	  
		databuffer[count]=TPReadY();
		count++;  
	}  
	if(count==10)//һ��Ҫ����10������,������
	{  
	    do//������X��������
		{	
			t1=0;		  
			for(t=0;t<count-1;t++)
			{
				if(databuffer[t]>databuffer[t+1])//��������
				{
					temp=databuffer[t+1];
					databuffer[t+1]=databuffer[t];
					databuffer[t]=temp;
					t1=1; 
				}  
			}
		}while(t1); 	    		 	 		  
		Y=(databuffer[3]+databuffer[4]+databuffer[5])/3;	    
	}
	return(Y); 
}



////�������ж�������������
//void Touch_Interrupt_Config(void)
//{
//  GPIO_InitTypeDef  GPIO_InitStructure; 
//  NVIC_InitTypeDef NVIC_InitStructure;
//  EXTI_InitTypeDef EXTI_InitStructure;
//
//
//  //++++++++++�������ж�����+++++++++++
//  // Configure GPIO Pin as input floating 
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOD, &GPIO_InitStructure);
//
//  // Connect EXTI Line to GPIO Pin
//  GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource8);
//  // Enable the EXTI8 Interrupt //
//  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQChannel;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//
//  //���������ж�����ΪPD8
//  // Enable the EXTI Line8 Interrupt //
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  EXTI_InitStructure.EXTI_Line = EXTI_Line8;
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);
//
//} 

#endif

