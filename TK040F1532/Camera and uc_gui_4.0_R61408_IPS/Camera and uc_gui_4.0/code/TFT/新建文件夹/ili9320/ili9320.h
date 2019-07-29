#ifndef _ili9320_H
#define	_ili9320_H

#include "stm32f10x_lib.h"

//Ӳ����ص��Ӻ���
#define Lcd_Light_ON   GPIO_SetBits(GPIOB,GPIO_Pin_2);
#define Lcd_Light_OFF  GPIO_ResetBits(GPIOB,GPIO_Pin_2);

//#define Set_Rst GPIO_SetBits(GPIOD,GPIO_Pin_14);
//#define Clr_Rst GPIO_ResetBits(GPIOD,GPIO_Pin_14);

#define Set_Cs  GPIO_SetBits(GPIOA,GPIO_Pin_0);
#define Clr_Cs  GPIO_ResetBits(GPIOA,GPIO_Pin_0);

#define Set_Rst  GPIO_SetBits(GPIOA,GPIO_Pin_1);
#define Clr_Rst  GPIO_ResetBits(GPIOA,GPIO_Pin_1);

#define Set_Rs  GPIO_SetBits(GPIOA,GPIO_Pin_1);
#define Clr_Rs  GPIO_ResetBits(GPIOA,GPIO_Pin_1);


#define Set_nWr GPIO_SetBits(GPIOA,GPIO_Pin_2);
#define Clr_nWr GPIO_ResetBits(GPIOA,GPIO_Pin_2);

#define Set_nRd GPIO_SetBits(GPIOA,GPIO_Pin_3);
#define Clr_nRd GPIO_ResetBits(GPIOA,GPIO_Pin_3);

//Ӳ����ص��Ӻ���
/*
#define Set_Cs  GPIO_SetBits(GPIOC,GPIO_Pin_8);
#define Clr_Cs  GPIO_ResetBits(GPIOC,GPIO_Pin_8);

#define Set_Rs  GPIO_SetBits(GPIOC,GPIO_Pin_9);
#define Clr_Rs  GPIO_ResetBits(GPIOC,GPIO_Pin_9);

#define Set_nWr GPIO_SetBits(GPIOC,GPIO_Pin_10);
#define Clr_nWr GPIO_ResetBits(GPIOC,GPIO_Pin_10);

#define Set_nRd GPIO_SetBits(GPIOC,GPIO_Pin_11);
#define Clr_nRd GPIO_ResetBits(GPIOC,GPIO_Pin_11);

#define Set_Rst GPIO_SetBits(GPIOC,GPIO_Pin_12);
#define Clr_Rst GPIO_ResetBits(GPIOC,GPIO_Pin_12);

#define Lcd_Light_ON   GPIO_SetBits(GPIOC,GPIO_Pin_13);
#define Lcd_Light_OFF  GPIO_ResetBits(GPIOC,GPIO_Pin_13);

#define nCsPin  GPIO_Pin_8
#define RsPin   GPIO_Pin_9
#define nWrPin  GPIO_Pin_10
#define nRdPin  GPIO_Pin_11
#define nRstPin GPIO_Pin_12
#define Lcd_LightPin GPIO_Pin_13
*/
/*
#define Set_nWr (*((volatile unsigned long *) 0x40011010) = nWrPin)
#define Clr_nWr (*((volatile unsigned long *) 0x40011014) = nWrPin)

#define Set_Cs  (*((volatile unsigned long *) 0x40011010) = nCsPin)
#define Clr_Cs  (*((volatile unsigned long *) 0x40011014) = nCsPin)

#define Set_Rs  (*((volatile unsigned long *) 0x40011010) = RsPin)
#define Clr_Rs  (*((volatile unsigned long *) 0x40011014) = RsPin)

#define Set_nRd (*((volatile unsigned long *) 0x40011010) = nRdPin)
#define Clr_nRd (*((volatile unsigned long *) 0x40011014) = nRdPin)

#define Set_Rst (*((volatile unsigned long *) 0x40011010) = nRstPin)
#define Clr_Rst (*((volatile unsigned long *) 0x40011014) = nRstPin)

#define Lcd_Light_ON   (*((volatile unsigned long *) 0x40011010) = Lcd_LightPin)
#define Lcd_Light_OFF  (*((volatile unsigned long *) 0x40011014) = Lcd_LightPin)
*/
u16 CheckController(void);
void Lcd_Configuration(void);
void ili9320_Initializtion(void);
void ili9320_Reset(void);
void ili9320_WriteRegister(u16 index,u16 dat);
void ili9320_SetCursor(u16 x,u16 y);
void ili9320_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY);
void ili9320_DrawPicture(u16 StartX,u16 StartY,u16 EndX,u16 EndY,u16 *pic);
void ili9320_SetPoint(u16 x,u16 y,u16 point);
void ili9320_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor);
void ili9320_Clear(u16 dat);
void ili9320_Delay(u32 nCount);
void ili9320_Test(void);
u16 ili9320_GetCode(void);
void ili9320_WriteData(u16 dat);
void ili9320_WriteIndex(u16 idx);

void ili9320_BackLight(u8 status);

u16 ili9320_BGR2RGB(u16 c);

u16 ili9320_GetPoint(u16 x,u16 y);
u16 ili9320_ReadData(void);
u16 ili9320_ReadRegister(u16 index);

u16 ili9320_GUI_Color565(u32 RGB);  // RGB��ɫתΪ16λ(565)

void ili9320_GUI_Text(u16 x, u16 y, u8 *str, u16 len,u16 Color, u16 bkColor);
void ili9320_GUI_Line(u16 x0, u16 y0, u16 x1, u16 y1,u16 color);  // ����
void ili9320_GUI_Circle(u16 cx,u16 cy,u16 r,u16 color,u8 fill);  // ��԰
void ili9320_GUI_Rectangle(u16 x0, u16 y0, u16 x1, u16 y1,u16 color,u8 fill); // ������
void ili9320_GUI_Square(u16 x0, u16 y0, u16 with, u16 color,u8 fill);  // ��������
void ili9320_DrawPicture_start(void);

/*
void Touch_Initializtion(void);
u16  Touch_GetPhyX(void);
u16  Touch_GetPhyY(void);
u16  Touch_MeasurementX(void);
u16  Touch_MeasurementY(void);
*/

#endif
