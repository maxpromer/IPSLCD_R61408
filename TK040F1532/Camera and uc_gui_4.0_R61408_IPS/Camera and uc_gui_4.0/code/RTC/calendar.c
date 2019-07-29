#include "calendar.h"
#include "stm32f10x.h"

//��ֲ���ѵĴ���
//��ǿ��������֧��ũ����24�����������������Ĺ���
//����ʱ����1970��ΪԪ�꣬��32bit��ʱ��Ĵ����������е�2100������						   	    					  
//����ԭ��@SCUT
//V1.0

//��ݱ�
unsigned char const year_code[597]=
{
0x04,0xAe,0x53, //1901 0
0x0A,0x57,0x48, //1902 3
0x55,0x26,0xBd, //1903 6
0x0d,0x26,0x50, //1904 9
0x0d,0x95,0x44, //1905 12
0x46,0xAA,0xB9, //1906 15
0x05,0x6A,0x4d, //1907 18
0x09,0xAd,0x42, //1908 21
0x24,0xAe,0xB6, //1909
0x04,0xAe,0x4A, //1910
0x6A,0x4d,0xBe, //1911
0x0A,0x4d,0x52, //1912
0x0d,0x25,0x46, //1913
0x5d,0x52,0xBA, //1914
0x0B,0x54,0x4e, //1915
0x0d,0x6A,0x43, //1916
0x29,0x6d,0x37, //1917
0x09,0x5B,0x4B, //1918
0x74,0x9B,0xC1, //1919
0x04,0x97,0x54, //1920
0x0A,0x4B,0x48, //1921
0x5B,0x25,0xBC, //1922
0x06,0xA5,0x50, //1923
0x06,0xd4,0x45, //1924
0x4A,0xdA,0xB8, //1925
0x02,0xB6,0x4d, //1926
0x09,0x57,0x42, //1927
0x24,0x97,0xB7, //1928
0x04,0x97,0x4A, //1929
0x66,0x4B,0x3e, //1930
0x0d,0x4A,0x51, //1931
0x0e,0xA5,0x46, //1932
0x56,0xd4,0xBA, //1933
0x05,0xAd,0x4e, //1934
0x02,0xB6,0x44, //1935
0x39,0x37,0x38, //1936
0x09,0x2e,0x4B, //1937
0x7C,0x96,0xBf, //1938
0x0C,0x95,0x53, //1939
0x0d,0x4A,0x48, //1940
0x6d,0xA5,0x3B, //1941
0x0B,0x55,0x4f, //1942
0x05,0x6A,0x45, //1943
0x4A,0xAd,0xB9, //1944
0x02,0x5d,0x4d, //1945
0x09,0x2d,0x42, //1946
0x2C,0x95,0xB6, //1947
0x0A,0x95,0x4A, //1948
0x7B,0x4A,0xBd, //1949
0x06,0xCA,0x51, //1950
0x0B,0x55,0x46, //1951
0x55,0x5A,0xBB, //1952
0x04,0xdA,0x4e, //1953
0x0A,0x5B,0x43, //1954
0x35,0x2B,0xB8, //1955
0x05,0x2B,0x4C, //1956
0x8A,0x95,0x3f, //1957
0x0e,0x95,0x52, //1958
0x06,0xAA,0x48, //1959
0x7A,0xd5,0x3C, //1960
0x0A,0xB5,0x4f, //1961
0x04,0xB6,0x45, //1962
0x4A,0x57,0x39, //1963
0x0A,0x57,0x4d, //1964
0x05,0x26,0x42, //1965
0x3e,0x93,0x35, //1966
0x0d,0x95,0x49, //1967
0x75,0xAA,0xBe, //1968
0x05,0x6A,0x51, //1969
0x09,0x6d,0x46, //1970
0x54,0xAe,0xBB, //1971
0x04,0xAd,0x4f, //1972
0x0A,0x4d,0x43, //1973
0x4d,0x26,0xB7, //1974
0x0d,0x25,0x4B, //1975
0x8d,0x52,0xBf, //1976
0x0B,0x54,0x52, //1977
0x0B,0x6A,0x47, //1978
0x69,0x6d,0x3C, //1979
0x09,0x5B,0x50, //1980
0x04,0x9B,0x45, //1981
0x4A,0x4B,0xB9, //1982
0x0A,0x4B,0x4d, //1983
0xAB,0x25,0xC2, //1984
0x06,0xA5,0x54, //1985
0x06,0xd4,0x49, //1986
0x6A,0xdA,0x3d, //1987
0x0A,0xB6,0x51, //1988
0x09,0x37,0x46, //1989
0x54,0x97,0xBB, //1990
0x04,0x97,0x4f, //1991
0x06,0x4B,0x44, //1992
0x36,0xA5,0x37, //1993
0x0e,0xA5,0x4A, //1994
0x86,0xB2,0xBf, //1995
0x05,0xAC,0x53, //1996
0x0A,0xB6,0x47, //1997
0x59,0x36,0xBC, //1998
0x09,0x2e,0x50, //1999 294
0x0C,0x96,0x45, //2000 297
0x4d,0x4A,0xB8, //2001 300
0x0d,0x4A,0x4C, //2002
0x0d,0xA5,0x41, //2003
0x25,0xAA,0xB6, //2004
0x05,0x6A,0x49, //2005
0x7A,0xAd,0xBd, //2006
0x02,0x5d,0x52, //2007
0x09,0x2d,0x47, //2008
0x5C,0x95,0xBA, //2009
0x0A,0x95,0x4e, //2010
0x0B,0x4A,0x43, //2011
0x4B,0x55,0x37, //2012
0x0A,0xd5,0x4A, //2013
0x95,0x5A,0xBf, //2014
0x04,0xBA,0x53, //2015
0x0A,0x5B,0x48, //2016
0x65,0x2B,0xBC, //2017
0x05,0x2B,0x50, //2018
0x0A,0x93,0x45, //2019
0x47,0x4A,0xB9, //2020
0x06,0xAA,0x4C, //2021
0x0A,0xd5,0x41, //2022
0x24,0xdA,0xB6, //2023
0x04,0xB6,0x4A, //2024
0x69,0x57,0x3d, //2025
0x0A,0x4e,0x51, //2026
0x0d,0x26,0x46, //2027
0x5e,0x93,0x3A, //2028
0x0d,0x53,0x4d, //2029
0x05,0xAA,0x43, //2030
0x36,0xB5,0x37, //2031
0x09,0x6d,0x4B, //2032
0xB4,0xAe,0xBf, //2033
0x04,0xAd,0x53, //2034
0x0A,0x4d,0x48, //2035
0x6d,0x25,0xBC, //2036
0x0d,0x25,0x4f, //2037
0x0d,0x52,0x44, //2038
0x5d,0xAA,0x38, //2039
0x0B,0x5A,0x4C, //2040
0x05,0x6d,0x41, //2041
0x24,0xAd,0xB6, //2042
0x04,0x9B,0x4A, //2043
0x7A,0x4B,0xBe, //2044
0x0A,0x4B,0x51, //2045
0x0A,0xA5,0x46, //2046
0x5B,0x52,0xBA, //2047
0x06,0xd2,0x4e, //2048
0x0A,0xdA,0x42, //2049
0x35,0x5B,0x37, //2050
0x09,0x37,0x4B, //2051
0x84,0x97,0xC1, //2052
0x04,0x97,0x53, //2053
0x06,0x4B,0x48, //2054
0x66,0xA5,0x3C, //2055
0x0e,0xA5,0x4f, //2056
0x06,0xB2,0x44, //2057
0x4A,0xB6,0x38, //2058
0x0A,0xAe,0x4C, //2059
0x09,0x2e,0x42, //2060
0x3C,0x97,0x35, //2061
0x0C,0x96,0x49, //2062
0x7d,0x4A,0xBd, //2063
0x0d,0x4A,0x51, //2064
0x0d,0xA5,0x45, //2065
0x55,0xAA,0xBA, //2066
0x05,0x6A,0x4e, //2067
0x0A,0x6d,0x43, //2068
0x45,0x2e,0xB7, //2069
0x05,0x2d,0x4B, //2070
0x8A,0x95,0xBf, //2071
0x0A,0x95,0x53, //2072
0x0B,0x4A,0x47, //2073
0x6B,0x55,0x3B, //2074
0x0A,0xd5,0x4f, //2075
0x05,0x5A,0x45, //2076
0x4A,0x5d,0x38, //2077
0x0A,0x5B,0x4C, //2078
0x05,0x2B,0x42, //2079
0x3A,0x93,0xB6, //2080
0x06,0x93,0x49, //2081
0x77,0x29,0xBd, //2082
0x06,0xAA,0x51, //2083
0x0A,0xd5,0x46, //2084
0x54,0xdA,0xBA, //2085
0x04,0xB6,0x4e, //2086
0x0A,0x57,0x43, //2087
0x45,0x27,0x38, //2088
0x0d,0x26,0x4A, //2089
0x8e,0x93,0x3e, //2090
0x0d,0x52,0x52, //2091
0x0d,0xAA,0x47, //2092
0x66,0xB5,0x3B, //2093
0x05,0x6d,0x4f, //2094
0x04,0xAe,0x45, //2095
0x4A,0x4e,0xB9, //2096
0x0A,0x4d,0x4C, //2097
0x0d,0x15,0x41, //2098
0x2d,0x92,0xB5  //2099
};
///////////////////////////////////////////////////////////////////////////////////////////////////////// 
//         ����Ϊ24����������س���			   
// 
//    ÿ��24������־��   
//    ����Ȥ�����ѿɰ����������ԭ�����������ݵı��
//    ���Ǻ���������ѿɸ��ҷ�EMAIL		   
/////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char const YearMonthBit[]=
{
	0x4E,0xA6,0x99,		//2000
	0x9C,0xA2,0x98,		//2001
	0x80,0x00,0x18,		//2002
	0x00,0x10,0x24,		//2003
	0x4E,0xA6,0x99,		//2004
	0x9C,0xA2,0x98,		//2005
	0x80,0x82,0x18,		//2006
	0x00,0x10,0x24,		//2007
	0x4E,0xA6,0xD9,		//2008
	0x9E,0xA2,0x98,		//2009

	0x80,0x82,0x18,		//2010
	0x00,0x10,0x04,		//2011
	0x4E,0xE6,0xD9,		//2012
	0x9E,0xA6,0xA8,		//2013
	0x80,0x82,0x18,		//2014
	0x00,0x10,0x00,		//2015
	0x0F,0xE6,0xD9,		//2016
	0xBE,0xA6,0x98,		//2017
	0x88,0x82,0x18,		//2018
	0x80,0x00,0x00,		//2019

	0x0F,0xEF,0xD9,		//2020
	0xBE,0xA6,0x99,		//2021
	0x8C,0x82,0x98,		//2022
	0x80,0x00,0x00,		//2023
	0x0F,0xEF,0xDB,		//2024
	0xBE,0xA6,0x99,		//2025
	0x9C,0xA2,0x98,		//2026
	0x80,0x00,0x18,		//2027
	0x0F,0xEF,0xDB,		//2028
	0xBE,0xA6,0x99,		//2029

	0x9C,0xA2,0x98,		//2030
	0x80,0x00,0x18,		//2031
	0x0F,0xEF,0xDB,		//2032
	0xBE,0xA2,0x99,		//2033
	0x8C,0xA0,0x98,		//2034
	0x80,0x82,0x18,		//2035
	0x0B,0xEF,0xDB,		//2036
	0xBE,0xA6,0x99,		//2037
	0x8C,0xA2,0x98,		//2038
	0x80,0x82,0x18,		//2039

	0x0F,0xEF,0xDB,		//2040
	0xBE,0xE6,0xD9,		//2041 
	0x9E,0xA2,0x98,		//2042
	0x80,0x82,0x18,		//2043
	0x0F,0xEF,0xFB,		//2044
	0xBF,0xE6,0xD9,		//2045
	0x9E,0xA6,0x98,		//2046
	0x80,0x82,0x18,		//2047
	0x0F,0xFF,0xFF,		//2048
	0xFC,0xEF,0xD9,		//2049
	0xBE,0xA6,0x18 		//2050
};
const unsigned char days[24]=
{
	6,20,4,19,6,21,         //һ�µ�����  �Ľ�����������
	5,20,6,21,6,21,         //���µ�����  �Ľ�����������
	7,23,8,23,8,23,         //���µ�����  �Ľ�����������
	8,24,8,22,7,22,         //ʮ�µ�ʮ���µĽ�����������
};
//�Թ��������Ⱥ�����
const char *JieQiStr[24]=   
{
 // ����        �Ƕ�    ��������     ���� //
	"С��",     //285     1�� 6��
	"��",     //300     1��20��    29.5��
	"����",     //315     2�� 4��
	"��ˮ",     //330     2��19��    29.8��
	"����",     //345     3�� 6��
	"����",     //  0     3��21��    30.2��
	"����",     // 15     4�� 5��
	"����",     // 30     4��20��    30.7��
	"����",     // 45     5�� 6��
	"����",     // 60     5��21��    31.2��
	"â��",     // 75     6�� 6��
	"����",     // 90     6��21��    31.4��
	"С��",     //105     7�� 7��
	"����",     //120     7��23��    31.4��
	"����",     //135     8�� 8��
	"����",     //150     8��23��    31.1��
	"��¶",     //165     9�� 8��
	"���",     //180     9��23��    30.7��
	"��¶",     //195    10�� 8��
	"˪��",     //210    10��24��    30.1��
	"����",     //225    11�� 8��
	"Сѩ",     //240    11��22��    29.7��
	"��ѩ",     //255    12�� 7��
	"����"      //270    12��22��    29.5��
};

