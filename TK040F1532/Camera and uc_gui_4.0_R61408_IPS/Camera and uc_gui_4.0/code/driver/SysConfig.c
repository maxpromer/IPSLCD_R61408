//*************************************************
//����������ϵͳ���ú���
//����ʱ�䣺2010/11/09  
//����޸�ʱ�䣺
//*************************************************
/*
  �޸���ʷ:

*/
#include "stm32f10x.h"
#include "SysConfig.h"

//�жϿ��������ȼ���������
void InterruptConfig(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);

  /* Configure the NVIC Preemption Priority Bits */
  //��ռ���ȼ� 2 λ   �����ȼ� 2 λ 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  	/* Timer2�ж�*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;		//RTCȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//��ռ���ȼ�1λ,�����ȼ�3λ
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//��ռ���ȼ�0λ,�����ȼ�4λ
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//ʹ�ܸ�ͨ���ж�
	NVIC_Init(&NVIC_InitStructure);		//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}
//
void TIM2_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 100;			//����ֵ   
	TIM_TimeBaseStructure.TIM_Prescaler = 14400-1;    	//Ԥ��Ƶ,��ֵ+1Ϊ��Ƶ�ĳ���
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  	//
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//���ϼ���	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
		/*ʹ��Ԥװ��*/
	TIM_ARRPreloadConfig(TIM2, ENABLE);

	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);

	TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);

	//TIM_Cmd(TIM2, ENABLE);
}

void Main_GPIO_Config(void)	 //GPS_EN
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;
	
		/* ����PB5Ϊ�ⲿ�ж����ţ���IR���� */
	/* EXTI line gpio config(PB1) */	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(GPIOB,&GPIO_InitStructure);	

	/* EXTI line(PB1) mode config */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1); 
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 		// �½��ز����ж�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;								//RTC
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 

}

void SysTick_Configuration(void)
{
	/* SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��SystemFrequency
	 */
	SysTick_Config(720);	 //10mS Pos
	                                         /* Load the SysTick Counter Value */
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Pos & 
                   SysTick_CTRL_TICKINT_Pos   & 
                   SysTick_CTRL_ENABLE_Pos;                    /* Enable SysTick IRQ and SysTick Timer */
}
void Sys_Enter_StopMode(void)
{		
 __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

	PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFE);

	  /*!< SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/    
  /*!< Enable HSE */    
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
	
  /*!< Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;  
  } while((HSEStatus == 0) && (StartUpCounter != HSEStartUp_TimeOut));

//   if ((RCC->CR & RCC_CR_HSERDY) != RESET)
//   {
//     HSEStatus = (uint32_t)0x01;
//   }
//   else
//   {
//     HSEStatus = (uint32_t)0x00;
//   } 
	/*!< Select HSE as system clock source */
//     RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));

    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSE;
    	
    /*!< Wait till HSE is used as system clock source */
//     while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x04)
//     {
//     }

}

extern vu32 TimingDelay;
void Delay(u32 nTime)
{
SysTick->VAL   = 0; 
SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | 
                   SysTick_CTRL_TICKINT_Msk   | 
                   SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
TimingDelay=nTime;
while(TimingDelay!=0);
SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Pos & 
                 SysTick_CTRL_TICKINT_Pos   & 
                 SysTick_CTRL_ENABLE_Pos;                    /* Enable SysTick IRQ and SysTick Timer */ 	
}
