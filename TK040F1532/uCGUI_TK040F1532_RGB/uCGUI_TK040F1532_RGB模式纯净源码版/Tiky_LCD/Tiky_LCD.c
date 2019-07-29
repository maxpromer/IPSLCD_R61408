#include "stm32f10x.h"
#include "Tiky_LCD.h" 
#define LCD_SPI_CS(a)	\
						if (a)	\
						GPIOC->BSRR = GPIO_Pin_5;	\
						else		\
						GPIOC->BRR  = GPIO_Pin_5;
#define SPI_DCLK(a)	\
						if (a)	\
						GPIOB->BSRR = GPIO_Pin_13;	\
						else		\
						GPIOB->BRR  = GPIO_Pin_13;
#define SPI_SDA(a)	\
						if (a)	\
						GPIOB->BSRR = GPIO_Pin_15;	\
						else		\
						GPIOB->BRR  = GPIO_Pin_15;

//800*480
#define  HDP 479  //Hsync Display period   
#define  VDP 799 //Vertical Display period

#define  HT  580	  //Horizontal total period = (HT + 1) pixels
#define  HPS 20	  //Horizontal Sync Pulse Start Position = (HPS + 1) pixels	 		 
#define  LPS 1	  //Horizontal Display Period Start Position = LPS pixels		
#define  HPW 12   //Horizontal Sync Pulse Width = (HPW + 1) pixels	

#define  VT 850	 //Vertical Total = (VT + 1) lines
#define  VPS 25  //Vertical Sync Pulse Start Position = VPS lines					  
#define  FPS 1   //Vertical Display Period Start Position = FPS lines				  
#define  VPW 12  //Vertical Sync Pulse Width = (VPW + 1) lines
						
////240*320
//#define  HDP 239  //Hsync Display period 
//#define  HT  250   //Horizontal total period = (HT + 1) pixels
//#define  HPS 6   //Horizontal Sync Pulse Start Position = (HPS + 1) pixels	 		 
//#define  LPS 2	  //Horizontal Display Period Start Position = LPS pixels		
//#define  HPW 2   //Horizontal Sync Pulse Width = (HPW + 1) pixels	

//#define  VDP 319 //Vertical Display period						
//#define  VT 330	 //Vertical Total = (VT + 1) lines
//#define  VPS 6  //Vertical Sync Pulse Start Position = VPS lines					  
//#define  FPS 2   //Vertical Display Period Start Position = FPS lines				  
//#define  VPW 2  //Vertical Sync Pulse Width = (VPW + 1) lines
/*
 * 函数名：LCD_GPIO_Config
 * 描述  ：根据FSMC配置LCD的I/O
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用        
 */
static void LCD_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* Enable the FSMC Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
    
    /* config lcd gpio clock base on FSMC */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE , ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    
    /* config tft Reset gpio */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;		
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    /* config tft BL gpio 背光开关控制引脚*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ; 	 
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;		
    GPIO_Init(GPIOC, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15; 	 
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
    
    /* config tft data lines base on FSMC
	 * data lines,FSMC-D0~D15: PD 14 15 0 1,PE 7 8 9 10 11 12 13 14 15,PD 8 9 10
	 */	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | 
                                  GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                  GPIO_Pin_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure); 
    
    /* config tft control lines base on FSMC
	 * PD4-FSMC_NOE  :LCD-RD
   * PD5-FSMC_NWE  :LCD-WR
	 * PD7-FSMC_NE1  :LCD-CS
   * PD11-FSMC_A16 :LCD-DC
	 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);  
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);  
    
    /* tft control gpio init */	 
    GPIO_SetBits(GPIOD, GPIO_Pin_13);		 // Reset				
    GPIO_SetBits(GPIOD, GPIO_Pin_4);		 // RD = 1  
    GPIO_SetBits(GPIOD, GPIO_Pin_5);		 // WR = 1 
		GPIO_SetBits(GPIOD, GPIO_Pin_11);		 // RS
    GPIO_SetBits(GPIOD, GPIO_Pin_7);		 //	CS = 1

		GPIO_SetBits(GPIOA, GPIO_Pin_1);	 	 //	BL = 1 
		
		
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	//按键 OK
    GPIO_Init(GPIOA, &GPIO_InitStructure);
																//  right         up           left        down
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_1 | GPIO_Pin_5 | GPIO_Pin_6;		
    GPIO_Init(GPIOE, &GPIO_InitStructure);

}

/*
 * 函数名：LCD_FSMC_Config
 * 描述  ：LCD  FSMC 模式配置
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用        
 */

