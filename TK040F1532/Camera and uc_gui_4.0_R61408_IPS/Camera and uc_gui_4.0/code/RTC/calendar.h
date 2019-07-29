#ifndef __CALENDAR_H
#define __CALENDAR_H
#include "stm32f10x.h"

typedef struct
{
  u8 hour;
  u8 min;
  u8 sec;

  //������������
  u16 w_year;
  u8  w_month;
  u8  w_date;
  u8  week;
}tm;

extern tm timer;//ʱ�ӽṹ�� 	  

u8 rtc_get(void);
u8 rtc_set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);

#endif 
