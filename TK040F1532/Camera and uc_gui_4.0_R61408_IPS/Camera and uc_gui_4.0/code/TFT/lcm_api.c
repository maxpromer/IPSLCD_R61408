#include "lcm_api.h"
#include "GUI_Private.h"
#include "lcm_font.h"
#include <stdio.h>
#include "ili9325.h" 


//************************************************** 
//* ��    ��: GUI_HLine()
//* ��    ��:��ˮƽ�ߡ�
//* ��    ��: x0		ˮƽ����������е�λ��
//*           y0		ˮƽ����������е�λ��
//*           x1      ˮƽ���յ������е�λ��
//*           color	��ʾ��ɫ
//* �� �� ֵ:
//**************************************************
void  virtual_LCM_HLine(u16 x0, u16 y0, u16 x1, u16 color) 
{   
   for(;x0<=x1;x0++)
   {
	  DrawPixel(x0, y0, color);
   }
}


//************************************************
//* ��    ��:void virtual_LCM_RLine(u16 x0, u16 y0, u16 y1, u16 color) 
//* ��    ��:����ֱ�� ����Ӳ���ص㣬ʵ�ּ��١�
//* ��    ��:x0		��ֱ����������е�λ��
//*           y0		��ֱ����������е�λ��
//*           y1      ��ֱ���յ������е�λ��
//*           color	��ʾ��ɫ
//* �� �� ֵ:
//************************************************
void virtual_LCM_RLine(u16 x0, u16 y0, u16 y1, u16 color) 
{
   for(;y0<=y1;y0++)
   {
	  DrawPixel(x0, y0, color);
   }
}	 	 


/****************************************************************************
* ��    �ƣ�void ssd1289_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColoru8 flag)
* ��    �ܣ���ָ��������ʾһ��8x16�����ascii�ַ�
* ��ڲ�����x          ������
*           y          ������
*           charColor  �ַ�����ɫ
*           bkColor    �ַ�������ɫ
* ���ڲ�������
* ˵    ������ʾ��Χ�޶�Ϊ����ʾ��ascii��
* ���÷�����ssd1289_PutChar(10,10,'a',0x0000,0xffff, 1);
****************************************************************************/
//void virtual_LCM_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor,u8 flag)
//{
//  u16 i=0;
//  u16 j=0;
//  
//  u8 tmp_char=0;
//
//  for (i=0;i<16;i++)
//  {
//    tmp_char=LCM_ascii_8x16[((c-0x20)*16)+i];
//    for (j=0;j<8;j++)
//    {
//      if ( (tmp_char >> 7-j) & 0x01 == 0x01)
//        {
//          DrawPixel(x+j,y+i,charColor); // �ַ���ɫ
//        }
//        else
//        {
//		   if(flag)
//             DrawPixel(x+j,y+i,bkColor); // ������ɫ
//        }
//    }
//  }
//}


//void virtual_LCM_PutChar_V1(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor,u8 flag)
//{  
//   if(flag){
//	 Put8_16Char(x,y,LCM_ascii_8x16,c,charColor,bkColor,flag);
//   }else{
//   	 //virtual_LCM_PutChar(x,y,c,charColor,bkColor,flag);
//   }
//}


/****************************************************************************
* ��    �ƣ�void ssd1289_PutString(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
* ��    �ܣ���ָ��������ʾһ��8x16�����ascii�ַ���
* ��ڲ�����x          ������
*           y          ������
*           charColor  �ַ�����ɫ
*           bkColor    �ַ�������ɫ
*       	flag	   �Ƿ���ʾ������ɫ��1�ǣ�0��
* ���ڲ�������
* ˵    ������ʾ��Χ�޶�Ϊ����ʾ��ascii��
* ���÷�����ssd1289_PutString(10,10,��abc��,0x0000,0xffff);
****************************************************************************/
void virtual_LCM_PutString(u16 x,u16 y,u8* c,u16 charColor,u16 bkColor ,u8 flag)
{
  unsigned int i=0,j=0;	
  
  if((x>=320-8)||(y>=240-16))  return;

  i=x;
  j=y;
  
  while(*c!='\0'&&*c!=NULL)
  {
	//virtual_LCM_PutChar( i,j,*c,charColor,bkColor,flag);
	c++;
	i+=8;
	if(i>=320-8){
	   i=0;
	   j+=16;
	   if(j>=240-16)
	      return;
	}
  }
}


//*************************************************
//�������� :  ssd1289_PutNums
//�������� :  ��ʾ��ֵnum
//������� : 
//������� : 
//����ֵ   : 
//*************************************************
void virtual_LCM_PutNums(u16 x,u16 y,u32 num,u16 charColor,u16 bkColor ,u8 flag)
{
     char s[30];
     sprintf(s, "%d",num);
	 virtual_LCM_PutString(x,y,(u8*)s,charColor,bkColor ,flag);
}

void virtual_LCM_PutNums_V2(u16 x,u16 y,u32 num,u8* format,u16 charColor,u16 bkColor ,u8 flag)
{
     char s[30];
     sprintf(s,(const char *)format,num);
	 virtual_LCM_PutString(x,y,(u8*)s,charColor,bkColor ,flag);
}


