
#include "delay.h"

void OV7670_delay_us(unsigned int i)
 {     
    while( i--);
 }		  
 
//    ���뼶��ʱ����	 
void OV7670_delay_ms(unsigned int time)
	 {
	  while(time--)
	  	  {	
		   OV7670_delay_us(1000);
		  }
	 }