static void LCD_FSMC_Config(void)
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  p; 
    
    
    p.FSMC_AddressSetupTime = 0x02;	 //地址建立时间
    p.FSMC_AddressHoldTime = 0x00;	 //地址保持时间
    p.FSMC_DataSetupTime = 0x05;		 //数据建立时间
    p.FSMC_BusTurnAroundDuration = 0x00;
    p.FSMC_CLKDivision = 0x00;
    p.FSMC_DataLatency = 0x00;

    p.FSMC_AccessMode = FSMC_AccessMode_B;	 // 一般使用模式B来控制LCD
    
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p; 
    
    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
    
    /* Enable FSMC Bank1_SRAM Bank */
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  
}

volatile void LCD_delay(int j)
{
volatile u16 i;	
	while(j--)
for(i=7200;i>0;i--);
}
static void LCD_Rst(void)
{			
    GPIO_ResetBits(GPIOD, GPIO_Pin_13);
    LCD_delay(100);					   
    GPIO_SetBits(GPIOD, GPIO_Pin_13 );		 	 
    LCD_delay(100);	
}
 void WriteComm(u16 CMD)
{			
	*(__IO u16 *) (Bank1_LCD_C) = CMD;
}
 void WriteData(u16 tem_data)
{			
	*(__IO u16 *) (Bank1_LCD_D) = tem_data;
}

void LCD_WriteByteSPI(unsigned char byte)
{
		unsigned char n;
   
   for(n=0; n<8; n++)			
   {  
	  if(byte&0x80) SPI_SDA(1)
      	else SPI_SDA(0)
      byte<<= 1;
		 
	  SPI_DCLK(0);
    SPI_DCLK(1);
   }
}

