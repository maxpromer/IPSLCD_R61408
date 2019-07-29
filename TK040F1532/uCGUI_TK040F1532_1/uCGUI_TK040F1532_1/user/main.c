//��ӭ�������ǵĹ�˾����ҳ��www.hjrkj.com  �����к�����Ƽ����޹�˾
//��ӭ�������ǵĹٷ��Ա��꣺http://tiky.taobao.com
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "gui.h"
#include "Tiky_LCD.h" 	  
#include  "touch_RTP.h"
#include  "touch_CTP.h"
/*******************************************************************************
* Function Name  : main.
* Description    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
extern void MainTask(void);
#define  SystemFrequency  72000000
void heart(void)
{
	int y, x, a;
	int x_row=0,y_col=0;
	GUI_SetFont(&GUI_Font8_ASCII);//����24��ASCII����
	GUI_SetColor(GUI_RED);//����ǰ��ɫΪ��ɫ
	GUI_SetBkColor(GUI_BLACK);//���ñ���ɫΪ��ɫ
	for (y = 30;y > -15;y -= 1)
	{
		for (x = -30;x < 30;x += 1)
		{
			a = x*x + y*y - 20;
			if(a*a*a <= x*x*y*y*y )
			GUI_DispCharAt('*',x_row,y_col);
			x_row+=8;
		}
		y_col=y_col+8;
	}
	
}
int main(void)
{	
  SystemInit();//��ʼ��STM32
	
	Lcd_Initialize();
	SysTick_Config(SystemFrequency / 100);//10mS��ʱ��1000Ϊ1ms
	Touch_GPIO_Config();
	GUI_Init();	//��ʼ��GUI
//	Touch_Test();
//	GUI_SetFont(&GUI_Font24_ASCII);//����24��ASCII����
//	GUI_SetColor(GUI_WHITE);//����ǰ��ɫΪ��ɫ
//	GUI_SetBkColor(GUI_RED);//���ñ���ɫΪ��ɫ
//  GUI_Clear();//����
//	GUI_DispStringAt("Wellcome to Tiky LCD world", 5, 15);//��(200, 15)��������д��һ����
//	GUI_Delay(200);
//	heart();while(1);
//	while(1)cube();
	
	while(1)MainTask();
	
	

}



/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
