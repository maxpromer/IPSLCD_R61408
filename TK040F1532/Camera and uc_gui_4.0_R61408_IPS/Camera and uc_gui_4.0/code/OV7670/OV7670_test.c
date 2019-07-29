/* Includes ------------------------------------------------------------------*/
#include "ff.h"	
#include "stm32f10x.h"
#include "stdio.h"
#include "fifo.h"
#include "ov7670.h"
#include "ili9325.h" 
#include "lcm_api.h"
#include "touch.h"
#include "msd.h"
#include "My_FF.h"


vu8 honghwai[3];
vu8 hongwai_lock;
extern vu8  touchIRQ;//�����жϱ�־

extern void display_picture(char *filename);
void LCD_GPIO_OUTPUT(void);
void LCD_GPIO_INPUT(void);


union MY_DATA
{
  vu16 i;
  vu8  a[2];
};

static FIL record_dat;
static u16 pic_counter=0;//��Ƭ������
static void record(void);
static unsigned int get_record(void);



vu8 OV7670_Done_FLAG;//֡ͬ���ź�
vu8 OV7670_LockClose = 1;
vu8 OV7670_F1=0 ,OV7670_F2=0;
vu8 OV7670_take_photo = 0;
	

static const u8 bmpheader[54]={//����bmp�ײ�
	0x42, 0x4D, 0x36, 0x84, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
	0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x84, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

extern void LCD_picture_start(void);//��picdemo.c��
static void Save_as_bmp(void);


//FatFS�õ��ı���
static FATFS fs;            // Work area (file system object) for logical drive
static FIL fdst;      // file objects
static FRESULT res;         // FatFs function common result code
static BYTE buffer[1028];
static UINT bw;
//static BYTE temp[323];

//++++++++++++++++++++++++++++++++++++++++++++++
//bmp�ļ���ʽ������ͷ�ṹ
//++++++++++++++++++++++++++++++++++++++++++++++
typedef __packed struct
{
	u8  pic_head[2];				//1
	u16 pic_size_l;			    //2
	u16 pic_size_h;			    //3
	u16 pic_nc1;				    //4
	u16 pic_nc2;				    //5
	u16 pic_data_address_l;	    //6
	u16 pic_data_address_h;		//7	
	u16 pic_message_head_len_l;	//8
	u16 pic_message_head_len_h;	//9
	u16 pic_w_l;					//10
	u16 pic_w_h;				    //11
	u16 pic_h_l;				    //12
	u16 pic_h_h;				    //13	
	u16 pic_bit;				    //14
	u16 pic_dip;				    //15
	u16 pic_zip_l;			    //16
	u16 pic_zip_h;			    //17
	u16 pic_data_size_l;		    //18
	u16 pic_data_size_h;		    //19
	u16 pic_dipx_l;			    //20
	u16 pic_dipx_h;			    //21	
	u16 pic_dipy_l;			    //22
	u16 pic_dipy_h;			    //23
	u16 pic_color_index_l;	    //24
	u16 pic_color_index_h;	    //25
	u16 pic_other_l;			    //26
	u16 pic_other_h;			    //27
	u16 pic_color_p01;		    //28
	u16 pic_color_p02;		    //29
	u16 pic_color_p03;		    //30
	u16 pic_color_p04;		    //31
	u16 pic_color_p05;		    //32
	u16 pic_color_p06;		    //33
	u16 pic_color_p07;		    //34
	u16 pic_color_p08;			//35			
}BMP_HEAD;




//------------------------------------------------
//��������OV7670��Demo
//------------------------------------------------
void OV7670_main(void)
{
  u32 a;
  char re,home=0,book=0;
  unsigned int x,y;
  char id=-1;
  vu8 lock_0,lock_1,lock_2,light_lock;
  vu16 time_counter;//�صƶ�ʱ��

  Fifo_Init();	   //Fifo���ų�ʼ��

  while(1!=OV7670_init());//��ʼ��ov7670

  OV7670_EXTI_Config();	   //֡�ж�����
  ov7670_Interrupts_Config();	 //�ж����ȼ�����

  HREF_H;
  FIFO_CS_H();//����FIFO���

  OV7670_Done_FLAG = 0;//֡ͬ���ź�

  //�����ļ�ϵͳ
  //f_mount(0, &fs);

  lock_0=1;
  lock_1=1;
  lock_2=1;//����,��¼ͼ��
  light_lock=0;//���ܱ�־
  time_counter=0;//�صƶ�ʱ��
  hongwai_lock=0;

  Del_oneWeekAgo();//ɾ��һ����ǰ����Ƭ
  pic_counter=get_record();//��ȡ��Ƭ��
  while(lock_0)
  { 
//-------------------------------xingxing-----------------------------------

    while(OV7670_Done_FLAG==0);//�ȴ�֡ͬ���ź�
    //HREF_L;//��ֹдFIFO 
    EXTI->IMR &= ~EXTI_LINE_VSYNC_CMOS;//������x�ϵ��ж�����
		  //FIFO_CS_L();//ʹ��FIFO���

    //��λ��FIFO�ĵ�ַ
		  FIFO_RRST_L();		    
	    FIFO_RD_L();
		  FIFO_RD_H();
		  FIFO_RD_L();FIFO_RRST_H();
		  FIFO_RD_H();
		  
		  
          //������һ������	  		   
// 		  FIFO_RD_L();
// 		  FIFO_RD_H();

//++++++++++++++++++++++�صƶ�ʱ��+++++++++++++++++++++
         if(light_lock==0){
		    time_counter++;
		 }

		 if(time_counter>180&&light_lock==0)
		 {
		    light_lock=1;
            Lcd_Light_OFF;
			time_counter=0;
		 }

//+++++++++++++++++++++++++++++++++++++++++++++++++++++
	
		  //Save_as_bmp();
/*
	      if(touchIRQ==1&&light_lock==0) {//�����жϱ�־
		     virtual_LCM_Rectangle_V2(290, 10, 20, 20,0xF800,1); // ������
			 //Save_as_bmp();
			 touchIRQ=0;
		  }else{
		     virtual_LCM_Rectangle_V2(290, 10, 20, 20,0xFFFF,1); // ������
		  }
*/
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>������<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
     x=800;
		 y=800;
		 x= TOUCH_X(); 
		 if(x!=800){
            y= TOUCH_Y();
		 } 

		if(x!=800&&y!=800)//�Ӵ��˴���
		{
		    if(light_lock==1)
					{//��������,���ѹ�
						Lcd_Light_ON;//����
						light_lock=0;
						time_counter=0;
           }
	    }

	if(x!=800&&y!=800)//�Ӵ��˴���
	{

		 time_counter=0;

         //>>>>>>>>>>>>>>>>>>>>>>>home��ť<<<<<<<<<<<<<<<<<<<<<<<<<
	  	 re=IsPressed(x, y,280,200,40,40,&home);//home
		 if(re==3&&light_lock==0)//�������ڳ���,��δ��
         {
					lock_0=0;
         }

         //>>>>>>>>>>>>>>>>>>>>>>>>>>book��ť<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
         //if(light_lock==0)
         {
	         re=IsPressed(x, y,280,0,40,40,&book);//book  

	   	 	 if(re==3&&light_lock==0)//�������ڳ���,��δ��
             {
               lock_1=1;			   	 
			   display_picture("sys2.bmp"); 
			   while(lock_1)
               {
	             main_menu(&id,2);//�ڶ����˵�
	  			 switch(id)
	             {
					case 0:
             			//Lcd_Clear(0x001F);
                        lock_2=0;//������,����¼ͼ��
		        		break;

					case 1:
             			//Lcd_Clear(0xFFE0);
						lock_2=1;//����,��¼ͼ��
                        //GPIO_ResetBits(GPIOB,GPIO_Pin_2);
		        		break;

					case 2:
             			Lcd_Clear(0x03E0);
									light_lock=1;
									Lcd_Light_OFF;
									lock_1=0;
		        		break;

					case 3:
					   
                        lock_1=0;
		        		break;

					default:
		        		break;
      			}//switch  
               }//while(lock_1)               
	     	 }else if(re==3&&light_lock==1){//�������ڳ���,���ѹ�
             	             
                    Lcd_Light_ON;//����
                    light_lock=0;
					time_counter=0;
             }
           }


	}
	//���ͺ�����
          //if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==1)hongwai_lock=1;
		  //else 	 hongwai_lock=0;

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	  
		  // lock_2==1||&&hongwai_lock==1
          //if(cnt<1800) {						   
              //cnt++;
       if(x<200)
			  {
			  //if(cnt%18==0&&(lock_2==1&&hongwai_lock==1)){
				 Save_as_bmp();
         record();
				 
				 if(pic_counter>300&&pic_counter%100==0){
				   Del_some_bmp(pic_counter-400,pic_counter-300);
				 }
				 
         }
          //}

//       if(light_lock==0)//�ǽ���״̬
//       {   		  
//		  virtual_LCM_PutString(180,10,"     ",0x0000,0xffff ,1);
//          virtual_LCM_PutNums(180,10,cnt,0x0000,0xffff ,1);
// 	   }
		  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

          //��λ��FIFO�ĵ�ַ
		  FIFO_RRST_L();		    
	    FIFO_RD_L();
		  FIFO_RD_H();
		  FIFO_RD_L();
			FIFO_RRST_H();
		  FIFO_RD_H();
		  
		  
          //������һ������	  		   
// 		  FIFO_RD_L();
// 		  FIFO_RD_H();

       if(light_lock==0)//�ǽ���״̬
       {   		  
		  LCD_Scan_Set(2);		  
      LCD_GPIO_INPUT();
		  FIFO_CS_L();
		  Clr_Cs;
          //д��
          for( a = 0;a < 320*240; a++)
          {
		   FIFO_RD_L();	
		   Clr_nWr;
	     Set_nWr;
		   FIFO_RD_H();

		   FIFO_RD_L();
		   Clr_nWr;
		   Set_nWr; 
		   FIFO_RD_H(); 
          }
         Set_Cs;
		 FIFO_CS_H();
         LCD_GPIO_OUTPUT();
		 LCD_Scan_Set(0);		  
       	}
	   FIFO_WRST_L();//��λдFIFO�ĵ�ַ
   	 FIFO_CS_H();//FIFO����� 
 		  FIFO_RD_L();
 		  FIFO_RD_H();
//------------------------------------------------------------------------------
	   OV7670_Done_FLAG=0;
	   EXTI_ClearITPendingBit(EXTI_LINE_VSYNC_CMOS);//����жϱ�־λ
     EXTI->IMR |= EXTI_LINE_VSYNC_CMOS;//������x�ϵ��ж����� 
     while(OV7670_Done_FLAG==0);//�ȴ���һ֡�Ŀ�ʼ�ź�
     HREF_H;//ʹ��FIFOд
	   FIFO_WRST_H();//���д��ַ��λ�ź�       
	   OV7670_Done_FLAG=0;
  }
  EXTI->IMR &= ~EXTI_LINE_VSYNC_CMOS;//������x�ϵ��ж�����
  HREF_L;//��ֹдFIFO 
  FIFO_CS_H();//����FIFO���
  RCC_MCOConfig(RCC_MCO_NoClock);//hsi	   //ָ�����������ʱ��Դ PA8
  FIFO_Set_GPIO_Config_END();  

}




//+++++++++++++++++++++++++++++++++++++++++++++++
//������ƬΪbmp�ļ�
//+++++++++++++++++++++++++++++++++++++++++++++++
#define	 DBG_BMP_SAVE  	0

static void Save_as_bmp(void)
{ 
  u16 ReadValue; 
  s32 tx,ty;
  u8 R,G,B;
  char s[30];
  //�����ļ�ϵͳ
  f_mount(0, &fs);

  pic_counter++;

  virtual_LCM_PutNums(10,10,pic_counter,0x0000,0xffff ,1);
  sprintf(s, "pic%d.bmp",pic_counter);
  //�������ļ�
  res = f_open(&fdst, s, FA_CREATE_ALWAYS | FA_WRITE);
  if(res!=FR_OK){
     f_mount(0, NULL); 
	 pic_counter--;
     MSD_Init();//���³�ʼ��SD��
	 return;
  }

  //����bmpͷ
  res = f_write(&fdst, bmpheader, 54, &bw);//д��BMP�ײ�
          
  Set_Cs;
  LCD_GPIO_INPUT();
  FIFO_CS_L(); 

  // Copy source to destination
	 for (ty = 0; ty < 240; ty++)
     {
		  for(tx=319;tx>=0;tx--)
          { 	     	  		       
		   FIFO_RD_L();	
		   ReadValue = ((GPIO_ReadInputData(GPIOB))&0XFF00);	   //��fifoһ����λ    	  ��λ
		   FIFO_RD_H();

		   FIFO_RD_L();
		   ReadValue |= ((GPIO_ReadInputData(GPIOB)>>8)&0X00FF);	   //��fifoһ����λ    	  ��λ
		   FIFO_RD_H(); 
		 
           R = (ReadValue>>8)&0xF8;
           G = (ReadValue>>3)&0xFC;
           B = (ReadValue<<3)&0xF8;
		   buffer[tx*3+2] = R;
		   buffer[tx*3+1] = G;
		   buffer[tx*3+0] = B;
		  }

        res = f_write(&fdst, buffer, 960, &bw);
     }

  FIFO_CS_H();
  LCD_GPIO_OUTPUT();

  f_close(&fdst);

  // Unregister a work area before discard it
  f_mount(0, NULL);

}


//��¼��ǰ�Ѵ���sd������Ƭ����
static void record(void)
{
    union MY_DATA temp;
	    
    //�����ļ�ϵͳ
    f_mount(0, &fs);

	res = f_open(&record_dat,"record.dat", FA_CREATE_ALWAYS | FA_WRITE); //�����ļ�
    if(res==FR_OK){
		temp.i=pic_counter;
        res = f_write(&record_dat,&temp, sizeof(union MY_DATA), &bw);
        f_close(&record_dat);

        // Unregister a work area before discard it
        f_mount(0, NULL);

		//printf("record=%d",temp.i);
    }else{
        f_close(&record_dat);
        // Unregister a work area before discard it
        f_mount(0, NULL);
		MSD_Init();//���³�ʼ��SD��
    }
}


//������ǰ�Ѵ���sd������Ƭ����
static unsigned int get_record(void)
{
    union MY_DATA temp;
	  
    //�����ļ�ϵͳ
    f_mount(0, &fs);

    res = f_open(&record_dat, "record.dat", FA_OPEN_EXISTING | FA_READ);	 //���ļ�
    if(res!=FR_OK){
        f_close(&record_dat);
	   	res = f_open(&record_dat,"record.dat", FA_CREATE_ALWAYS | FA_WRITE); //�����ļ�
        if(res==FR_OK){
            temp.i=0;
            res = f_write(&record_dat,&temp, sizeof(union MY_DATA), &bw);
            f_close(&record_dat);

            // Unregister a work area before discard it
            f_mount(0, NULL);
			//printf("get_record_1=%d",temp.i);
            return temp.i;
        }else{
        	f_close(&record_dat);
        	// Unregister a work area before discard it
        	f_mount(0, NULL);
			MSD_Init();//���³�ʼ��SD��
        }

    }else if(res==FR_OK){
        res = f_read(&record_dat, &temp, sizeof(union MY_DATA), &bw);
        f_close(&record_dat);

  		// Unregister a work area before discard it
  		f_mount(0, NULL);
		//printf("get_record_2=%d",temp.i);
        return temp.i;
    }
}


void LCD_GPIO_OUTPUT(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void LCD_GPIO_INPUT(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
