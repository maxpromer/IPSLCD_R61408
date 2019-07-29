#include "stm32f10x.h"
#include "Tiky_LCD.h" 

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
    
    /* config tft data lines base on FSMC
	 * data lines,FSMC-D0~D15: PD 14 15 0 1,PE 7 8 9 10 11 12 13 14 15,PD 8 9 10
	 */	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
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

volatile void LCD_delay(char j)
{
volatile u16 i;	
	while(j--)
for(i=7200;i>0;i--);
}
static void LCD_Rst(void)
{			
    GPIO_ResetBits(GPIOD, GPIO_Pin_13);
    LCD_delay(10);					   
    GPIO_SetBits(GPIOD, GPIO_Pin_13 );		 	 
    LCD_delay(10);	
}
 void WriteComm(u16 CMD)
{			
	*(__IO u16 *) (Bank1_LCD_C) = CMD;
}
 void WriteData(u16 tem_data)
{			
	*(__IO u16 *) (Bank1_LCD_D) = tem_data;
}


/**********************************************
Lcd初始化函数
***********************************************/
void Lcd_Initialize(void)
{	
LCD_GPIO_Config();
LCD_FSMC_Config();
LCD_Rst();
	
WriteComm(0x11); // exit_sleep_mode
WriteData(0x00);   
LCD_delay(40);
WriteComm(0xB0); // MCAP: Manufacturer Command Access Protect
WriteData(0x04); // Manufacturer Command inputs are enabled - enabled all
WriteComm(0xB3); // Frame Memory Access and Interface Setting (B3h) 
WriteData(0x02); // The write start position is reset to the start of window address area to overwrite the subsequent data to the previous data.
WriteData(0x00); // OK !
WriteComm(0xB6); // DSI Control (B6h) 
WriteData(0x52);
WriteData(0x83); // Operating frequency (MHz), Min: 140 Max: 210
WriteComm(0xB7); // I don't know !
WriteData(0x80);
WriteData(0x72);
WriteData(0x11);
WriteData(0x25);
WriteComm(0xB8); // I don't know !
WriteData(0x00);
WriteData(0x0F);
WriteData(0x0F);
WriteData(0xFF);
WriteData(0xFF);
WriteData(0xC8);
WriteData(0xC8);
WriteData(0x02);
WriteData(0x18);
WriteData(0x10);
WriteData(0x10);
WriteData(0x37);
WriteData(0x5A);
WriteData(0x87);
WriteData(0xBE);
WriteData(0xFF);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteComm(0xB9); // Backlight Control (2) (B9h)
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteComm(0xBD); // Resizing Control (BDh) 
WriteData(0x00);
WriteComm(0xC0); // Panel Driving Setting 1 (C0h) 
WriteData(0x02);
WriteData(0x76);
WriteComm(0xC1); // Panel Driving Setting 2 (C1h) 
WriteData(0x63);
WriteData(0x31);
WriteData(0x00);
WriteData(0x27);
WriteData(0x27);
WriteData(0x32);
WriteData(0x12);
WriteData(0x28);
WriteData(0x4E);
WriteData(0x10);
WriteData(0xA5);
WriteData(0x0F);
WriteData(0x58);
WriteData(0x21);
WriteData(0x01);
WriteComm(0xC2); // Display V-Timing Setting (C2h) 
WriteData(0x28);
WriteData(0x06);
WriteData(0x06);
WriteData(0x01);
WriteData(0x03);
WriteData(0x00);
WriteComm(0xC3); // Test Mode 1 (C3h) 
WriteData(0x40);
WriteData(0x00);
WriteData(0x03);
WriteComm(0xC4); // Panel Driving Setting 3 (C4h) 
WriteData(0x00);
WriteData(0x01);
WriteComm(0xC6); // Outline Sharpening Control (C6h) 
WriteData(0x00);
WriteData(0x00);
WriteComm(0xC7); // Panel Driving Setting 4 (C7h) 
WriteData(0x11);
WriteData(0x8D);
WriteData(0xA0);
WriteData(0xF5);
WriteData(0x27);
WriteComm(0xC8); // Gamma Setting A Set (C8h) 
WriteData(0x02);
WriteData(0x13);
WriteData(0x18);
WriteData(0x25);
WriteData(0x34);
WriteData(0x4E);
WriteData(0x36);
WriteData(0x23);
WriteData(0x17);
WriteData(0x0E);
WriteData(0x0C);
WriteData(0x02);
WriteData(0x02);
WriteData(0x13);
WriteData(0x18);
WriteData(0x25);
WriteData(0x34);
WriteData(0x4E);
WriteData(0x36);
WriteData(0x23);
WriteData(0x17);
WriteData(0x0E);
WriteData(0x0C);
WriteData(0x02);
WriteComm(0xC9); // Gamma Setting B Set (C9h)
WriteData(0x02);
WriteData(0x13);
WriteData(0x18);
WriteData(0x25);
WriteData(0x34);
WriteData(0x4E);
WriteData(0x36);
WriteData(0x23);
WriteData(0x17);
WriteData(0x0E);
WriteData(0x0C);
WriteData(0x02);
WriteData(0x02);
WriteData(0x13);
WriteData(0x18);
WriteData(0x25);
WriteData(0x34);
WriteData(0x4E);
WriteData(0x36);
WriteData(0x23);
WriteData(0x17);
WriteData(0x0E);
WriteData(0x0C);
WriteData(0x02);
WriteComm(0xCA); // Gamma Setting C Set (CAh) 
WriteData(0x02);
WriteData(0x13);
WriteData(0x18);
WriteData(0x25);
WriteData(0x34);
WriteData(0x4E);
WriteData(0x36);
WriteData(0x23);
WriteData(0x17);
WriteData(0x0E);
WriteData(0x0C);
WriteData(0x02);
WriteData(0x02);
WriteData(0x13);
WriteData(0x18);
WriteData(0x25);
WriteData(0x34);
WriteData(0x4E);
WriteData(0x36);
WriteData(0x23);
WriteData(0x17);
WriteData(0x0E);
WriteData(0x0C);
WriteData(0x02);
WriteComm(0xD0); // Power Setting (Charge Pump Setting) (D0h) 
WriteData(0xA9);
WriteData(0x03);
WriteData(0xCC);
WriteData(0xA5);
WriteData(0x00);
WriteData(0x53);
WriteData(0x20);
WriteData(0x10);
WriteData(0x01);
WriteData(0x00);
WriteData(0x01);
WriteData(0x01);
WriteData(0x00);
WriteData(0x03);
WriteData(0x01);
WriteData(0x00);
WriteComm(0xD1); // Power Setting (Switching Regulator Setting) (D1h)
WriteData(0x18);
WriteData(0x0C);
WriteData(0x23);
WriteData(0x03);
WriteData(0x75);
WriteData(0x02);
WriteData(0x50);
WriteComm(0xD3); // Power Setting for Internal Mode (D3h) 
WriteData(0x33);
WriteComm(0xD5); // VPLVL/VNLVL Setting (D5h) 
WriteData(0x2a);
WriteData(0x2a);
WriteComm(0xD6); // Test Mode 2 (D6h) 
WriteData(0x28); //a8
WriteComm(0xD7); // Test Mode 3 (D7h) 
WriteData(0x01);
WriteData(0x00);
WriteData(0xAA);
WriteData(0xC0);
WriteData(0x2A);
WriteData(0x2C);
WriteData(0x22);
WriteData(0x12);
WriteData(0x71);
WriteData(0x0A);
WriteData(0x12);
WriteData(0x00);
WriteData(0xA0);
WriteData(0x00);
WriteData(0x03);
WriteComm(0xD8); // Test Mode 4 (D8h) 
WriteData(0x44);
WriteData(0x44);
WriteData(0x22);
WriteData(0x44);
WriteData(0x21);
WriteData(0x46);
WriteData(0x42);
WriteData(0x40);
WriteComm(0xD9); // Test Mode 5 (D9h) 
WriteData(0xCF);
WriteData(0x2D);
WriteData(0x51);
WriteComm(0xDA); // Test Mode 6 (DAh) 
WriteData(0x01);
WriteComm(0xDE); // VCOMDC Setting (DEh)
WriteData(0x01);
WriteData(0x51);//58
WriteComm(0xE1); // set_DDB_write_control (E1h) 
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteComm(0xE6); // VCOMDC Setting 2 (E6h) 
WriteData(0x55); //58
WriteComm(0xF3); // Test Mode 12 (F3h) 
WriteData(0x06);
WriteData(0x00);
WriteData(0x00);
WriteData(0x24);
WriteData(0x00);
WriteComm(0xF8); // Test Mode 13 (F8h) 
WriteData(0x00);
WriteComm(0xFA); // VDC_SEL Setting (FAh)
WriteData(0x01);
WriteComm(0xFB); // Test Mode 15 (FBh) 
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteComm(0xFC); // Test Mode 16 (FCh) 
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteComm(0xFD); // Test Mode 17 (FDh) 
WriteData(0x00);
WriteData(0x00);
WriteData(0x70);
WriteData(0x00);
WriteData(0x72);
WriteData(0x31);
WriteData(0x37);
WriteData(0x70);
WriteData(0x32);
WriteData(0x31);
WriteData(0x07);
WriteData(0x00);
WriteData(0x00);
WriteComm(0xFE); // Test Mode 18 (FEh) 
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteData(0x00);
WriteData(0x20);
WriteComm(0xB0); // MCAP: Manufacturer Command Access Protect
WriteData(0x04); // Manufacturer Command inputs are enabled - enabled all
LCD_delay(40);
WriteComm(0x35); // set_tear_on: 35h 
WriteData(0x00); // 
WriteComm(0x44); // set_tear_scanline:44h
WriteData(0x00);
WriteComm(0x36); // set_address_mode: 36h -> rotation
WriteData(0x00);
WriteComm(0x3A); // set_pixel_format: 3Ah 
WriteData(0x55); // 16 bits/pixel (65,536 colors) 
WriteComm(0x2A); // set_column_address: 2Ah 
WriteData(0x00);
WriteData(0x00);
WriteData(0x01);
WriteData(0xDF);
WriteComm(0x2B); // set_page_address: 2Bh 
WriteData(0x00);
WriteData(0x00);
WriteData(0x03);
WriteData(0x1F);
WriteComm(0x29); // set_display_on: 29h
WriteData(0x00);     
LCD_delay(180);
WriteComm(0x2C); // write_memory_start: 2Ch
WriteData(0x00); 
LCD_delay(10); 
Lcd_Light_ON;
	
WriteComm(0x36); // set_address_mode: 36h
WriteData(0x68);//设置为横屏显示,三原色的顺序是B，G，R
//	Lcd_ColorBox(0,0,800,480,YELLOW);
	
// 	LCD_Fill_Pic(80,160,320,480, gImage_MM_T035);
// 	BlockWrite(0,0,799,479);
}
/******************************************
函数名：Lcd写命令函数
功能：向Lcd指定位置写入应有命令或数据
入口参数：Index 要寻址的寄存器地址
          ConfigTemp 写入的数据或命令值
******************************************/
void LCD_WR_REG(u16 Index,u16 CongfigTemp)
{
	*(__IO u16 *) (Bank1_LCD_C) = Index;	
	*(__IO u16 *) (Bank1_LCD_D) = CongfigTemp;
}