void SPI_WriteComm(u16 CMD)
{			
	LCD_SPI_CS(0);
	SPI_SDA(0);
	SPI_DCLK(0);
	SPI_DCLK(1);
	LCD_WriteByteSPI(CMD);
	LCD_SPI_CS(1);
}
void SPI_WriteData(u16 tem_data)
{			
	LCD_SPI_CS(0);
	SPI_SDA(1);
	SPI_DCLK(0);
	SPI_DCLK(1);
	LCD_WriteByteSPI(tem_data);
	LCD_SPI_CS(1);
}
/**********************************************
Lcd初始化函数
***********************************************/
void Lcd_Initialize(void)
{	
LCD_GPIO_Config();
LCD_FSMC_Config();
	LCD_SPI_CS(0);
LCD_Rst();



SPI_WriteComm(0x11); 
LCD_delay(40);
SPI_WriteComm(0xB0);
SPI_WriteData(0x04);
	
SPI_WriteComm(0xB3);
SPI_WriteData(0x10);//0x02
SPI_WriteData(0x00);
SPI_WriteData(0x00);

SPI_WriteComm(0xB6);
SPI_WriteData(0x52);
SPI_WriteData(0x83);

SPI_WriteComm(0xB7);
SPI_WriteData(0x80);
SPI_WriteData(0x72);
SPI_WriteData(0x11);
SPI_WriteData(0x25);

SPI_WriteComm(0xB8);
SPI_WriteData(0x00);
SPI_WriteData(0x0F);
SPI_WriteData(0x0F);
SPI_WriteData(0xFF);
SPI_WriteData(0xFF);
SPI_WriteData(0xC8);
SPI_WriteData(0xC8);
SPI_WriteData(0x02);
SPI_WriteData(0x18);
SPI_WriteData(0x10);
SPI_WriteData(0x10);
SPI_WriteData(0x37);
SPI_WriteData(0x5A);
SPI_WriteData(0x87);
SPI_WriteData(0xBE);
SPI_WriteData(0xFF);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteComm(0xB9);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteComm(0xBD);
SPI_WriteData(0x00);

SPI_WriteComm(0xC0);
SPI_WriteData(0x02);
SPI_WriteData(0x76);

SPI_WriteComm(0xC1);
SPI_WriteData(0x63);
SPI_WriteData(0x31);
SPI_WriteData(0x00);
SPI_WriteData(0x27);
SPI_WriteData(0x27);
SPI_WriteData(0x32);
SPI_WriteData(0x12);
SPI_WriteData(0x28);
SPI_WriteData(0x4E);
SPI_WriteData(0x10);
SPI_WriteData(0xA5);
SPI_WriteData(0x0F);
SPI_WriteData(0x58);
SPI_WriteData(0x21);
SPI_WriteData(0x01);

SPI_WriteComm(0xC2);
SPI_WriteData(0x28);
SPI_WriteData(0x06);
SPI_WriteData(0x06);
SPI_WriteData(0x01);
SPI_WriteData(0x03);
SPI_WriteData(0x00);

SPI_WriteComm(0xC3);
SPI_WriteData(0x40);
SPI_WriteData(0x00);
SPI_WriteData(0x03);
SPI_WriteComm(0xC4);
SPI_WriteData(0x00);
SPI_WriteData(0x01);
SPI_WriteComm(0xC6);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteComm(0xC7);
SPI_WriteData(0x11);
SPI_WriteData(0x8D);
SPI_WriteData(0xA0);
SPI_WriteData(0xF5);
SPI_WriteData(0x27);
SPI_WriteComm(0xC8);
SPI_WriteData(0x02);
SPI_WriteData(0x13);
SPI_WriteData(0x18);
SPI_WriteData(0x25);
SPI_WriteData(0x34);
SPI_WriteData(0x4E);
SPI_WriteData(0x36);
SPI_WriteData(0x23);
SPI_WriteData(0x17);
SPI_WriteData(0x0E);
SPI_WriteData(0x0C);
SPI_WriteData(0x02);
SPI_WriteData(0x02);
SPI_WriteData(0x13);
SPI_WriteData(0x18);
SPI_WriteData(0x25);
SPI_WriteData(0x34);
SPI_WriteData(0x4E);
SPI_WriteData(0x36);
SPI_WriteData(0x23);
SPI_WriteData(0x17);
SPI_WriteData(0x0E);
SPI_WriteData(0x0C);
SPI_WriteData(0x02);
SPI_WriteComm(0xC9);
SPI_WriteData(0x02);
SPI_WriteData(0x13);
SPI_WriteData(0x18);
SPI_WriteData(0x25);
SPI_WriteData(0x34);
SPI_WriteData(0x4E);
SPI_WriteData(0x36);
SPI_WriteData(0x23);
SPI_WriteData(0x17);
SPI_WriteData(0x0E);
SPI_WriteData(0x0C);
SPI_WriteData(0x02);
SPI_WriteData(0x02);
SPI_WriteData(0x13);
SPI_WriteData(0x18);
SPI_WriteData(0x25);
SPI_WriteData(0x34);
SPI_WriteData(0x4E);
SPI_WriteData(0x36);
SPI_WriteData(0x23);
SPI_WriteData(0x17);
SPI_WriteData(0x0E);
SPI_WriteData(0x0C);
SPI_WriteData(0x02);
SPI_WriteComm(0xCA);
SPI_WriteData(0x02);
SPI_WriteData(0x13);
SPI_WriteData(0x18);
SPI_WriteData(0x25);
SPI_WriteData(0x34);
SPI_WriteData(0x4E);
SPI_WriteData(0x36);
SPI_WriteData(0x23);
SPI_WriteData(0x17);
SPI_WriteData(0x0E);
SPI_WriteData(0x0C);
SPI_WriteData(0x02);
SPI_WriteData(0x02);
SPI_WriteData(0x13);
SPI_WriteData(0x18);
SPI_WriteData(0x25);
SPI_WriteData(0x34);
SPI_WriteData(0x4E);
SPI_WriteData(0x36);
SPI_WriteData(0x23);
SPI_WriteData(0x17);
SPI_WriteData(0x0E);
SPI_WriteData(0x0C);
SPI_WriteData(0x02);
SPI_WriteComm(0xD0);
SPI_WriteData(0xA9);
SPI_WriteData(0x03);
SPI_WriteData(0xCC);
SPI_WriteData(0xA5);
SPI_WriteData(0x00);
SPI_WriteData(0x53);
SPI_WriteData(0x20);
SPI_WriteData(0x10);
SPI_WriteData(0x01);
SPI_WriteData(0x00);
SPI_WriteData(0x01);
SPI_WriteData(0x01);
SPI_WriteData(0x00);
SPI_WriteData(0x03);
SPI_WriteData(0x01);
SPI_WriteData(0x00);
SPI_WriteComm(0xD1);
SPI_WriteData(0x18);
SPI_WriteData(0x0C);
SPI_WriteData(0x23);
SPI_WriteData(0x03);
SPI_WriteData(0x75);
SPI_WriteData(0x02);
SPI_WriteData(0x50);
SPI_WriteComm(0xD3);
SPI_WriteData(0x33);
SPI_WriteComm(0xD5);
SPI_WriteData(0x2a);
SPI_WriteData(0x2a);
SPI_WriteComm(0xD6);
SPI_WriteData(0x28);//a8
SPI_WriteComm(0xD7);
SPI_WriteData(0x01);
SPI_WriteData(0x00);
SPI_WriteData(0xAA);
SPI_WriteData(0xC0);
SPI_WriteData(0x2A);
SPI_WriteData(0x2C);
SPI_WriteData(0x22);
SPI_WriteData(0x12);
SPI_WriteData(0x71);
SPI_WriteData(0x0A);
SPI_WriteData(0x12);
SPI_WriteData(0x00);
SPI_WriteData(0xA0);
SPI_WriteData(0x00);
SPI_WriteData(0x03);
SPI_WriteComm(0xD8);
SPI_WriteData(0x44);
SPI_WriteData(0x44);
SPI_WriteData(0x22);
SPI_WriteData(0x44);
SPI_WriteData(0x21);
SPI_WriteData(0x46);
SPI_WriteData(0x42);
SPI_WriteData(0x40);
SPI_WriteComm(0xD9);
SPI_WriteData(0xCF);
SPI_WriteData(0x2D);
SPI_WriteData(0x51);
SPI_WriteComm(0xDA);
SPI_WriteData(0x01);
SPI_WriteComm(0xDE);
SPI_WriteData(0x01);
SPI_WriteData(0x51);//58
SPI_WriteComm(0xE1);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteComm(0xE6);
SPI_WriteData(0x55);//58
SPI_WriteComm(0xF3);
SPI_WriteData(0x06);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x24);
SPI_WriteData(0x00);
SPI_WriteComm(0xF8);
SPI_WriteData(0x00);
SPI_WriteComm(0xFA);
SPI_WriteData(0x01);
SPI_WriteComm(0xFB);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteComm(0xFC);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteComm(0xFD);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x70);
SPI_WriteData(0x00);
SPI_WriteData(0x72);
SPI_WriteData(0x31);
SPI_WriteData(0x37);
SPI_WriteData(0x70);
SPI_WriteData(0x32);
SPI_WriteData(0x31);
SPI_WriteData(0x07);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteComm(0xFE);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x20);
SPI_WriteComm(0xB0);
SPI_WriteData(0x04); //04
LCD_delay(40);
SPI_WriteComm(0x35);
SPI_WriteData(0x00);
SPI_WriteComm(0x44);
SPI_WriteData(0x00);
SPI_WriteComm(0x36);
SPI_WriteData(0x00);
SPI_WriteComm(0x3A);
SPI_WriteData(0x77);
SPI_WriteComm(0x2A);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x01);
SPI_WriteData(0xDF);
SPI_WriteComm(0x2B);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x03);
SPI_WriteData(0x1F);
SPI_WriteComm(0x29);    
LCD_delay(10);
SPI_WriteComm(0x2C);
LCD_delay(10); 
	Lcd_Light_ON;

