#include "ssd1289.h"
//#include "lcd.h"



void ssd1289_Configuration(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	/*������Ӧʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);  
	/*����Lcd��������Ϊ�������*/
	/*16λ����*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	/*���ƽ�*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}
/*********************************************************************************
* ���ƣ�DELAY()
* ���ܣ������ʱ����������LCM��ʾ���ʱ����ơ�
* ��ڲ�������
* ���ڲ�������
**********************************************************************************/
static void delay(uint16 time)
{ 
 while(time--);
}

//ssd1289
static void Delay_nms(int n)
{
  
  u32 f=n,k;
  for (; f!=0; f--)
  {
    for(k=0xFFF; k!=0; k--);
  }
  
}

static void DelayNS(u16 time)                
{
    while(time--!=0);                                                                                                                                 
}

static void DataToWrite(u16 data) 
{
	GPIOE->ODR = data;
}
/******************************************
��������Lcdд�����
���ܣ���Lcdָ��λ��д��Ӧ�����������
��ڲ�����Index ҪѰַ�ļĴ�����ַ
          ConfigTemp д������ݻ�����ֵ
����ֵ����
******************************************/
static void LCD_WR_REG(u16 Index,u16 CongfigTemp)
{
	Clr_Cs;
	Clr_Rs;
	Set_nRd;
	DataToWrite(Index);
	Clr_nWr;
	Set_nWr;
	Set_Rs;       
	DataToWrite(CongfigTemp);       
	Clr_nWr;
	Set_nWr;
	//Set_Cs;
}

/**************************************************************************************
* ��    ��: void send_command(u16 out_data
* ��    ��: ��λ��������
* ��    ��: out_dataҪ���͵�����
* �� �� ֵ: 
**************************************************************************************/ 
static void send_command(u16 out_data)              
{
Lcd_RS_low;			//RSΪ0�������������
Lcd_WR_high;                    //д�����½�����Ч
Lcd_CS_low;                     //Ƭѡ��������Ч
Lcd_rd_high;
//GPIO_ResetBits(GPIOE, 0xffff);  //��Ҫ���ͺͽ������ݱ����Ȱ�GPIO����0
GPIO_Write(GPIOE, out_data);
Lcd_WR_low;
Lcd_CS_high;
}
/**************************************************************************************
* ��    ��: void send_data(u16 out_data)
* ��    ��: ������Ҫ��ʾ�ĵ�ַ����ɫ
* ��    ��: out_data��������λ����ɫ
* �� �� ֵ: 
**************************************************************************************/ 
static void send_data(u16 out_data)           
{
Lcd_RS_high;
Lcd_WR_high;
Lcd_CS_low;
//GPIO_ResetBits(GPIOE, 0xffff);      
//GPIO_Write(GPIOE, out_data);
GPIOE->ODR = out_data;
Lcd_WR_low;
Lcd_CS_high;
}

/**************************************************************************************
* ��    ��: rece_data(void)
* ��    ��: ������ʱGPIOE�˿ڵ�����
* ��    ��: 
* �� �� ֵ: i����������
**************************************************************************************/ 
static u16 rece_data(void)              
{

u16 i; 
Lcd_rd_high;
Lcd_RS_high;
Lcd_WR_high;
Lcd_CS_low;
//GPIO_Write(GPIOE, 0xffff);
//GPIOE->CRH = 0x44444444;
//GPIOE->CRL = 0x44444444;
//��Ϊ��������ģʽ
GPIOE->CRH = (GPIOE->CRH & 0x00000000) | 0x44444444;
GPIOE->CRL = (GPIOE->CRL & 0x00000000) | 0x44444444;
Lcd_rd_low;
//i=GPIO_ReadOutputData(GPIOE);
i = GPIOE->IDR;
//GPIOE->CRH = 0x33333333;
//GPIOE->CRL = 0x33333333;
//ͨ���������ģʽ
GPIOE->CRH = (GPIOE->CRH & 0x00000000) | 0x33333333;
GPIOE->CRL = (GPIOE->CRL & 0x00000000) | 0x33333333;		
Lcd_rd_high;
Lcd_CS_high;
return i;

}
//=================================
/**************************************************************************************
* ��    ��: void write_com(u16 address,u16 num)
* ��    ��: ������Ҫ���͵ĵ�ַ
* ��    ��: 
* �� �� ֵ: 
*
**************************************************************************************/ 
static void write_com(u16 address,u16 num)
{
    send_command(address);//��Ҫ��ʾ��λ�õ�������x����y
    send_data(num);       //��Ҫ��ʾ�ĵ�ַ
}
/**************************************************************************************
* ��    ��: 
* ��    ��:
* ��    ��:
* �� �� ֵ:
*
* �޸���ʷ:
*   �汾    ����     ����     
*   ----------------------------------------------------
*   1.0   2008.8.13  ���ݽ�       www.http://shop35330111.taobao.com
**************************************************************************************/
static void LCD_SetArea( uint16 x0, uint16 y0, uint16 x1, uint16 y1 ) 
{ /*
    write_com(0x52,x0);
    write_com(0x53,x1);
    write_com(0x50,y0);
    write_com(0x51,y1);
   */
	write_com(0x50,x0);
    write_com(0x51,x1);
    write_com(0x52,y0);
    write_com(0x53,y1);

}
/**************************************************************************************
* ��    ��: LCM_DispIni()
* ��    ��: LCM��ʾ��ʼ��
* ��    ��: 
* �� �� ֵ: ��
*
* �޸���ʷ:
*   �汾    ����     ����     
*   ----------------------------------------------------
*   1.0   2009.2.9  ���ݽ�       www.http://shop35330111.taobao.com
**************************************************************************************/ 
void ssd1289_LCM_DispIni(void)
{	
  	u16 i;
	Lcd_Light_ON;
	Clr_Rst;
	Delay_nms(1);
	Set_Rst;
	Delay_nms(1);
	Clr_Cs;
    Clr_nRd;
    Set_nRd;

    LCD_WR_REG(0x0000,0x0001);      //�򿪾���
    LCD_WR_REG(0x0003,0xA8A4);       //0xA8A4
    LCD_WR_REG(0x000C,0x0000);       
    LCD_WR_REG(0x000D,0x080C);       
    LCD_WR_REG(0x000E,0x2B00);       
    LCD_WR_REG(0x001E,0x00B0);       
    LCD_WR_REG(0x0001,0x6B3F);       //�����������320*240  0x6B3F
    LCD_WR_REG(0x0002,0x0600);    
    LCD_WR_REG(0x0010,0x0000);  
  
    LCD_WR_REG(0x0011,0x6068);            //0x4030           //�������ݸ�ʽ  16λɫ 
    //LCD_WR_REG(0x0011,0x6048|(1<<4));            //0x4030  (1<<5)|         //�������ݸ�ʽ  16λɫ
 
    LCD_WR_REG(0x0005,0x0000);    
    LCD_WR_REG(0x0006,0x0000);    
    LCD_WR_REG(0x0016,0xEF1C);    
    LCD_WR_REG(0x0017,0x0003);    
    LCD_WR_REG(0x0007,0x0233);            //0x0233       
    LCD_WR_REG(0x000B,0x0000);    
    LCD_WR_REG(0x000F,0x0000);            //ɨ�迪ʼ��ַ
    LCD_WR_REG(0x0041,0x0000);    
    LCD_WR_REG(0x0042,0x0000);    
    LCD_WR_REG(0x0048,0x0000);    
    LCD_WR_REG(0x0049,0x013F);    
    LCD_WR_REG(0x004A,0x0000);    
    LCD_WR_REG(0x004B,0x0000);    
    LCD_WR_REG(0x0044,0xEF00);    
    LCD_WR_REG(0x0045,0x0000);    
    LCD_WR_REG(0x0046,0x013F);    
    LCD_WR_REG(0x0030,0x0707);    
    LCD_WR_REG(0x0031,0x0204);    
    LCD_WR_REG(0x0032,0x0204);    
    LCD_WR_REG(0x0033,0x0502);    
    LCD_WR_REG(0x0034,0x0507);    
    LCD_WR_REG(0x0035,0x0204);    
    LCD_WR_REG(0x0036,0x0204);    
    LCD_WR_REG(0x0037,0x0502);    
    LCD_WR_REG(0x003A,0x0302);    
    LCD_WR_REG(0x003B,0x0302);    
    LCD_WR_REG(0x0023,0x0000);    
    LCD_WR_REG(0x0024,0x0000);    
    LCD_WR_REG(0x0025,0x8000);    
    LCD_WR_REG(0x004f,0);        //����ַ0
    LCD_WR_REG(0x004e,0);        //����ַ0

}
/**************************************************************************************
* ��    ��: GUI_FillSCR()
* ��    ��:ȫ����䡣ֱ��ʹ�����������ʾ������
* ��    ��:dat		��������
* �� �� ֵ:
*
* �޸���ʷ:
*   �汾    ����     ����     
*   ----------------------------------------------------
*   1.0   2008.8.13  ���ݽ�       www.http://shop35330111.taobao.com
**************************************************************************************/

void  ssd1289_GUI_FillSCR(uint16 dat)
{  
    uint16 i,j;
    LCD_SetArea(0,0,GUI_LCM_XMAX-1,GUI_LCM_YMAX-1); 
    send_command(0x22);
    for(j=0;j<GUI_LCM_XMAX;j++)
        for(i=0;i<GUI_LCM_YMAX;i++)
            send_data(dat);
}
/*
void  GUI_Fill(uint16 x0,uint16 y0 ,uint16 x1,uint16 y1 ,uint16 clor)
{  
    uint16 i,j;
    LCD_SetArea(x0,y0,x1-1,y1-1); 
    send_command(0x22);
    for(j=x0;j<x1;j++)
        for(i=y0;i<y1;i++)
            send_data(clor);
}
*/
/**************************************************************************************
* ��    ��: Set_ramaddr()
* ��    ��:������ʾram��ַ
* ��    ��:
* �� �� ֵ:
*
* �޸���ʷ:
*   �汾    ����     ����     
*   ----------------------------------------------------
*   1.0   2008.8.13  ���ݽ�       www.http://shop35330111.taobao.com
**************************************************************************************/
void ssd1289_Set_ramaddr(uint16 x,uint16 y)
{  
    write_com(0x004f,x);        //����ַ
	write_com(0x004e,y);        //����ַ
}
/****************************************************************************
* ��    �ƣ�uint16 BGR_RGB(uint16 c)
* ��    �ܣ�RRRRRGGGGGGBBBBB ��Ϊ BBBBBGGGGGGRRRRR ��ʽ
* ��ڲ�����c      BRG ��ɫֵ
* ���ڲ�����RGB ��ɫֵ
* ˵    �����ڲ���������
* ���÷�����
****************************************************************************/
static uint16 BGR_RGB(uint16 c)
{
  uint16  r, g, b;

  b = (c>>0)  & 0x1f;
  g = (c>>5)  & 0x3f;
  r = (c>>11) & 0x1f;
  
  return( (b<<11) + (g<<5) + (r<<0) );
}
/****************************************************************************
* ��    �ƣ�uint16 GetID()
* ��    �ܣ���ȡID
* ��ڲ�����
* ���ڲ�������ǰID
* ˵    ����
* ���÷�����i=GetID();
****************************************************************************/
static uint16 GetID(void)
{
    send_command(0x00);
    return rece_data();
}
/**************************************************************************************
* ��    ��:GUI_Initialize()
* ��    ��:��ʼ��GUI��������ʼ����ʾ����������ʼ��LCM��������
* ��    ��:
* �� �� ֵ:
*
* �޸���ʷ:
*   �汾    ����     ����     
*   ----------------------------------------------------
*   1.0   2008.8.13  ���ݽ�       www.http://shop35330111.taobao.com
**************************************************************************************/

void  ssd1289_GUI_Initialize(void)
{  					// ��ʼ��LCMģ�鹤��ģʽ����ͼ��ģʽ
	ssd1289_LCM_DispIni();	
//	send_command(0xAD); /* Set DC-DC */
//	send_command(0x8B); /* 8B=ON, 8A=Off */
//	send_command(0xaf);
//    GUI_FillSCR(0x00);				// ��ʼ��������Ϊ0x00���������Ļ(����)
}


static uint16  DEC_HEX_TAB[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
/**************************************************************************************
* ��    ��: GUI_Point()
* ��    ��:��ָ��λ���ϻ��㡣
* ��    ��:x		ָ���������е�λ��
*          y		ָ���������е�λ��
*          color	��ʾ��ɫ(���ںڰ�ɫLCM��Ϊ0ʱ��Ϊ1ʱ��ʾ)
* �� �� ֵ:����ֵΪ1ʱ��ʾ�����ɹ���Ϊ0ʱ��ʾ����ʧ�ܡ�
*
* �޸���ʷ:
*   �汾    ����     ����     
*   ----------------------------------------------------
*   1.0   2008.8.13  ���ݽ�       www.http://shop35330111.taobao.com


**************************************************************************************/
uint8  ssd1289_GUI_Point(uint16 x, uint16 y, TCOLOR color)
{
   // �������� 
   if(x>=GUI_LCM_XMAX) return(0);
   if(y>=GUI_LCM_YMAX) return(0);
   
    ssd1289_Set_ramaddr(x,y);        //
    send_command(0x22);
    send_data(color);
   return(1);
}

/**************************************************************************************
* ��    ��: GetPoint(uint16 x,uint16 y)
* ��    ��:��ȡָ���������ɫֵ
* ��    ��:x      ������
*          y      ������
* �� �� ֵ:��ǰ������ɫֵ
*
* �޸���ʷ:
*   �汾    ����     ����     
*   ----------------------------------------------------
*   1.0   2008.8.13  ���ݽ�       www.http://shop35330111.taobao.com
**************************************************************************************/

uint16 ssd1289_GetPoint(uint16 x,uint16 y)
{
  	ssd1289_Set_ramaddr(x,y);
	send_command(0x22);
    rece_data();
	//return BGR_RGB(rece_data());
	return	rece_data();
}
/**************************************************************************************
* ��    ��:GUI_ReadPoint()
* ��    ��:��ȡָ�������ɫ?
* ��    ��: x		ָ���������е�λ��
*           y		ָ���������е�λ��
*           ret     ������ɫֵ��ָ��
* �� �� ֵ:����0��ʾָ����ַ������������Χ
*
* �޸���ʷ:
*   �汾    ����     ����     
*   ----------------------------------------------------
*   1.0   2008.8.13  ���ݽ�       www.http://shop35330111.taobao.com


**************************************************************************************/
uint8 ssd1289_GUI_ReadPoint(uint16 x, uint16 y, TCOLOR *ret)
{  

   // ��������
   if(x>=GUI_LCM_XMAX) return(0);
   if(y>=GUI_LCM_YMAX) return(0);
  
   *ret = ssd1289_GetPoint(x,y);
  
   return(1);
}
/**************************************************************************************
* ��    ��: GUI_HLine()
* ��    ��:��ˮƽ�ߡ�
* ��    ��: x0		ˮƽ����������е�λ��
*           y0		ˮƽ����������е�λ��
*           x1      ˮƽ���յ������е�λ��
*           color	��ʾ��ɫ
* �� �� ֵ:
*
* �޸���ʷ:
*   �汾    ����     ����     
*   ----------------------------------------------------
*   1.0   2008.8.13  ���ݽ�       www.http://shop35330111.taobao.com
**************************************************************************************/

void  ssd1289_GUI_HLine(uint16 x0, uint16 y0, uint16 x1, TCOLOR color) 
{  uint16  i;

  if(x0>x1) 						// ��x0��x1��С�������У��Ա㻭ͼ
   {  i = x1;
      x1 = x0;
      x0 = i;
   }
 
/*	for(i=0;i<x1-x0+1;i++) 
		GUI_Point(x0+i,y0,color);   


	LCD_SetArea(x0,x1,y0,y0);
	send_command(0x22); 
	for(i=0;i<x1-x0+1;i++) 
		send_data(color);		  
									   */  
	ssd1289_GUI_Point(x0,y0,color); 
	for(i=0;i<x1-x0;i++)
    	send_data(color);	
 
}
/**************************************************************************************
* ��    ��:GUI_RLine()
* ��    ��:����ֱ�� ����Ӳ���ص㣬ʵ�ּ��١�
* ��    ��:x0		��ֱ����������е�λ��
*           y0		��ֱ����������е�λ��
*           y1      ��ֱ���յ������е�λ��
*           color	��ʾ��ɫ
* �� �� ֵ:
*
* �޸���ʷ:
*   �汾    ����     ����     
*   ----------------------------------------------------
*   1.0   2008.8.13  ���ݽ�       www.http://shop35330111.taobao.com
**************************************************************************************/

void  ssd1289_GUI_RLine(uint16 x0, uint16 y0, uint16 y1, TCOLOR color) 
{  uint16  i;
 
   if(y0>y1) 		// ��y0��y1��С�������У��Ա㻭ͼ
   {  i = y1;
      y1 = y0;
      y0 = i;
   }
  	for(i=0;i<y1-y0+1;i++) 
		ssd1289_GUI_Point(x0,y0+i,color); 
/* 	LCD_SetArea(x0,x0,y0,y1);
	send_command(0x22); 
	for(i=0;i<y1-y0+1;i++) 
		send_data(color);		*/

}	 	 


void SSD1289_picture_start(void)
{
    ssd1289_Set_ramaddr(0,0);
    ssd1289_Set_ramaddr(0,0);
	LCD_WR_REG(0x0050,0x00);//ˮƽ GRAM��ʼλ��
	LCD_WR_REG(0x0051,319);//ˮƽGRAM��ֹλ��
	LCD_WR_REG(0x0052,0x00);//��ֱGRAM��ʼλ��
	LCD_WR_REG(0x0053,239);//��ֱGRAM��ֹλ��    	
	send_command(0x22);
    Clr_Cs;
}

//====================================================================================
