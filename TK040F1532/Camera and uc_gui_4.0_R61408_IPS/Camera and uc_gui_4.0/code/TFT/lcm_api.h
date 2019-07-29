//*************************************************
//��������������ͼ�κ���
//����ʱ�䣺2010/11/08  
//����޸�ʱ�䣺
//*************************************************
/*
  �޸���ʷ:

*/
#ifndef _LCM_API_
#define _LCM_API_

#include "stm32f10x.h"

void virtual_LCM_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor,u8 flag);
void virtual_LCM_PutString(u16 x,u16 y,u8* c,u16 charColor,u16 bkColor, u8 flag);
void virtual_LCM_PutNums(u16 x,u16 y,u32 num,u16 charColor,u16 bkColor ,u8 flag);
void virtual_LCM_PutNums_V2(u16 x,u16 y,u32 num,u8* format,u16 charColor,u16 bkColor ,u8 flag);
void virtual_LCM_PutNum_32x16(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor,u8 flag);
void virtual_LCM_PutNums_32x16(u16 x,u16 y,u32 num,u8 *format,u16 charColor,u16 bkColor ,u8 flag);

void virtual_LCM_HLine(u16 x0, u16 y0, u16 x1, u16 color);
void virtual_LCM_RLine(u16 x0, u16 y0, u16 y1, u16 color);
void virtual_LCM_Line(u16 x0, u16 y0, u16 x1, u16 y1,u16 color);  // ����

void GUI_Circle(u16 cx,u16 cy,u16 r,u16 color,u8 fill);

void virtual_LCM_Circle(u16 cx,u16 cy,u16 r,u16 color,u8 fill);  // ��Բ
void virtual_LCM_Rectangle(u16 x0, u16 y0, u16 x1, u16 y1,u16 color,u8 fill); // ������
void virtual_LCM_Square(u16 x0, u16 y0, u16 with, u16 color,u8 fill);  // ��������
void virtual_LCM_Rectangle_V2(u16 x0, u16 y0, u16 lenth, u16 width,u16 color,u8 fill); 
void virtual_LCM_picture_start(void);
  
#endif
 
/*********************end*********************/

