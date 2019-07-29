//��ӭ�������ǵĹ�˾����ҳ��www.hjrkj.com  �����к�����Ƽ����޹�˾
//��ӭ�������ǵĹٷ��Ա��꣺http://tiky.taobao.com
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "gui.h"
#include "Tiky_LCD.h" 	  
#include  "touch.h"
/*******************************************************************************
* Function Name  : main.
* Description    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
extern void MainTask(void);
#define  SystemFrequency  72000000
int main(void)
{	
  SystemInit();//��ʼ��STM32
	Lcd_Initialize();
	SysTick_Config(SystemFrequency / 100);//10mS��ʱ��1000Ϊ1ms
	Touch_GPIO_Config();
	GUI_Init();	//��ʼ��GUI
	
	GUI_SetFont(&GUI_Font24_ASCII);//����24��ASCII����
	GUI_SetColor(GUI_WHITE);//����ǰ��ɫΪ��ɫ
	GUI_SetBkColor(GUI_RED);//���ñ���ɫΪ��ɫ
  GUI_Clear();//����
	GUI_DispStringAt("Wellcome to Tiky LCD world", 5, 15);//��(200, 15)��������д��һ����
	GUI_Delay(200);
//	GUI_ClearRect(1,1, 5,5);
	while(1)MainTask();
	
}



/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