//�²���������ũ������Ҫʹ�õ�
//�·����ݱ�
u8  const day_code1[9]={0x0,0x1f,0x3b,0x5a,0x78,0x97,0xb5,0xd4,0xf3};
unsigned short const day_code2[3]={0x111,0x130,0x14e};
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //���������ݱ�
u8 const *sky[10]=  {"��","��","��","��","��","��","��","��","��","��",};//���
u8 const *earth[12]={"��","��","��","î","��","��","��","δ","��","��","��","��",};//��֧
u8 const *monthcode[12]={"һ","��","��","��","��","��","��","��","��","ʮ","��","��",};//ũ���·�
u8 const *nongliday[4]={"��","ʮ","إ","��",};//ũ������  

tm timer;//ʱ�ӽṹ�� 	  
//ʵʱʱ������
//BKP->DR1 bit3 ʱ���Ƿ�Ҫ����?
void rtc_init(void)
{	
	//����ǲ��ǵ�һ������ʱ��
	u8 temp=0;
	//temp=FM24C16_ReadOneByte(52);
	if(temp&0X02)//ϵͳ������ʱ
	{	
// 		printf("TIME Init OK!\n");							    					  
		//NVIC_RTCConfiguration();//RTC�ж�ʹ��   
    	//while(!(RTC->CRL&(1<<3)));//�ȴ�RTC�Ĵ���ͬ��  
    	//RTC->CRH|=0X01;  		  //�������ж�
    	//while(!(RTC->CRL&(1<<5)));//�ȴ�RTC�Ĵ����������	 
	}else //��һ������(δ�������ж�!!)	
	{	
// 		printf("TIME need INIT!\n");								  
		//NVIC_RTCConfiguration(); //RTC�ж�ʹ��	 
	  	RCC->APB1ENR|=1<<28;     //ʹ�ܵ�Դʱ��	    
		RCC->APB1ENR|=1<<27;     //ʹ�ܱ���ʱ��
			
		PWR->CR|=1<<8;           //ȡ��������д����
		RCC->BDCR|=1<<16;        //����������λ	   
		RCC->BDCR&=~(1<<16);     //����������λ
				 
	    RCC->BDCR|=1<<0;         //�����ⲿ�������� 
	    while(!(RCC->BDCR&0X02));//�ȴ��ⲿʱ�Ӿ���	 
		RCC->BDCR|=1<<8; //LSI��ΪRTCʱ�� 	    
		RCC->BDCR|=1<<15;//RTCʱ��ʹ��	    

		RTC->PRLH=0X0000;
		RTC->PRLL=32770;          //ʱ����������(�д��۲�,���Ƿ�������?)����ֵ��32767										 
	  	while(!(RTC->CRL&(1<<5)));//�ȴ�RTC�Ĵ����������
											   
		RTC->CRL|=1<<4;   //��������	  
		rtc_set(2009,4,19,23,59,55);//����ʱ��	  
		RTC->CRL&=~(1<<4);//���ø���
		while(!(RTC->CRL&(1<<5)));//�ȴ�RTC�Ĵ����������	
		temp|=0x02;//���ʱ�ӱ��޸���
	 	//FM24C16_WriteOneByte(52,temp);  
	}
	rtc_get();//����ʱ�� 
}
/*
//RTCʱ���ж�
//ÿ�봥��һ��   
void RTC_IRQHandler(void)
{
	u16 RTCCRL;	  
	RTCCRL=RTC->CRL;//��ȡFLAG
	if(RTCCRL&0x0001)//�����ж�
	{
		rtc_get();//����ʱ�� 
		printf("sec ok\n");	
	}
	if(RTCCRL&0x0002)//�����ж�
	{
		RTC->CRL&=~(0x0002);//�������ж�
		//Alarm_Process();
		//���Ӵ���
	} 				  								 
    RTC->CRL&=0X0FFA;         //�������������жϱ�־
	while(!(RTC->CRL&(1<<5)));//�ȴ�RTC�Ĵ����������		   							 	   	 
}
*/

