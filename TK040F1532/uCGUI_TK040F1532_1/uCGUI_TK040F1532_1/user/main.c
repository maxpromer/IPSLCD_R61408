//欢迎光临我们的公司的主页：www.hjrkj.com  深圳市好钜润科技有限公司
//欢迎光临我们的官方淘宝店：http://tiky.taobao.com
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
	GUI_SetFont(&GUI_Font8_ASCII);//设置24号ASCII字体
	GUI_SetColor(GUI_RED);//设置前景色为白色
	GUI_SetBkColor(GUI_BLACK);//设置背景色为红色
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
  SystemInit();//初始化STM32
	
	Lcd_Initialize();
	SysTick_Config(SystemFrequency / 100);//10mS定时，1000为1ms
	Touch_GPIO_Config();
	GUI_Init();	//初始化GUI
//	Touch_Test();
//	GUI_SetFont(&GUI_Font24_ASCII);//设置24号ASCII字体
//	GUI_SetColor(GUI_WHITE);//设置前景色为白色
//	GUI_SetBkColor(GUI_RED);//设置背景色为红色
//  GUI_Clear();//清屏
//	GUI_DispStringAt("Wellcome to Tiky LCD world", 5, 15);//在(200, 15)的座标上写上一行字
//	GUI_Delay(200);
//	heart();while(1);
//	while(1)cube();
	
	while(1)MainTask();
	
	

}



/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
