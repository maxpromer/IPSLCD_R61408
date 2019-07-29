#include  "config.h"
#include  <stdio.h>
#include  <string.h>
#include  "ili9320.h"

// zlg_gui.bmp (82x29点)
// 调整后为88x29点。
uint8 const  LOGO_DAT[]=
{
0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x06,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x1F,0x80,0x0C,0x7F,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x31,
0x80,0x0C,0xF3,0x88,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0xC0,0x18,0xE1,0x8E,0x00,
0x00,0x00,0x00,0x03,0x00,0x60,0x40,0x19,0xC1,0x87,0x80,0x00,0x00,0x00,0x0F,0x00,
0x60,0x00,0x31,0xC1,0x87,0x0C,0x00,0x00,0x00,0x1E,0x00,0xE0,0x00,0x33,0x81,0x0E,
0x1F,0x00,0x00,0x00,0x1E,0x00,0xE0,0x00,0x63,0x80,0x0E,0x0E,0x00,0x00,0x00,0x0F,
0x00,0xE0,0x00,0x63,0x80,0x1E,0x0C,0x00,0x00,0x04,0x07,0x00,0xE0,0x70,0xC3,0x80,
0x1C,0x18,0x00,0x00,0x0C,0x07,0x80,0xE1,0xE0,0xC3,0x0E,0x1C,0x18,0x08,0x00,0x0E,
0x03,0x80,0x60,0x61,0x83,0x07,0xD8,0x10,0x0C,0x00,0x1E,0x03,0x80,0x70,0x61,0x83,
0x87,0x38,0x30,0x06,0x00,0x33,0x03,0xC0,0x30,0x63,0x01,0x87,0x78,0x30,0x06,0x00,
0x63,0x01,0xC0,0xF8,0x73,0x01,0x86,0x70,0x20,0x0F,0x00,0x63,0x81,0xC0,0xDC,0x76,
0x01,0xC6,0x70,0x20,0x1C,0x80,0xE1,0x80,0xE0,0xCF,0xC6,0x00,0xFE,0x60,0x40,0x38,
0x40,0x61,0x83,0xE1,0xC3,0x00,0x00,0x38,0x70,0x40,0xF0,0x00,0x20,0xC1,0xF7,0xC0,
0x00,0x00,0x00,0x30,0xC1,0xE0,0x00,0x00,0xC1,0xFF,0x00,0x00,0x00,0x00,0x1F,0x83,
0x80,0x00,0x00,0xE1,0xF8,0x00,0x00,0x00,0x00,0x04,0xC7,0x00,0x00,0x00,0x61,0xC0,
0x00,0x00,0x00,0x00,0x00,0x6E,0x00,0x00,0x00,0x71,0x00,0x00,0x00,0x00,0x00,0x00,
0x3C,0x00,0x00,0x00,0x72,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x00,0x00,0x00,0x3E,
0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x3C,0x00,0x00,0x00,0x00,0x00,
0x00,0x08,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00
};

#define  PROG_STARTX   		10
#define  PROG_STARTY   		35

#define  PROG_HIGH    		10
#define  PROG_LENGTH     	100
/****************************************************************************
* 名称: DispLogin()
* 功能: 闪动显示"Loading..."，并显示进度条。
* 入口参数: 无
* 出口参数: 无
****************************************************************************/
void  DispLogin(void)
{  int  i;
   
   // 显示loding...
   GUI_PutString(30, 25, "Loading...");
   
   // 显示进度条
   GUI_Rectangle( PROG_STARTX, PROG_STARTY, 
                  PROG_STARTX+PROG_LENGTH, PROG_STARTY+PROG_HIGH, disp_color);
   
   // 循环增长进度
   for(i=3; i<100; i+=1)
   {  // 改变进度条显示 
      GUI_RectangleFill( PROG_STARTX, PROG_STARTY, 
                         PROG_STARTX+i, PROG_STARTY+PROG_HIGH, disp_color);
                       
      // 若进度到尾数为5时，则使能"Loading..."显示
      if((i%10) == 5)
      {   GUI_PutString(30, 25, "Loading...");
      }
      
      // 若进度到尾数为0时，则禁止"Loading..."显示
      if((i%10) == 0)
      {   GUI_PutString(30, 25, "          ");
      }
      DELAY5();
      DELAY5();

      DELAY5();
     
DELAY5();
      DELAY5();
   } 
}


/****************************************************************************
* 名称: DispZlgGui()
* 功能: 屏保--再现"ZLG/GUI"。
*       随机位置显示ZLG/GUI的Logo(动画)
* 入口参数: 无
* 出口参数: 无
****************************************************************************/
void  DispZlgGui(void)
{  uint8  x0, y0,i;
   
   x0 = 50;	
   y0 = 50;
   i= 30;

   while(i--)
   {  // 取得随机位置坐标

	  x0 += 10;	
      y0 += 10;

      //x0 = rand()&0x7f;	
      //y0 = rand()&0x3f;
      if(x0 > (GUI_LCM_XMAX))
      {  x0 = 0;
      } 
      if(y0 > (GUI_LCM_YMAX))
      {  y0 = 0;
      } 
   
      // 显示Logo
      GUI_LoadPic( x0, y0, 
                   (uint8 *)LOGO_DAT,
                   88, 29);
      
      // 延时
      DELAY5();
      DELAY5();

      DELAY5();
     
DELAY5();
      DELAY5();
      
      // 清除显示
      GUI_RectangleFill( x0, y0, 
                         x0+88, y0+29, 
                         BACKcolor );
   }
}

/****************************************************************************
* 名称：DemoEllipse()
* 功能：绘画椭圆演示。
* 入口参数：无
* 出口参数：无
****************************************************************************/  
void  DemoEllipse(void)
{   WINDOWS  demow;
   
    /* 显示演示窗口 */
    demow.x = 0;
    demow.y = 0;
    demow.with = 128;
    demow.hight = 64;
    demow.title = (uint8 *) "Ellipse for Demo";
    demow.state = NULL;
    GUI_WindowsDraw(&demow);
   
    /* 显示椭圆 */
    GUI_Ellipse(5, 50, 20, 55, disp_color);	
         DELAY5();
      DELAY5();

      DELAY5();
     
DELAY5();
      DELAY5();
    GUI_Ellipse(55, 75, 20, 55, disp_color);
         DELAY5();
      DELAY5();

      DELAY5();
     
DELAY5();
      DELAY5();
    GUI_EllipseFill(80,120, 25, 50, disp_color);
}





//  主函数（程序入口）
int test_main(void)
{  
    
    // 初始化随机总机
    srand(96169);
    GUI_SetColor(YELLOW,RED); 
    GUI_Initialize();
    
    
    
    DispLogin();        // 显示加载进度条
    GUI_SetColor(BLUE,GREEN);
    GUI_ClearSCR();     // 清屏
    DemoEllipse();
       DELAY5();
      DELAY5();

      DELAY5();
     
DELAY5();
      DELAY5();
	DELAY5();
      DELAY5();

      DELAY5();
     
DELAY5();
      DELAY5();DELAY5();
      DELAY5();

      DELAY5();
     
DELAY5();
      DELAY5();
    GUI_SetColor(orange,RED);
	    GUI_ClearSCR();     // 清屏
        DispZlgGui();       // 显示logo图片
	
    
}


