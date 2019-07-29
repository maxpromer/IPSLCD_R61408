#include "ili9325.h" 
#include  "touch.h"
#include  "lcm_api.h"

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

    GPIO_InitTypeDef GPIO_InitStructure ;
	//////������SPI���GPIO��ʼ��//////
    GPIO_InitStructure.GPIO_Pin=SPI_CLK|SPI_MOSI|SPI_CS;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz ;	  
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz ;	  
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC,&GPIO_InitStructure);
		GPIO_SetBits(GPIOC,GPIO_Pin_3);	
	
    GPIO_InitStructure.GPIO_Pin = SPI_MISO; 
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //GPIO_Mode_IN_FLOATING��������
    GPIO_Init(GPIOB,&GPIO_InitStructure);	
	
		GPIO_InitStructure.GPIO_Pin = TP_INT_PIN; 
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //GPIO_Mode_IN_FLOATING��������
    GPIO_Init(GPIOC,&GPIO_InitStructure);	

    GPIO_ResetBits(GPIOB,SPI_CS);
}

//====================================================================================
static void WR_CMD (unsigned char cmd) 
{
    unsigned char buf;
    unsigned char i;
//     TP_CS(1);
    TP_DIN(0);
    TP_DCLK(0);
//     TP_CS(0);
    for(i=0;i<8;i++) 
    {
        buf=(cmd>>(7-i))&0x1;
        TP_DIN(buf);
        //Delayus(5);
        TP_DCLK(1);
        //Delayus(5);
        TP_DCLK(0);
    }
}
//====================================================================================
static unsigned short RD_AD(void) 
{
    unsigned short buf=0,temp;
    unsigned char i;
    TP_DIN(0);
    TP_DCLK(1);
    for(i=0;i<12;i++) 
    {
        TP_DCLK(0);          
        temp= (TP_DOUT) ? 1:0;
        buf|=(temp<<(11-i));
        //Delayus(5);
        TP_DCLK(1);
    }
//     TP_CS(1);
    buf&=0x0fff;
    return(buf);
}
//====================================================================================
 int GUI_TOUCH_X_MeasureX(void) 
{ 
	int i[5],temp;
	u8 cou=0,k=0;
	WR_CMD(CHX);
	RD_AD();
	while(cou<5)	//ѭ������5��
	{	
		WR_CMD(CHX); 
		cou++;	
		i[cou]=RD_AD();		  
	}
        //��������������
	for(k=0;k<4;k++)
	{	  
		for(cou=1;cou<5-k;cou++)
		{
			if(i[cou]>i[cou+1])
			{
				temp=i[cou+1];
				i[cou+1]=i[cou];
				i[cou]=temp;
			}  
		}
	}
	return (i[1]+i[2]+i[3])/3;   
}
//====================================================================================
 int GUI_TOUCH_X_MeasureY(void) 
{ 
int i[5],temp;
	u8 cou=0,k=0;
	WR_CMD(CHY);
	RD_AD();
	while(cou<5)	//ѭ������5��
	{	
		WR_CMD(CHY); 
		cou++;	
		i[cou]=RD_AD();		  
	}
        //��������������
	for(k=0;k<4;k++)
	{	  
		for(cou=1;cou<5-k;cou++)
		{
			if(i[cou]>i[cou+1])
			{
				temp=i[cou+1];
				i[cou+1]=i[cou];
				i[cou]=temp;
			}  
		}
	}
	return (i[1]+i[2]+i[3])/3;
}
unsigned int TOUCH_X(void)
{
	unsigned int i;
	i=GUI_TOUCH_X_MeasureX();
	if(i<250||i>3800)return 800;
		else return (3800-i)*40/(380-25);//2011��6��3��֮�������3850-350��3600-1000��֮ǰ������3850-150
} 
unsigned int TOUCH_Y(void)
{
	unsigned int i;
	i=GUI_TOUCH_X_MeasureY();
	if(i<300||i>3800)return 800;
		else return (3800-i)*24/(380-30);
}