void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend) 
{
	
	WriteComm(0x2a);   
	WriteData(Xstart>>8);
	WriteData(Xstart&0xff);
	WriteData(Xend>>8);
	WriteData(Xend&0xff);

	WriteComm(0x2b);   
	WriteData(Ystart>>8);
	WriteData(Ystart&0xff);
	WriteData(Yend>>8);
	WriteData(Yend&0xff);
	WriteComm(0x2c);
}
 u16 GetPoint( u16 x, u16 y)
{
//	WriteComm(0x2a);   
//	WriteData(x>>8);
//	WriteData(x);


//	WriteComm(0x2b);   
//	WriteData(y>>8);
//	WriteData(y);


//	WriteComm(0x2e);
//	
//	x = *(__IO u16 *) (Bank1_LCD_D);x=1;while(--x);
//	x = *(__IO u16 *) (Bank1_LCD_D);
//	y = *(__IO u16 *) (Bank1_LCD_D);

//	return (x&0xf800)|((x&0x00fc)<<3)|(y>>11);

	return 0;//故意注释掉正确值，返回一个错值，令其出现拖影
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

// 	WriteComm(0x36); //Set_address_mode
// 	WriteData(0x20); //横屏，从左下角开始，从左到右，从下到上
	BlockWrite(x,x+pic_H-1,y,y+pic_V-1);
	for (i = 0; i < pic_H*pic_V*2; i+=2)
	{
		j=pic[i];
		j=j<<8;
		j=j+pic[i+1];
		*(__IO u16 *) (Bank1_LCD_D) = j;
	}
// 	WriteComm(0x36); //Set_address_mode
// 	WriteData(0xA0);
}

//在指定座标上打一个点
void DrawPixel(u16 x, u16 y, int Color)
{
	BlockWrite(x,x,y,y);

	*(__IO u16 *) (Bank1_LCD_D) = Color;
}

