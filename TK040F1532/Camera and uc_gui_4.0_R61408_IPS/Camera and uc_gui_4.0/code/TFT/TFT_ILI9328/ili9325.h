#ifndef __ILI9325_H
#define __ILI9325_H
#include "stm32f10x.h"
//��������IC��ILI9320����ILI9325
#define  ILI9325 

#define   BLACK                0x0000                // ��ɫ��    0,   0,   0 //
#define   BLUE                 0x001F                // ��ɫ��    0,   0, 255 //
#define   GREEN                0x07E0                // ��ɫ��    0, 255,   0 //
#define   CYAN                 0x07FF                // ��ɫ��    0, 255, 255 //
#define   RED                  0xF800                // ��ɫ��  255,   0,   0 //
#define   MAGENTA              0xF81F                // Ʒ�죺  255,   0, 255 //
#define   YELLOW               0xFFE0                // ��ɫ��  255, 255, 0   //
#define   WHITE                0xFFFF                // ��ɫ��  255, 255, 255 //
#define   NAVY                 0x000F                // ����ɫ��  0,   0, 128 //
#define   DGREEN               0x03E0                // ����ɫ��  0, 128,   0 //
#define   DCYAN                0x03EF                // ����ɫ��  0, 128, 128 //
#define   MAROON               0x7800                // ���ɫ��128,   0,   0 //
#define   PURPLE               0x780F                // ��ɫ��  128,   0, 128 //
#define   OLIVE                0x7BE0                // ����̣�128, 128,   0 //
#define   LGRAY                0xC618                // �Ұ�ɫ��192, 192, 192 //
#define   DGRAY                0x7BEF                // ���ɫ��128, 128, 128 //

#define White          0xFFFF
#define Black          0x0000
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

//��Ļ��ת���� ���ְ��� ID[1:0]AM ����PDF�е����ö���
#define ID_AM  110
//��Ļ��ʼʱ��ʾ��ʽ��ע�⣺��IDelayʱ��ʾ��һ��������������ˢ�µ�
//��ʱ�����ֶ���ˢ�½��������  LCD_WR_REG(0x0007,0x0173);������ʾ



//Ӳ����ص��Ӻ���
#define Bank1_LCD_D    ((u32)0x60020000)    //Disp Data ADDR
#define Bank1_LCD_C    ((u32)0x60000000)	   //Disp Reg ADDR


#define Lcd_Light_ON   GPIOA->BSRR = GPIO_Pin_1;
#define Lcd_Light_OFF  GPIOA->BRR  = GPIO_Pin_1;


#define Set_Cs  //GPIO_SetBits(GPIOA,GPIO_Pin_0);
#define Clr_Cs  GPIO_ResetBits(GPIOA,GPIO_Pin_0);

#define Set_Rs  GPIO_SetBits(GPIOA,GPIO_Pin_1);
#define Clr_Rs  GPIO_ResetBits(GPIOA,GPIO_Pin_1);

#define Set_nWr GPIO_SetBits(GPIOA,GPIO_Pin_2);
#define Clr_nWr GPIO_ResetBits(GPIOA,GPIO_Pin_2);

#define Set_nRd //GPIO_SetBits(GPIOA,GPIO_Pin_3);
#define Clr_nRd //GPIO_ResetBits(GPIOA,GPIO_Pin_3);

void Lcd_Start(void);
void Lcd_Configuration(void);
void DataToWrite(u16 data);
void LCD_Scan_Set(char i);

//Lcd��ʼ������ͼ����ƺ���
void Lcd_Initialize(void);
//void LCD_WR_REG(u16 Index,u16 CongfigTemp);
void Lcd_CMD_WR_Start(void);
void Lcd_WR_Start(void);
void LCD_WR_REG(u16 Index,u16 CongfigTemp);
//Lcd�߼����ƺ���

void Lcd_Clear(u16 Color);
void Lcd_ClearCharBox(u16 x,u16 y,u16 Color);
void Lcd_SetBox(u16 xStart,u16 yStart,u16 xLong,u16 yLong,u16 x_offset,u16 y_offset);
void Lcd_ColorBox(u16 x,u16 y,u16 xLong,u16 yLong,u16 Color);
void Lcd_SetCursor(u16 x,u16 y);
void DrawPixel(u16 x, u16 y, int Color);
u16 GetPoint(u16 x,u16 y);
void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend);
void LCD_Fill_Pic(u16 x, u16 y,u16 pic_H, u16 pic_V, const unsigned char* pic);
void Put8_16Char(u16 x,u16 y,const unsigned char *ascii_8x16,u8 ch,u16 charColor,u16 bkColor,u8 flag);
void Put16_16HZ(u16 x,u16 y,const unsigned char *c,u16 charColor,u16 bkColor,u8 flag);
void Put32_32HZ(u16 x,u16 y,const unsigned char *c,u16 charColor,u16 bkColor,u8 flag);
void Put16x32Num(u16 x,u16 y,const unsigned char *num_16x32,u8 ch,u16 charColor,u16 bkColor,u8 flag);

void lcd_scrol_screen(unsigned int line_num);
void lcd_scrol_disable(void);

#endif