SPI_WriteComm(0x36);
SPI_WriteData(0x08);
	//SSD1963,800*480

//注意：下面代码是桥接芯片SSD1963的初始化，有RGB功能的单片机不需要此部分代码
	//注意：下面代码是桥接芯片SSD1963的初始化，有RGB功能的单片机不需要此部分代码
	//注意：下面代码是桥接芯片SSD1963的初始化，有RGB功能的单片机不需要此部分代码


	WriteComm(0x00E2);	//set_pll_mn
	WriteData(0x0034);	//M=0x23=35, VCO = Reference input clock x (M + 1),PLL multiplier, set PLL clock to 120M
	WriteData(0x0001);	//N=0x02=2,	 PLL frequency  = VCO / (N + 1) 
	WriteData(0x0004);	//0x36 for 6.5M, 0x23 for 10M crystal

	WriteComm(0x00E0);  //set_pll
	WriteData(0x0001);				  
	LCD_delay(3);
	WriteComm(0x00E0);
	WriteData(0x0003);
	LCD_delay(5);

	WriteComm(0x0001);  // software	reset
	LCD_delay(5);

	WriteComm(0x00E6);//Set LSHIFT(pixel clock) Frequency
	WriteData(0x0001);
	WriteData(0x0060);
	WriteData(0x0094);

	WriteComm(0x00B0);	//set_lcd_mode 
	WriteData(0x0024);	//18BIT  Data latch in rising edge
	WriteData(0x0000);
	WriteData((HDP>>8)&0X00FF);  //Set HDP
	WriteData(HDP&0X00FF);
    WriteData((VDP>>8)&0X00FF);  //Set VDP
	WriteData(VDP&0X00FF);
    WriteData(0x0000);
	LCD_delay(5);

	WriteComm(0x00B4);	//set_hori_period,HSYNC
	WriteData((HT>>8)&0X00FF);  //Set HT
	WriteData(HT&0X00FF);
	WriteData((HPS>>8)&0X00FF);  //Set HPS
	WriteData(HPS&0X00FF);
	WriteData(HPW);			   //Set HPW
	WriteData((LPS>>8)&0X00FF);  //SetLPS
	WriteData(LPS&0X00FF);
	WriteData(0x0000);

	WriteComm(0x00B6);	//set_vert_period,VSYNC
	WriteData((VT>>8)&0X00FF);   //Set VT
	WriteData(VT&0X00FF);
	WriteData((VPS>>8)&0X00FF);  //Set VPS
	WriteData(VPS&0X00FF);
	WriteData(VPW);			   //Set VPW
	WriteData((FPS>>8)&0X00FF);  //Set FPS
	WriteData(FPS&0X00FF);


	WriteComm(0x0036); //set_address_mode  
	WriteData(0x0000);

	WriteComm(0x00F0); //set_pixel_data_interface   
  WriteData(0x0003); //0x03:16-bit(565 format);0x04:18bit;0x05:24bit	
	LCD_delay(5);
	WriteComm(0x0029); 
	LCD_delay(5);
	Lcd_ColorBox(0,0,100,100,RED);