//��ʾһ������ 
void virtual_LCM_PutNum_32x16(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor,u8 flag)
{  
  u16 i=0,k;
  u16 j=0;
  
  u8 tmp_char=0;

  for (i=0;i<32;i++)
  {
    for (k=0;k<2;k++)
	{
    	tmp_char=LCM_NUM_16x32[c][i*2+k];
    	for (j=0;j<8;j++)
    	{
      		if ( ((tmp_char >> 7-j) & 0x01) == 0x01)
        	{
          		DrawPixel(x+k*8+j,y+i,charColor); // �ַ���ɫ
        	}
        	else
        	{
		   		if(flag)
             	DrawPixel(x+k*8+j,y+i,bkColor); // ������ɫ
        	}
    	}
	}
  }
}


//��ʾ��ֵ
void virtual_LCM_PutNums_32x16(u16 x,u16 y,u32 num,u8 *format,u16 charColor,u16 bkColor ,u8 flag)
{
  char s[30];
  char *p;
  unsigned int i=0,j=0;	
  
  if((x>=320-16)||(y>=240-32))  return;

  sprintf(s,(const char *)format,num);

  p=s;
  i=x;
  j=y;
  
  while(*p!='\0'&&*p!=NULL)
  {
    if(flag){
       Put16x32Num(i,j,(const unsigned char *)LCM_NUM_16x32,*p-'0',charColor,bkColor,flag);
	}
	else{
       virtual_LCM_PutNum_32x16(i,j,*p-'0',charColor,bkColor,flag);
	}
	p++;
	i+=16;
	if(i>=320-16){
	   i=0;
	   j+=32;
	   if(j>=240-32)
	      return;
	}
  }
}

/****************************************************************************
* ��    �ƣ�void ili9320_GUI_Circle(u16 cx,u16 cy,u16 r,u16 color,u8 fill)
* ��    �ܣ���ָ�����껭Բ�������,���ƻ���
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
****************************************************************************/
void GUI_Circle(u16 cx,u16 cy,u16 r,u16 color,u8 fill)
{
	u16 x,y,tmp_color;
	s16 delta,tmp;
	x=0;
	y=r;
	delta=3-(r<<1);
	tmp_color=LCD_COLORINDEX;
	while(y>x)
	{
		if(fill)
		{
			LCD_COLORINDEX=color;
			LCD_L0_DrawHLine(cx-x,cy+y,cx+x);
			LCD_L0_DrawHLine(cx-x,cy-y,cx+x);
			LCD_L0_DrawHLine(cx-y,cy+x,cx+y);
			LCD_L0_DrawHLine(cx-y,cy-x,cx+y);
		}
		else
		{
			DrawPixel(cx+x,cy+y,color);
			DrawPixel(cx-x,cy+y,color);
			DrawPixel(cx+x,cy-y,color);
			DrawPixel(cx-x,cy-y,color);
			DrawPixel(cx+y,cy+x,color);
			DrawPixel(cx-y,cy+x,color);
			DrawPixel(cx+y,cy-x,color);
			DrawPixel(cx-y,cy-x,color);
		}
		x++;
		if(delta>=0)
		{
			y--;
			tmp=(x<<2);
			tmp-=(y<<2);
			delta+=(tmp+10);
		}
		else
		{
			delta+=((x<<2)+6);
		}
	}
	LCD_COLORINDEX=tmp_color;
}

/****************************************************************************
* ��    �ƣ�void GUI_Rectangle(u16 x0, u16 y0, u16 x1, u16 y1,u16 color,u8 fill)
* ��    �ܣ���ָ�����򻭾��Σ��������ɫ
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
****************************************************************************/
void virtual_LCM_Rectangle(u16 x0, u16 y0, u16 x1, u16 y1,u16 color,u8 fill)
{
	if(fill)
	{
		u16 i;
		if(x0>x1)
		{
			i=x1;
			x1=x0;
		}
		else
		{
			i=x0;
		}
		for(;i<=x1;i++)
		{
			//virtual_LCM_Line(i,y0,i,y1,color);
			virtual_LCM_RLine(i,y0,y1,color);
		}
		return;
	}
		
	virtual_LCM_HLine(x0,y0,x1,color);
	virtual_LCM_HLine(x0,y1,x1,color);
	virtual_LCM_RLine(x0,y0,y1,color);
	virtual_LCM_RLine(x1,y0,y1,color);

}

/****************************************************************************
* ��    �ƣ�void GUI_Rectangle_V2(u16 x0, u16 y0, u16 lenth, u16 width,u16 color,u8 fill)
* ��    �ܣ���ָ�����򻭾��Σ��������ɫ
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
****************************************************************************/
void virtual_LCM_Rectangle_V2(u16 x0, u16 y0, u16 lenth, u16 width,u16 color,u8 fill)
{
	if(fill){
	   Lcd_ColorBox(x0,y0,lenth,width,color);
	}else{
	   virtual_LCM_Rectangle( x0, y0, x0+lenth, y0+width,color,fill);
	}
}


/****************************************************************************
* ��    �ƣ�void  GUI_Square(u16 x0, u16 y0, u16 with, u16 color,u8 fill)
* ��    �ܣ���ָ�����������Σ��������ɫ
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
****************************************************************************/
void virtual_LCM_Square(u16 x0, u16 y0, u16 with, u16 color,u8 fill)
{
	if(fill){
	  Lcd_ColorBox(x0,y0,with,with,color);
    }else{
	  virtual_LCM_Rectangle(x0, y0, x0+with, y0+with, color,fill);
    } 
}