//�·�   1  2  3  4  5  6  7  8  9  10 11 12
//����   31 29 31 30 31 30 31 31 30 31 30 31
//������ 31 28 31 30 31 30 31 31 30 31 30 31
//����:���
//���:������ǲ�������.1,��.0,����
u8 Is_Leap_Year(u16 year)
{			  
	if(year%4==0) //�����ܱ�4����
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;//�����00��β,��Ҫ�ܱ�400���� 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}

//����ʱ��
//�������ʱ��ת��Ϊ����
//��1970��1��1��Ϊ��׼
//1970~2099��Ϊ�Ϸ����

//ƽ����·����ڱ�
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
u8 rtc_set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 seccount=0;
	if(syear<1970||syear>2099)return 0;	   
	for(t=1970;t<syear;t++)	//��������ݵ��������
	{
		if(Is_Leap_Year(t))seccount+=31622400;//�����������
		else seccount+=31536000;			  //ƽ���������
	}
	smon-=1;
	for(t=0;t<smon;t++)	   //��ǰ���·ݵ����������
	{
		seccount+=(u32)mon_table[t]*86400;//�·����������
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//����2�·�����һ���������	   
	}
	seccount+=(u32)(sday-1)*86400;//��ǰ�����ڵ���������� 
	seccount+=(u32)hour*3600;//Сʱ������
    seccount+=(u32)min*60;	 //����������
	seccount+=sec;//�������Ӽ���ȥ
													    
	//����ʱ��
    RCC->APB1ENR|=1<<28;//ʹ�ܵ�Դʱ��
    RCC->APB1ENR|=1<<27;//ʹ�ܱ���ʱ��
	PWR->CR|=1<<8;    //ȡ��������д����
	//���������Ǳ����!
	RTC->CRL|=1<<4;   //�������� 
	RTC->CNTL=seccount&0xffff;
	RTC->CNTH=seccount>>16;
	RTC->CRL&=~(1<<4);//���ø���
	while(!(RTC->CRL&(1<<5)));//�ȴ�RTC�Ĵ����������  	
	return 1;	    
}
//�ӵ�ǰ����ֵ,�õ�ʱ��.
//����ֵ:0,ʧ��;1,�ɹ�
u8 rtc_get(void)
{
	u32 timecount=0; 
	u32 temp=0;
	u16 temp1=0;   
	timecount=RTC->CNTH;//�õ��������е�ֵ(������)
	timecount<<=16;
	timecount+=RTC->CNTL;			 

	temp=timecount/86400;   //�õ�����(��������Ӧ��)
	temp1=1970;	//��1970�꿪ʼ
	while(temp>=365)
	{				 
		if(Is_Leap_Year(temp1))//������
		{
			if(temp>=366)temp-=366;//�����������
			else {temp1++;break;}  
		}
		else temp-=365;	  //ƽ�� 
		temp1++;  
	}   
	timer.w_year=temp1;//�õ����
	temp1=0;
	while(temp>=28)//������һ����
	{
		if(Is_Leap_Year(timer.w_year)&&temp1==1)//�����ǲ�������/2�·�
		{
			if(temp>=29)temp-=29;//�����������
			else break; 
		}
		else 
		{
			if(temp>=mon_table[temp1])temp-=mon_table[temp1];//ƽ��
			else break;
		}
		temp1++;  
	}
	timer.w_month=temp1+1;//�õ��·�
	timer.w_date=temp+1;  //�õ�����		 

	temp=timecount%86400;     //�õ�������   	   
	timer.hour=temp/3600;     //Сʱ
	timer.min=(temp%3600)/60; //����	
	timer.sec=(temp%3600)%60; //����
	return 1;
}