void TP_GetAdXY(unsigned int *x,unsigned int *y) 
{
    unsigned int adx,ady;
    adx=GUI_TOUCH_X_MeasureX();
    ady=GUI_TOUCH_X_MeasureY();
    *x=adx;
    *y=ady;
}
//*************************************************
//���ߣ�XING
//������������������
//����ʱ�䣺2010/11/08  
//����޸�ʱ�䣺
//*************************************************

//******************************************************
//�������� :  IsPressed
//�������� :  �жϵ�ǰ���ΰ�ť�Ƿ񱻰���
//������� :  *pressed����ð�����״̬ ,x,yΪ��ǰ����
//����ֵ   :  ����1��ʾ���£�����0��ʾ����
//******************************************************
char IsPressed(u16 x, u16 y,u16 x0, u16 y0, u16 lenth, u16 width,char* pressed)
{
  char state=*pressed;//����ǰһ�ε�״̬
  char Newstate=0;//���浱ǰ��״̬

  if(x>x0&&x<x0+lenth){
	 if(y>y0&&y<y0+width){//�ھ��η�Χ��

	     Newstate=1;//���浱ǰ��״̬

		 if(*pressed==0){//֮ǰδ������
		   	virtual_LCM_Rectangle_V2(x0, y0,lenth,width,BLUE,0); // ������
			*pressed=1;//����������
		  }

	  }else if(*pressed==1){//֮ǰ��������
	   	 virtual_LCM_Rectangle_V2(x0, y0,lenth,width,DGRAY,0); // ������
		 *pressed=0;
	  }
  }else if(*pressed==1){//֮ǰ��������
	  virtual_LCM_Rectangle_V2(x0, y0,lenth,width,DGRAY,0); // ������
	  *pressed=0;
  }

  if(state==0&&Newstate==1){//���µĶ�������
      //virtual_LCM_PutChar(x0+lenth/2-4,y0+width/2-8,'1',0x0000,0xffff,1);
	  return 1;
  }

  if(state==1&&Newstate==0){//����Ķ�������
      //virtual_LCM_PutChar(x0+lenth/2-4,y0+width/2-8,'0',0x0000,0xffff,1);
	  return 0;
  }

  if(state==0&&Newstate==0){//����������
      //virtual_LCM_PutChar(x0+lenth/2-4,y0+width/2-8,'2',0x0000,0xffff,1);
      return 2;
  }

  if(state==1&&Newstate==1){//���������޶���
      //virtual_LCM_PutChar(x0+lenth/2-4,y0+width/2-8,'3',0x0000,0xffff,1);
      return 3;
  }
	return 2;
}

//******************************************************
//�������� :  IsPressed_V2(ר����ͼƬ���)
//�������� :  �жϵ�ǰ���ΰ�ť�Ƿ񱻰���
//������� :  *pressed����ð�����״̬ ,x,yΪ��ǰ����
//����ֵ   :  ����1��ʾ���£�����0��ʾ����
//******************************************************
char IsPressed_V2(u16 x, u16 y,u16 x0, u16 y0, u16 lenth, u16 width,char* pressed)
{
  char state=*pressed;//����ǰһ�ε�״̬
  char Newstate=0;//���浱ǰ��״̬

  if(x>x0&&x<x0+lenth){
	 if(y>y0&&y<y0+width){//�ھ��η�Χ��

	     Newstate=1;//���浱ǰ��״̬

		 if(*pressed==0){//֮ǰδ������
		   	//virtual_LCM_Rectangle_V2(x0, y0,20,20,BLUE,0); // ������
			*pressed=1;//����������
		  }

	  }else if(*pressed==1){//֮ǰ��������
	   	 //virtual_LCM_Rectangle_V2(x0, y0,20,20,RED,0); // ������
		 *pressed=0;
	  }
  }else if(*pressed==1){//֮ǰ��������
	  //virtual_LCM_Rectangle_V2(x0, y0,20,20,RED,0); // ������
	  *pressed=0;
  }

  if(state==0&&Newstate==1){//���µĶ�������
      //virtual_LCM_PutChar(x0+lenth/2-4,y0+width/2-8,'1',0x0000,0xffff,1);
	  return 1;
  }

  if(state==1&&Newstate==0){//����Ķ�������
      //virtual_LCM_PutChar(x0+lenth/2-4,y0+width/2-8,'0',0x0000,0xffff,1);
	  return 0;
  }

  if(state==0&&Newstate==0){//����������
      //virtual_LCM_PutChar(x0+lenth/2-4,y0+width/2-8,'2',0x0000,0xffff,1);
      return 2;
  }

  if(state==1&&Newstate==1){//���������޶���
      virtual_LCM_Rectangle_V2(x0, y0,20,20,BLUE,0); // ������
      //virtual_LCM_PutChar(x0+lenth/2-4,y0+width/2-8,'3',0x0000,0xffff,1);
      return 3;
  }
return 2;
}

