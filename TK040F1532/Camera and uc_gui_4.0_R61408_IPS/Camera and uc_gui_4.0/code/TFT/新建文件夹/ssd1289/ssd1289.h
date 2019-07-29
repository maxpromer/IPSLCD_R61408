/****************************************************************************
* �ļ�����LCMDRV.H
* ���ܣ�MG12864ͼ��Һ��ģ����������ʹ��LPC2131��GPIO�ڿ��Ʋ�����(ͷ�ļ�)
*       ����ZLG/GUI�û�ͼ�ν��档
*  
*  ���ߣ����ܱ�
*  ���ڣ�2005/3/7
****************************************************************************/
#ifndef  _SSD1289_H_
#define  _SSD1289_H_ 
#include "stm32f10x_lib.h"
//#include "stm32f10x_it.h" 
//#include "config.h"
/* ������ɫ��������(���������ݽṹ) */
#define  TCOLOR	  uint16 		//�����޸ģ�ԭ����	uint8 

/* ����LCM�������� */
#define  GUI_LCM_XMAX		320				//�����޸�			/* ����Һ��x��������� */
#define  GUI_LCM_YMAX		240				//�����޸�
						/* ����Һ��y��������� */


#define   BLACK                0x0000                // ��ɫ��    0,   0,   0 //
#define   BLUE                 0x001F                // ��ɫ��    0,   0, 255 //
#define   GREEN                0x07E0                // ��ɫ��    0, 255,   0 //
#define   CYAN                 0x07FF                // ��ɫ��    0, 255, 255 //
#define   RED                  0xF800                // ��ɫ��  255,   0,   0 //
#define   MAGENTA              0xF81F                // Ʒ�죺  255,   0, 255 //
#define   YELLOW               0xFFE0                // ��ɫ��  255, 255, 0   //
#define   WHITE                0xFFFF                // ��ɫ��  255, 255, 255 //
#define   NAVY                 0x000F                // ����ɫ��  0,   0, 128 //
#define   DGREEN               0x03E0                // ����ɫ��  0, 128,   0 //
#define   DCYAN                0x03EF                // ����ɫ��  0, 128, 128 //
#define   MAROON               0x7800                // ���ɫ��128,   0,   0 //
#define   PURPLE               0x780F                // ��ɫ��  128,   0, 128 //
#define   OLIVE                0x7BE0                // ����̣�128, 128,   0 //
#define   LGRAY                0xC618                // �Ұ�ɫ��192, 192, 192 //
#define   DGRAY                0x7BEF                // ���ɫ��128, 128, 128 //


//Ӳ����ص��Ӻ���
#define Set_Cs  GPIO_SetBits(GPIOD,GPIO_Pin_14);
#define Clr_Cs  GPIO_ResetBits(GPIOD,GPIO_Pin_14);

#define Set_Rs  GPIO_SetBits(GPIOD,GPIO_Pin_13);
#define Clr_Rs  GPIO_ResetBits(GPIOD,GPIO_Pin_13);

#define Set_nWr GPIO_SetBits(GPIOD,GPIO_Pin_12);
#define Clr_nWr GPIO_ResetBits(GPIOD,GPIO_Pin_12);

#define Set_nRd GPIO_SetBits(GPIOD,GPIO_Pin_11);
#define Clr_nRd GPIO_ResetBits(GPIOD,GPIO_Pin_11);

#define Lcd_Light_ON   GPIO_SetBits(GPIOD,GPIO_Pin_10);
#define Lcd_Light_OFF  GPIO_ResetBits(GPIOD,GPIO_Pin_10);

#define Set_Rst GPIO_SetBits(GPIOD,GPIO_Pin_9);
#define Clr_Rst GPIO_ResetBits(GPIOD,GPIO_Pin_9);


#define Lcd_CS_low      GPIO_ResetBits(GPIOD,GPIO_Pin_14)	//Ƭѡ
#define Lcd_CS_high     GPIO_SetBits(GPIOD,GPIO_Pin_14)