///////////////////////////////////////////////////////////////////////
//֧�ִ�1900�굽2099���ũ����ѯ
//֧�ִ�2000�굽2050��Ľ�����ѯ
//�Ӻ���,���ڶ�ȡ���ݱ���ũ���µĴ��»�С��,�������Ϊ�󷵻�1,ΪС����0
u8 GetMoonDay(u8 month_p,unsigned short table_addr)
{
	switch (month_p)
	{
		case 1:
			if((year_code[table_addr]&0x08)==0)	return(0);
			else 								return(1); 
		case 2:
			if((year_code[table_addr]&0x04)==0)	return(0);
			else 								return(1);
		case 3:
			if((year_code[table_addr]&0x02)==0)	return(0);
			else 								return(1);
		case 4:
			if((year_code[table_addr]&0x01)==0)	return(0);
			else 								return(1);
		case 5:
			if((year_code[table_addr+1]&0x80)==0)	return(0);
			else 									return(1);
		case 6:
			if((year_code[table_addr+1]&0x40)==0)	return(0);
			else 									return(1);
		case 7:
			if((year_code[table_addr+1]&0x20)==0)	return(0);
			else 									return(1);
		case 8:
			if((year_code[table_addr+1]&0x10)==0)	return(0);
			else 									return(1);
		case 9:
			if((year_code[table_addr+1]&0x08)==0)	return(0);
			else 									return(1);
		case 10:
			if((year_code[table_addr+1]&0x04)==0)	return(0);
			else 									return(1);
		case 11:
			if((year_code[table_addr+1]&0x02)==0)	return(0);
			else 									return(1);
		case 12:
			if((year_code[table_addr+1]&0x01)==0)	return(0);
			else 									return(1);
		case 13:
			if((year_code[table_addr+2]&0x80)==0)	return(0);
			else 									return(1);
	}
	return(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////// 
// ��������:GetChinaCalendar
//��������:��ũ��ת��(ֻ����1901-2099��)
// �䡡��:  year        ������
//          month       ������
//          day         ������
//          p           ����ũ�����ڵ�ַ
// �䡡��:  1           �ɹ�
//          0           ʧ��																			 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
u8 GetChinaCalendar(u16  year,u8 month,u8 day,u8 *p)
{ 
	u8 temp1,temp2,temp3,month_p,yearH,yearL;	
	u8 flag_y;
	unsigned short temp4,table_addr;

	yearH=year/100;	yearL=year%100;//��ݵĸߵ������ֽ� 
	if((yearH!=19)&&(yearH!=20))return(0);//���ڲ���19xx ~ 20xx ��Χ��,���˳�
	
	// ��λ���ݱ��ַ  
	if(yearH==20)	table_addr=(yearL+100-1)*3;
	else  			table_addr=(yearL-1)*3;

	// ȡ���괺�����ڵĹ����·�  
	temp1=year_code[table_addr+2]&0x60;	
	temp1>>=5;

	// ȡ���괺�����ڵĹ�����  
	temp2=year_code[table_addr+2]&31; 

	// ���㵱�괺���뵱��Ԫ��������,����ֻ���ڹ���1�»�2��  
	if(temp1==1) 	temp3=temp2-1; 
	else 			temp3=temp2+31-1; 

	// ���㹫�����뵱��Ԫ��������  
	if (month<10) 	temp4=day_code1[month-1]+day-1;
	else  			temp4=day_code2[month-10]+day-1;
	// ��������´���2�²��Ҹ����2��Ϊ����,������1  
	if ((month>2)&&(yearL%4==0)) 	temp4++;

	// �жϹ������ڴ���ǰ���Ǵ��ں�  
	if (temp4>=temp3)
	{ 						
		temp4-=temp3;
		month=1;
		month_p=1;
							
		flag_y=0;
		if(GetMoonDay(month_p,table_addr)==0)	temp1=29; //С��29��
		else 									temp1=30; //��С30��
		// �����ݱ���ȡ����������·�,��Ϊ0�����������  
		temp2=year_code[table_addr]/16; 	
		while(temp4>=temp1)
		{
			temp4-=temp1;
			month_p++;
			if(month==temp2)
			{
				flag_y=~flag_y;
				if(flag_y==0)month++;
			}
			else month++;
			if(GetMoonDay(month_p,table_addr)==0)	temp1=29;
			else 									temp1=30;
		}
		day=temp4+1;
	}
	// �������ڴ���ǰʹ����������������  
	else
	{ 						
		temp3-=temp4;
		if (yearL==0)
		{
			yearL=100-1;
			yearH=19;
		}
		else yearL--;
		table_addr-=3;
		month=12;
		temp2=year_code[table_addr]/16; 	
		if (temp2==0)	month_p=12; 
		else 			month_p=13; 

		flag_y=0;
		if(GetMoonDay(month_p,table_addr)==0)	temp1=29; 
		else 									temp1=30; 
		while(temp3>temp1)
		{
			temp3-=temp1;
			month_p--;
			if(flag_y==0)		month--;
			if(month==temp2)	flag_y=~flag_y;
			if(GetMoonDay(month_p,table_addr)==0)	temp1=29;
			else 									temp1=30;
		}
		day=temp1-temp3+1;
	}

	*p++=yearH;
	*p++=yearL;
	*p++=month;
	*p=day;	
	return(1);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������:GetWeek
// ��������:���빫�����ڵõ�����(ֻ����1901-2099��)
// �䡡��:  year        ������
//          month       ������
//          day	        ������
//          p           �������ڵ�ַ
// �䡡��:  ��																							 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void GetWeek(u16 year,u8 month,u8 day,u8 *p)
{	
	u16 temp2;
	u8 yearH,yearL;
	
	yearH=year/100;	yearL=year%100;
	
	// ���Ϊ21����,�������100  
	if (yearH>19)	 	yearL+=100;
	// ����������ֻ��1900��֮���  
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	*p=(temp2%7);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������:GetSkyEarth
// ��������:���빫�����ڵõ�һ��������(ֻ����1901-2099��)
// �䡡��:  year        ������
//          p           �������ڵ�ַ
// �䡡��:  ��																							   
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void GetSkyEarth(u16 year,u8 *p)
{
	u8 x;
	
	if(year>=1984)
	{
		year=year-1984;
		x=year%60;				
	}
	else
	{
		year=1984-year;
		x=60-year%60;
	}
	*p=x;
}
//��ָ���ַ�source����no����target
void StrCopy(u8 *target,u8 const *source,u8 no)
{
	u16 i;	 
	for(i=0;i<no;i++)
	{
		*target++=*source++;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������:GetChinaCalendarStr
// ��������:���빫�����ڵõ�ũ���ַ���	
//          ��:GetChinaCalendarStr(2007,02,06,str) ����str="����������ʮ��"
// �䡡��:  year        ������
//          month       ������
//          day         ������
//          str         ����ũ�������ַ�����ַ   15Byte
// �䡡��:  ��																							  
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void GetChinaCalendarStr(u16 year,u8 month,u8 day,u8 *str)
{
	u8 NLyear[4];
	u8 SEyear;
	
	StrCopy(&str[0],(u8 *)"���������³�һ",15);
	if(GetChinaCalendar(year,month,day,(u8 *)NLyear)==0)	return;
	GetSkyEarth(NLyear[0]*100+NLyear[1],&SEyear);
	StrCopy(&str[0],(u8 *)  sky[SEyear%10],2);	//  ��
	StrCopy(&str[2],(u8 *)earth[SEyear%12],2);	//  ��	
	
	if(NLyear[2]==1)	StrCopy(&str[6],(u8 *)"��",2);
	else				StrCopy(&str[6],(u8 *)monthcode[NLyear[2]-1],2);		
	
	if(NLyear[3]>10) 	StrCopy(&str[10],(u8 *)nongliday[NLyear[3]/10],2);	
	else				StrCopy(&str[10],(u8 *)"��",2);
	StrCopy(&str[12],(u8 *)monthcode[(NLyear[3]-1)%10],2);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������:GetJieQi
// ��������:���빫�����ڵõ�����24�������� day<15�����ϰ��½���,��֮�����°���	
//          ��:GetJieQiStr(2007,02,08,str) ����str[0]=4
// �䡡��:  year        ������
//          month       ������
//          day         ������
//          str         �����Ӧ���½������ڵ�ַ   1Byte
// �䡡��:  1           �ɹ�
//          0           ʧ��																			  
/////////////////////////////////////////////////////////////////////////////////////////////////////////
u8 GetJieQi(u16 year,u8 month,u8 day,u8 *JQdate)
{
	u8 bak1,value,JQ;

	if((year<2000)||(year>2050))     return 0;//������ķ�Χ����
	if((month==0) ||(month>12))      return 0;
	JQ = (month-1) *2 ;		                        //��ý���˳����(0��23
	if(day >= 15) JQ++; 	                        //�ж��Ƿ����ϰ���

	bak1=YearMonthBit[(year-2000)*3+JQ/8];          //��ý����������ֵ�����ֽ�  
	value =((bak1<<(JQ%8))&0x80);                   //��ý����������ֵ״̬

	*JQdate=days[JQ];								//�õ�����������
	if( value != 0 )
	{
		//�ж����,�Ծ����������ֵ1����1,���ǣ�1��
		if( (JQ== 1||JQ== 11||JQ== 18||JQ== 21)&&year< 2044)  (*JQdate)++;
		else                                                  (*JQdate)--;
	}
	return 1;
}
static u8 const MonthDayMax[]={31,28,31,30,31,30,31,31,30,31,30,31,};
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������:GetJieQiStr
// ��������:���빫�����ڵõ�24�����ַ���	
//          ��:GetJieQiStr(2007,02,08,str) ����str="����ˮ����11��"
// �䡡��:  year        ������
//          month       ������
//          day         ������
//          str         ����24�����ַ�����ַ   15Byte
// �䡡��:  1           �ɹ�
//          0           ʧ��																			  
/////////////////////////////////////////////////////////////////////////////////////////////////////////
u8 GetJieQiStr(u16 year,u8 month,u8 day,u8 *str)
{
	u8 JQdate,JQ,MaxDay;

	if(GetJieQi(year,month,day,&JQdate)==0)	return 0;

	JQ = (month-1) *2 ;                             //��ý���˳����(0��23
	if(day >= 15) JQ++;                             //�ж��Ƿ����ϰ���

	if(day==JQdate)                                 //��������һ��������
	{
		StrCopy(str,(u8 *)JieQiStr[JQ],5);
		return 1;
	}
	                                                //���첻��һ��������
	StrCopy(str,(u8 *)"��С������??��",15);
	if(day<JQdate)                                  //�����������С�ڱ��µĽ�������
	{
		StrCopy(&str[2],(u8 *)JieQiStr[JQ],4);
		day=JQdate-day;
	} 
	else                                            //����������ڴ��ڱ��µĽ�������
	{
		StrCopy(&str[2],(u8 *)JieQiStr[JQ+1],4);
		if(day < 15)
		{
			GetJieQi(year,month,15,&JQdate);
			day=JQdate-day;
		}
		else                                        //����
		{
			MaxDay=MonthDayMax[month-1];
			if(month==2)                            //��������
			{
				if((year%4==0)&&((year%100!=0)||(year%400==0))) MaxDay++;
			}
			if(++month==13)	month=1;
			GetJieQi(year,month,1,&JQdate);
			day=MaxDay-day+JQdate;
		}
	}
	str[10]=day/10+'0';
	str[11]=day%10+'0';
	return 1;
}