//while(1);
}
/******************************************
函数名：Lcd写命令函数
功能：向Lcd指定位置写入应有命令或数据
入口参数：Index 要寻址的寄存器地址
          ConfigTemp 写入的数据或命令值
******************************************/
void LCD_WR_REG(u16 Index,u16 CongfigTemp)
{
	SPI_WriteComm(Index);
	SPI_WriteData(CongfigTemp);
}

void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend) 
{
	WriteComm(0x2a);   
	WriteData(Xstart>>8);
	WriteData(Xstart);
	WriteData(Xend>>8);
	WriteData(Xend);

	WriteComm(0x2b);  	
	WriteData(Ystart>>8);
	WriteData(Ystart);
	WriteData(Yend>>8);
	WriteData(Yend);

	WriteComm(0x2c);
}
u16 GetPoint(u16 x,u16 y)
{
	u16 a,b;
	BlockWrite(x,x,y,y);

	WriteComm(0x2e00);x=1;while(--x);
	
	a = *(__IO u16 *) (Bank1_LCD_D);x=1;while(--x);
// 	printf("RIN=%04x\r\n",a);
	a = *(__IO u16 *) (Bank1_LCD_D);x=1;while(--x);
// 	printf("RIN=%04x\r\n",a);
	b = *(__IO u16 *) (Bank1_LCD_D);x=1;while(--x);
// 	printf("RIN=%04x\r\n",b);

	return (a&0xf800)|((a&0x00fc)<<3)|(b>>11);
}
/**********************************************
函数名：Lcd块选函数
功能：选定Lcd上指定的矩形区域

注意：xStart和 yStart随着屏幕的旋转而改变，位置是矩形框的四个角

入口参数：xStart x方向的起始点
          ySrart y方向的终止点
          xLong 要选定矩形的x方向长度
          yLong  要选定矩形的y方向长度
返回值：无
***********************************************/
void Lcd_ColorBox(u16 xStart,u16 yStart,u16 xLong,u16 yLong,u16 Color)
{
	u32 temp;

	BlockWrite(xStart,xStart+xLong-1,yStart,yStart+yLong-1);
	for (temp=0; temp<xLong*yLong; temp++)
	{
		*(__IO u16 *) (Bank1_LCD_D) = Color;
	}
}

/******************************************
函数名：Lcd图像填充100*100
功能：向Lcd指定位置填充图像
入口参数：Index 要寻址的寄存器地址
          ConfigTemp 写入的数据或命令值
******************************************/
void LCD_Fill_Pic(u16 x, u16 y,u16 pic_H, u16 pic_V, const unsigned char* pic)
{
  unsigned long i;
	unsigned int j;

// 	WriteComm(0x3600); //Set_address_mode
// 	WriteData(0x00); //横屏，从左下角开始，从左到右，从下到上
	BlockWrite(x,x+pic_H-1,y,y+pic_V-1);
	for (i = 0; i < pic_H*pic_V*2; i+=2)
	{
		j=pic[i];
		j=j<<8;
		j=j+pic[i+1];
		*(__IO u16 *) (Bank1_LCD_D) = j;
	}
// 	WriteComm(0x3600); //Set_address_mode
// 	WriteData(0xA0);
}

//在指定座标上打一个点
void DrawPixel(u16 x, u16 y, int Color)
{
	BlockWrite(x,x,y,y);

	*(__IO u16 *) (Bank1_LCD_D) = Color;
}