//******************************************************
//�������� :  IsPressed_V3(ר����ʱ������)
//�������� :  �жϵ�ǰ���ΰ�ť�Ƿ񱻰���
//������� :  *pressed����ð�����״̬ ,x,yΪ��ǰ����
//����ֵ   :  ����1��ʾ���£�����0��ʾ����
//******************************************************
char IsPressed_V3(u16 x, u16 y,u16 x0, u16 y0, u16 lenth, u16 width,char* pressed)
{
  char state=*pressed;//����ǰһ�ε�״̬
  char Newstate=0;//���浱ǰ��״̬

  if(x>x0&&x<x0+lenth){
	 if(y>y0&&y<y0+width){//�ھ��η�Χ��

	     Newstate=1;//���浱ǰ��״̬

		 if(*pressed==0){//֮ǰδ������
		   	//virtual_LCM_Rectangle_V2(x0, y0,20,20,BLUE,0); // ������
			*pressed=1;//����������
		  }

	  }else if(*pressed==1){//֮ǰ��������
	   	 //virtual_LCM_Rectangle_V2(x0, y0,20,20,RED,0); // ������
		 *pressed=0;
	  }
  }else if(*pressed==1){//֮ǰ��������
	  //virtual_LCM_Rectangle_V2(x0, y0,20,20,RED,0); // ������
	  *pressed=0;
  }

  if(state==0&&Newstate==1){//���µĶ�������
      //virtual_LCM_PutChar(x0+lenth/2-4,y0+width/2-8,'1',0x0000,0xffff,1);
	  return 1;
  }

  if(state==1&&Newstate==0){//����Ķ�������
      //virtual_LCM_PutChar(x0+lenth/2-4,y0+width/2-8,'0',0x0000,0xffff,1);
	  return 0;
  }

  if(state==0&&Newstate==0){//����������
      //virtual_LCM_PutChar(x0+lenth/2-4,y0+width/2-8,'2',0x0000,0xffff,1);
      return 2;
  }

  if(state==1&&Newstate==1){//���������޶���
      virtual_LCM_Rectangle_V2(x0-1, y0-1,lenth+1,width+1,BLUE,0); // ������
      //virtual_LCM_Rectangle_V2(x0, y0,20,20,BLUE,0); // ������
      //virtual_LCM_PutChar(x0+lenth/2-4,y0+width/2-8,'3',0x0000,0xffff,1);
      return 3;
  }
	return 2;
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

// static void WR_CMD (unsigned char cmd) 
// {
//     unsigned char buf;
//     unsigned char i;
// //     TP_CS(1);
//     TP_DIN(0);
//     TP_DCLK(0);
// //     TP_CS(0);
//     for(i=0;i<8;i++) 
//     {
//         buf=(cmd>>(7-i))&0x1;
//         TP_DIN(buf);
//         //Delayus(5);
//         TP_DCLK(1);
//         //Delayus(5);
//         TP_DCLK(0);
//     }
// }
// //====================================================================================
// static unsigned short RD_AD(void) 
// {
//     unsigned short buf=0,temp;
//     unsigned char i;
//     TP_DIN(0);
//     TP_DCLK(1);
//     for(i=0;i<12;i++) 
//     {
//         TP_DCLK(0);          
//         temp= (TP_DOUT) ? 1:0;
//         buf|=(temp<<(11-i));
//         //Delayus(5);
//         TP_DCLK(1);
//     }
// //     TP_CS(1);
//     buf&=0x0fff;
//     return(buf);
// }