#define Lcd_RS_low      GPIO_ResetBits(GPIOD,GPIO_Pin_13)	
#define Lcd_RS_high     GPIO_SetBits(GPIOD,GPIO_Pin_13)

#define Lcd_WR_low      GPIO_ResetBits(GPIOD,GPIO_Pin_12)	//д
#define Lcd_WR_high     GPIO_SetBits(GPIOD,GPIO_Pin_12)	
      
#define Lcd_rd_low      GPIO_ResetBits(GPIOD,GPIO_Pin_11)	//����
#define Lcd_rd_high     GPIO_SetBits(GPIOD,GPIO_Pin_11)	

#define Lcd_res_low      GPIO_ResetBits(GPIOD,GPIO_Pin_9)
#define Lcd_res_high     GPIO_SetBits(GPIOD,GPIO_Pin_9)

typedef unsigned char  uint8;                   /* defined for unsigned 8-bits integer variable 	�޷���8λ���ͱ���  */
typedef signed   char  int8;                    /* defined for signed 8-bits integer variable		�з���8λ���ͱ���  */
typedef unsigned short uint16;                  /* defined for unsigned 16-bits integer variable 	�޷���16λ���ͱ��� */
typedef signed   short int16;                   /* defined for signed 16-bits integer variable 		�з���16λ���ͱ��� */
typedef unsigned int   uint32;                  /* defined for unsigned 32-bits integer variable 	�޷���32λ���ͱ��� */
typedef signed   int   int32;                   /* defined for signed 32-bits integer variable 		�з���32λ���ͱ��� */
typedef float          fp32;                    /* single precision floating point variable (32bits) �����ȸ�������32λ���ȣ� */
typedef double         fp64;                    /* double precision floating point variable (64bits) ˫���ȸ�������64λ���ȣ� */


void ssd1289_Configuration(void);
void ssd1289_LCM_DispIni(void);
uint16 ssd1289_GetPoint(uint16 x,uint16 y);
void ssd1289_Set_ramaddr(uint16 x,uint16 y);
void  ssd1289_GUI_FillSCR(uint16 dat);
void SSD1289_picture_start(void);
//void LCD_SetArea( uint16 x0, uint16 y0, uint16 x1, uint16 y1 );
void  ssd1289_GUI_Fill(uint16 x0,uint16 y0 ,uint16 x1,uint16 y1 ,uint16 clor);
/****************************************************************************
* ���ƣ�GUI_Initialize()
* ���ܣ���ʼ��GUI��������ʼ����ʾ����������ʼ��LCM��������
* ��ڲ�������
* ���ڲ�������
* ˵�����û�����LCM��ʵ�������д�˺�����
****************************************************************************/
extern void  ssd1289_GUI_Initialize(void);


/****************************************************************************
* ���ƣ�GUI_FillSCR()
* ���ܣ�ȫ����䡣ֱ��ʹ�����������ʾ��������
* ��ڲ�����dat		��������
* ���ڲ�������
* ˵�����û�����LCM��ʵ�������д�˺�����
****************************************************************************/
extern void  ssd1289_GUI_FillSCR(TCOLOR dat);


/****************************************************************************
* ���ƣ�GUI_ClearSCR()
* ���ܣ�������
* ��ڲ�������
* ���ڲ�������
* ˵�����û�����LCM��ʵ�������д�˺�����
****************************************************************************/
//extern void  GUI_ClearSCR(void);
#define  ssd1289_GUI_ClearSCR()		ssd1289_GUI_FillSCR(0x00)


