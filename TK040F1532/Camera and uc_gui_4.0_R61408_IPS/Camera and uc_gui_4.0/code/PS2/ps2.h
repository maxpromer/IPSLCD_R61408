#ifndef __PS2_H
#define __PS2_H	
#include "stm32f10x.h" 
// #include "SYSTICK.h"	   
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//PS2 ��������		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/6/17 
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////	 

				   
//����ӿڶ���
//PS2����	  				    
#define PS2_SCL GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)	//PA1
#define PS2_SDA GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)	//PA2
//PS2���
#define PS2_SCL_OUT_OFF GPIO_ResetBits(GPIOC, GPIO_Pin_3)	//PA1
#define PS2_SDA_OUT_OFF GPIO_ResetBits(GPIOC, GPIO_Pin_2)	//PA2

#define PS2_SCL_OUT_ON GPIO_SetBits(GPIOC, GPIO_Pin_3)	    //PA1
#define PS2_SDA_OUT_ON GPIO_SetBits(GPIOC, GPIO_Pin_2)  	//PA2
//����PS2_SCL�������״̬.		  
#define PS2_SET_SCL_IN()  {GPIOC->CRL&=0XFFFF0FFF;GPIOC->CRL|=0X00008000;}
#define PS2_SET_SCL_OUT() {GPIOC->CRL&=0XFFFF0FFF;GPIOC->CRL|=0X00003000;}	  
//����PS2_SDA�������״̬.		  
#define PS2_SET_SDA_IN()  {GPIOC->CRL&=0XFFFFF0FF;GPIOC->CRL|=0X00000800;}
#define PS2_SET_SDA_OUT() {GPIOC->CRL&=0XFFFFF0FF;GPIOC->CRL|=0X00000300;} 

#define MOUSE    0X20 //���ģʽ
#define KEYBOARD 0X10 //����ģʽ
#define CMDMODE  0X00 //��������
//PS2_Status��ǰ״̬��־
//[5:4]:��ǰ������ģʽ;[7]:���յ�һ������
//[6]:У�����;[3:0]:�յ������ݳ���;	 
extern u8 PS2_Status;       //����Ϊ����ģʽ
extern u8 PS2_DATA_BUF[16]; //ps2���ݻ�����
extern u8 MOUSE_ID;
extern u8 int_ps2_end;

void PS2_Init(void);
u8 PS2_Send_Cmd(u8 cmd);
void PS2_Set_Int(u8 en);
u8 PS2_Get_Byte(void);
void PS2_En_Data_Report(void);  
void PS2_Dis_Data_Report(void);		  				    
#endif