/****************************************************************************
* ���ƣ�GUI_Point()
* ���ܣ���ָ��λ���ϻ��㡣
* ��ڲ����� x		ָ���������е�λ��
*            y		ָ���������е�λ��
*            color	��ʾ��ɫ(���ںڰ�ɫLCM��Ϊ0ʱ��Ϊ1ʱ��ʾ)
* ���ڲ���������ֵΪ1ʱ��ʾ�����ɹ���Ϊ0ʱ��ʾ����ʧ�ܡ�(����ʧ��ԭ����ָ����ַ������
*          Ч��Χ)
* ˵�����û�����LCM��ʵ�������д�˺�����
****************************************************************************/
extern uint8  ssd1289_GUI_Point(uint16 x, uint16 y, TCOLOR color);


/****************************************************************************
* ���ƣ�GUI_ReadPoint()
* ���ܣ���ȡָ�������ɫ��
* ��ڲ�����x		ָ���������е�λ��
*           y		ָ���������е�λ��
*           ret     ������ɫֵ��ָ��
* ���ڲ���������0��ʾָ����ַ������������Χ
* ˵�������ڵ�ɫ������ret��d0λΪ1��0��4���Ҷ���Ϊd0��d1��Ч��8λRGB��d0--d7��Ч��
*      RGB�ṹ��R��G��B������Ч��
****************************************************************************/
extern uint8  ssd1289_GUI_ReadPoint(uint16 x, uint16 y, TCOLOR *ret);



/****************************************************************************
* ���ƣ�GUI_HLine()
* ���ܣ���ˮƽ�ߡ�
* ��ڲ����� x0		ˮƽ����������е�λ��
*           y0		ˮƽ����������е�λ��
*           x1      ˮƽ���յ������е�λ��
*           color	��ʾ��ɫ(���ںڰ�ɫLCM��Ϊ0ʱ��Ϊ1ʱ��ʾ)
* ���ڲ�������
* ˵�������ڵ�ɫ��4���Ҷȵ�Һ������ͨ���޸Ĵ˺�����ͼ����ٶȣ��絥ɫLCM������һ�θ�
*      ��8���㣬������Ҫһ����һ�����д��LCM�С�
****************************************************************************/
extern void  ssd1289_GUI_HLine(uint16 x0, uint16 y0, uint16 x1, TCOLOR color);


/****************************************************************************
* ���ƣ�GUI_RLine()
* ���ܣ�����ֱ�ߡ�
* ��ڲ����� x0		��ֱ����������е�λ��
*           y0		��ֱ����������е�λ��
*           y1      ��ֱ���յ������е�λ��
*           color	��ʾ��ɫ
* ���ڲ�������
* ˵�������ڵ�ɫ��4���Ҷȵ�Һ������ͨ���޸Ĵ˺�����ͼ����ٶȣ��絥ɫLCM������һ�θ�
*      ��8���㣬������Ҫһ����һ�����д��LCM�С�
****************************************************************************/
extern void  ssd1289_GUI_RLine(uint16 x0, uint16 y0, uint16 y1, TCOLOR color);




/****************************************************************************
* ���ƣ�GUI_CmpColor()
* ���ܣ��ж���ɫֵ�Ƿ�һ�¡�
* ��ڲ�����color1		��ɫֵ1
*		   color2		��ɫֵ2
* ���ڲ���������1��ʾ��ͬ������0��ʾ����ͬ��
* ˵����������ɫ����TCOLOR�����ǽṹ���ͣ�������Ҫ�û���д�ȽϺ�����
****************************************************************************/
//extern int  GUI_CmpColor(TCOLOR color1, TCOLOR color2);
#define  ssd1289_GUI_CmpColor(color1, color2)	( color1 == color2 )

/****************************************************************************
* ���ƣ�GUI_CopyColor()
* ���ܣ���ɫֵ���ơ�
* ��ڲ�����color1		Ŀ����ɫ����
*		   color2		Դ��ɫ����
* ���ڲ�������
* ˵����������ɫ����TCOLOR�����ǽṹ���ͣ�������Ҫ�û���д���ƺ�����
****************************************************************************/
//extern void  GUI_CopyColor(TCOLOR *color1, TCOLOR color2);
#define  ssd1289_GUI_CopyColor(color1, color2) 	*color1 = color2


#endif
