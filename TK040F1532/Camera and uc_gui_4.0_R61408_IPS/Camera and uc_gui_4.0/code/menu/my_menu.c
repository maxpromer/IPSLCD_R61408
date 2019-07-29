//*************************************************
//���ߣ�XING
//�����������˵���ʾ
//����ʱ�䣺2010/11/08  10:15
//����޸�ʱ�䣺
//*************************************************
/*
  �޸���ʷ:
*/
#include "my_menu.h"
#include "gui.h"
#include "ili9325.h"
#include "lcm_api.h"
#include "touch.h"
#include "stdio.h"
#include "My_FF.h"
//#include "ff.h"
#include "calendar.h"
#include "Font_lib_SD.h"
  
#define  M_X   25  
#define  M_Y   55
#define  M_L   125
#define  M_H   50  
#define  M_DX  (320-M_L*2-M_X*2)
#define  M_DY  (240-M_H*2-M_Y*2)   
 FILINFO Finfo;     
extern vu8 touchIRQ;//�����жϱ�־
extern void Delay(u32 nTime);
extern char display_picture(char *filename);
extern char display_button_picture(char *filename);
extern char display_partation_picture(char *filename,u16 x0,u16 y0,u16 pic_x,u16 pic_y,u16 pic_L,u16 pic_H);
extern void Time_Show(u16 x,u16 y);
//*************************************************
//�������� :  main_menu
//�������� :  ��ʾ���˵�
//������� : 
//������� : 
//����ֵ   : 
//*************************************************
#define DBG_main_menu  0

void main_menu(char* id,char menu)
{
    vu16 x,y;
	
	char pressed=0,pressed_1=0,pressed_2=0,pressed_3=0;//�������µı�־
	char re,book=0;
    vu16 time_counter;//�صƶ�ʱ��
    vu8  light_lock,lock;
	//��ʾ��ť
    virtual_LCM_Rectangle_V2(M_X-1, M_Y-1, M_L, M_H,0xffff,0); // ������
    virtual_LCM_Rectangle_V2(M_X-1, M_Y+M_H+M_DY-1, M_L, M_H,0xffff,0); // ������
    virtual_LCM_Rectangle_V2(M_X+M_L+M_DX-1, M_Y-1, M_L, M_H,0xffff,0); // ������
    virtual_LCM_Rectangle_V2(M_X+M_L+M_DX-1, M_Y+M_H+M_DY-1, M_L, M_H,0xffff,0); // ������

    virtual_LCM_Rectangle_V2(M_X, M_Y, M_L, M_H,0x7BEF,1); // ������
    virtual_LCM_Rectangle_V2(M_X, M_Y+M_H+M_DY, M_L, M_H,0x7BEF,1); // ������
    virtual_LCM_Rectangle_V2(M_X+M_L+M_DX, M_Y, M_L, M_H,0x7BEF,1); // ������
    virtual_LCM_Rectangle_V2(M_X+M_L+M_DX, M_Y+M_H+M_DY, M_L, M_H,0x7BEF,1); // ������

    if(menu==1)//һ���˵�
    {
	    display_button_picture("sys8.bmp");
        //����

		f_mount(0, &fs);	
		f_open(&fsrc,"st1616.bin", FA_OPEN_EXISTING | FA_READ);		
		Lcd_WriteChineseStringClarity(M_X+M_L/2-16,M_Y+M_H/2-8,0xf800,0,"ϵͳ",0); 
		Lcd_WriteChineseStringClarity(M_X+M_L+M_DX+M_L/2-16,M_Y+M_H/2-8,0xf800,0,"���",0);
		Lcd_WriteChineseStringClarity(M_X+M_L/2-12,M_Y+M_H+M_DY+M_H/2-8,0xf800,0,"GPS",0);
		Lcd_WriteChineseStringClarity(M_X+M_L+M_DX+M_L/2-16,M_Y+M_H+M_DY+M_H/2-8,0xf800,0,"����",0);

    }

    if(menu==2)//�����˵�1
    {	
	    display_button_picture("sys7.bmp");

		//����
		f_mount(0, &fs);	
		f_open(&fsrc,"st1616.bin", FA_OPEN_EXISTING | FA_READ);
		Lcd_WriteChineseStringClarity(M_X+M_L/2-16,M_Y+M_H/2-8,0xf800,0,"����",0); 
		Lcd_WriteChineseStringClarity(M_X+M_L+M_DX+M_L/2-16,M_Y+M_H/2-8,0xf800,0,"����",0);
		Lcd_WriteChineseStringClarity(M_X+M_L/2-16,M_Y+M_H+M_DY+M_H/2-8,0xf800,0,"����",0);
		Lcd_WriteChineseStringClarity(M_X+M_L+M_DX+M_L/2-16,M_Y+M_H+M_DY+M_H/2-8,0xf800,0,"�˳�",0);
	}

    if(menu==3)//�����˵�2
    {
		display_button_picture("sys12.bmp");

		//����
		f_mount(0, &fs);	
		f_open(&fsrc,"st1616.bin", FA_OPEN_EXISTING | FA_READ);
		Lcd_WriteChineseStringClarity(M_X+M_L/2-32,M_Y+M_H/2-8,0xf800,0,"ɾ��ȫ��",0); 
		Lcd_WriteChineseStringClarity(M_X+M_L+M_DX+M_L/2-32,M_Y+M_H/2-8,0xf800,0,"ʱ������",0);
		Lcd_WriteChineseStringClarity(M_X+M_L+M_DX+M_L/2-16,M_Y+M_H+M_DY+M_H/2-8,0xf800,0,"�˳�",0);
		Lcd_WriteChineseStringClarity(M_X+M_L/2-28,M_Y+M_H+M_DY+M_H/2-18,0xf800,0,"��C/GUI",0);
		Lcd_WriteChineseStringClarity(M_X+M_L/2-24,M_Y+M_H+M_DY+M_H/2+ 2,0xf800,0,"��ϰ��",0);
    }

  time_counter=0;//�صƶ�ʱ��
	light_lock=0;//���ܱ�־
	lock=1;
	f_mount(0, &fs);	
	f_open(&fsrc,"st1616.bin", FA_OPEN_EXISTING | FA_READ);
	GUI_SetFont(&GUI_Font8x16);
	while(lock)
	{
		Delay(5);
	    if(menu==1)//һ���˵�
		Time_Show(72,200); 

// 		RTC_SetAlarm(RTC_GetCounter()+4); 
// 		RTC_WaitForLastTask();
// 		Sys_Enter_StopMode();
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>������<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
     x=800;
		 y=800;
		 x= TOUCH_X(); 
		 if(x!=800)y=TOUCH_Y();
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	
	//-----------------------------LCD�ƿ���---------------------------------
			 if(time_counter<1800) time_counter++;
			 else {
				 	Lcd_Light_OFF;
				    while(1)
					{
					    Delay(4);
						x=800;
				    	y=800;
				    	x= TOUCH_X(); 
				    	if(x!=800) y= TOUCH_Y();	
					 	if(x!=800&&y!=800&&((y>=M_Y+M_H+M_DY+M_H+10)||y<=M_Y-10))//�Ӵ��˴���
		            	{
						    time_counter=0;
							Lcd_Light_ON;
				  			break;
		            	}
					 }		 
			 		}
	//++++++++++++++++++++++�صƶ�ʱ��+++++++++++++++++++++
			 if(x!=800&&y!=800)//�Ӵ��˴���
	         {
		       time_counter=0;
	         }
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++
		if(light_lock==0)
	    {	
		   re=IsPressed(x, y,M_X, M_Y, M_L, M_H,&pressed);//����0  
		   if(re==0){//����Ķ�������
		      *id=0;
		   	  lock=0;
			  break;
		   } 
		      
		   re=IsPressed(x, y,M_X+M_L+M_DX, M_Y, M_L, M_H,&pressed_1);//����1 
		   if(re==0){//����Ķ�������
		      *id=1;
		   	  lock=0;
			  break;
		   }    
	    
	       re=IsPressed(x, y,M_X, M_Y+M_H+M_DY, M_L, M_H,&pressed_2);//����2     
		   if(re==0){//����Ķ�������
		      *id=2;
		   	  lock=0;
			  break;
		   }    
	
	       re=IsPressed(x, y,M_X+M_L+M_DX, M_Y+M_H+M_DY, M_L, M_H,&pressed_3);//����3     
		   if(re==0){//����Ķ�������
		      *id=3;
		   	  lock=0;
			  break;
		   }  
		 }
    }  	 
	return;
}

//*************************************************
//�������� :  scan_menu
//�������� :  ���ͼƬ�˵�
//������� : 
//������� : 
//����ֵ   : 
//*************************************************

//��ť
#define  B_X   10  
#define  B_Y   (240-10-25)
#define  B_L   50
#define  B_H   25  
#define  B_DX  10

//ͼ��
#define  WX   10  
#define  WY   5
#define  WL   20
#define  WH   20  
#define  WDX  10
#define  WDY  13


static char filePressed[12];//�ļ�������¼   
char file_num;//ѡ�е��ļ����
extern DIR start_dirs; //Ŀ¼�����Ϣ 
unsigned int page;//ҳ��
FILE_NAME_LIT File_list[12];

void scan_menu(char* id)
{
    vu16 time_counter;//�صƶ�ʱ��
    unsigned int cnt;
    char pressed=0,pressed_1=0,pressed_2=0,pressed_3=0;//�������µı�־
	char re,home=0;
    vu16 x,y;
    vu8  light_lock,lock,lock_scan;
	u8 i;

  page = 1;
  lock_scan=1; 

loop_A:

  while(lock_scan)
  {

	//ssd1289_GUI_FillSCR(GREEN);
	display_picture("sys3.bmp");
 
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!��ʾ��ť!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    virtual_LCM_Rectangle_V2(B_X-1, B_Y-1, B_L, B_H,0xffff,0); // ������
    virtual_LCM_Rectangle_V2(B_X+B_L*1+B_DX*1-1, B_Y-1, B_L, B_H,0xffff,0);  // ������
    virtual_LCM_Rectangle_V2(B_X+B_L*2+B_DX*2-1, B_Y-1, B_L, B_H,0xffff,0); // ������
    virtual_LCM_Rectangle_V2(B_X+B_L*3+B_DX*3-1, B_Y-1, B_L, B_H,0xffff,0); // ������
	virtual_LCM_Rectangle_V2(B_X+B_L*4+B_DX*4-1, B_Y-1, B_L, B_H,0xffff,0); // ������

    virtual_LCM_Rectangle_V2(B_X, B_Y, B_L, B_H,0x7BEF,1); // ������
    virtual_LCM_Rectangle_V2(B_X+B_L*1+B_DX*1, B_Y, B_L, B_H,0x7BEF,1); // ������
    virtual_LCM_Rectangle_V2(B_X+B_L*2+B_DX*2, B_Y, B_L, B_H,0x7BEF,1); // ������
    virtual_LCM_Rectangle_V2(B_X+B_L*3+B_DX*3, B_Y, B_L, B_H,0x7BEF,1); // ������
	virtual_LCM_Rectangle_V2(B_X+B_L*4+B_DX*4, B_Y, B_L, B_H,0x7BEF,1); // ������//home

	GUI_SetBkColor(0x787c78);
    GUI_SetColor(0x000000);
	GUI_DispCharAt('<',B_X+B_L/2-4, B_Y+B_H/2-8);
	GUI_DispCharAt('>',B_X+B_L*1+B_DX*1+B_L/2-4, B_Y+B_H/2-8);
	GUI_DispStringAt("Del" ,B_X+B_L*2+B_DX*2+B_L/2-4-8, B_Y+B_H/2-8);
	GUI_DispStringAt("OK"  ,B_X+B_L*3+B_DX*3+B_L/2-4-8, B_Y+B_H/2-8);
	GUI_DispStringAt("Home",B_X+B_L*4+B_DX*4+B_L/2-4-8, B_Y+B_H/2-8);

    //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$��ʾͼ�꼰�ļ���$$$$$$$$$$$$$$$$$$$$$$$$$
    for(i=0;i<12;i++){
        filePressed[i]=0;
    }

    for(i=0;i<6;i++){
        virtual_LCM_Rectangle_V2(WX, WY+WH*i+WDY*i, WL, WH,RED,1); // ������
	    //virtual_LCM_PutString(WX+35, WY+WH*i+WDY*i,"pic1.bmp",0x0000,0xffff,1); 

        virtual_LCM_Rectangle_V2(WX+170, WY+WH*i+WDY*i, WL, WH,RED,1); // ������
	    //virtual_LCM_PutString(WX+170+35, WY+WH*i+WDY*i,"pic7.bmp",0x0000,0xffff,1); 
    }


    //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    lock=1;
	file_num=0;
	virtual_LCM_Rectangle_V2(WX, WY,20,20,BLUE,0); //Ĭ��ѡ�е�һ���ļ�

//-----------------------------------------------------------
  time_counter=0;//�صƶ�ʱ��  

  cnt = List_files("\\",&start_dirs,page,File_list);
  List_name(cnt);
//-----------------------------------------------------------
loop_B:

    while(lock)
    {
	     Delay(4);

//-----------------------------LCD�ƿ���---------------------------------
		 if(time_counter<1800)
		 {
			time_counter++;
		 }
		 else {
		 	Lcd_Light_OFF;
			
		    while(1)
			{
			    Delay(4);
				x=800;
		    	y=800;
		    	x= TOUCH_X(); 
		    	if(x!=800){
               		y= TOUCH_Y();
		    	}
			 
		    	if(x!=800&&y<=200)//�Ӵ��˴���
            	{
					    time_counter=0;
						Lcd_Light_ON;
			  			break;
            	}

			}//while		 
		 }



//-----------------------------------------------------------------------


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>������<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
         x=800;
		 y=800;
		 x= TOUCH_X(); 
		 if(x!=800){
            y= TOUCH_Y();
		 }
		 
		 if(x!=800&&y!=800)//�Ӵ��˴���
         {
	       time_counter=0;
         }
 
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	   re=IsPressed(x, y,B_X+B_L*4+B_DX*4, B_Y, B_L, B_H,&home);//����OK
	   //re=IsPressed(x, y,326,201,16,4,&home);//home  
	   if(re==3){
	      *id=0;
		  break;
	   } 


	   re=IsPressed(x, y,B_X, B_Y, B_L, B_H,&pressed);//����<  
	   if(re==3){
	      *id=1;
	   	  lock=0;
		  break;
	   } 
	      
	   re=IsPressed(x, y,B_X+B_L*1+B_DX*1, B_Y, B_L, B_H,&pressed_1);//����> 
	   if(re==3){
	      *id=2;
	   	  lock=0;
		  break;
	   }    
    
       re=IsPressed(x, y,B_X+B_L*2+B_DX*2, B_Y, B_L, B_H,&pressed_2);//����Del     
	   if(re==3){
	      *id=3;
	   	  lock=0;
		  break;
	   }    

       re=IsPressed(x, y,B_X+B_L*3+B_DX*3, B_Y, B_L, B_H,&pressed_3);//����OK     
	   if(re==3){
	      *id=4;
	   	  lock=0;
		  break;
	   }  

    //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$�꼰�ļ���$$$$$$$$$$$$$$$$$$$$$$$$$
    for(i=0;i<6;i++){
        re=IsPressed_V2(x, y,WX, WY+WH*i+WDY*i, 130, WH,&filePressed[i]);  
	    if(re==3){
          if(file_num!=i&&file_num>=0){//֮ǰ����ѡ���Լ�
            if(file_num<6){//�����
			   virtual_LCM_Rectangle_V2(WX, WY+WH*file_num+WDY*file_num,20,20,RED,0); //ѡ��ʧЧ
            }else{//�ұ���
			   virtual_LCM_Rectangle_V2(WX+170, WY+WH*(file_num-6)+WDY*(file_num-6),20,20,RED,0); //ѡ��ʧЧ
            }
             file_num=i;
          }

		  break;
	    } 
//Program Size: Code=91848 RO-data=26144 RW-data=888 ZI-data=17000  
        re=IsPressed_V2(x, y,WX+170, WY+WH*i+WDY*i,130,WH,&filePressed[i+6]);   
	    if(re==3){
          if(file_num!=i+6&&file_num>=0){//֮ǰ����ѡ���Լ�
            if(file_num<6){//�����
			   virtual_LCM_Rectangle_V2(WX, WY+WH*file_num+WDY*file_num,20,20,RED,0); //ѡ��ʧЧ
            }else{//�ұ���
			   virtual_LCM_Rectangle_V2(WX+170, WY+WH*(file_num-6)+WDY*(file_num-6),20,20,RED,0); //ѡ��ʧЧ
            }
             file_num=i+6;
          }

		  break;
	    }   
    }
    //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


    }//while(1)
   
   switch(*id)
   {
	  case 0://home
			   lock_scan=0;
		      break;

	  case 1://<
               page-=1;
               if(page<=0) page=1; 
  			   cnt = List_files("\\",&start_dirs,page,File_list);
  			   List_name(cnt);
			   *id=-1;
			   lock=1;
               goto loop_B;
	        break;

	  case 2://>
               page+=1;
  			   cnt = List_files("\\",&start_dirs,page,File_list);
               if(!cnt){
        		 page-=1;
  			     cnt = List_files("\\",&start_dirs,page,File_list);
                 List_name(cnt);
               }else{
				 List_name(cnt);
               }
			   *id=-1;
			   lock=1;
               goto loop_B;
	        break;

	  case 3://Del
               Del_files(File_list[file_num].name);
  			   cnt = List_files("\\",&start_dirs,page,File_list);
               List_name(cnt);
			   *id=-1;
			   lock=1;
               goto loop_B;
	        break;

	  case 4://OK
			if( strstr(File_list[file_num].name,"BMP"))
		      if(!display_picture(File_list[file_num].name))
			  {
			    *id=-1;
			    lock_scan=1;
			    goto loop_A;
              }	  
		
			if( strstr(File_list[file_num].name,"TXT") )	  //Finfo.fname
			  	{
				//LCD_X=0,LCD_Y=0;	
				Ebook(File_list[file_num].name);
				} 
			  while(1)
			  {
			    Delay(4);
				x=800;
		    	y=800;
		    	x= TOUCH_X(); 
		    	if(x!=800){
               		y= TOUCH_Y();
		    	}
			 
		    	if(x!=800&&y!=800){//�Ӵ��˴���	 
					break;
				}

		 		if(time_counter<1800*3)
		 		{
					time_counter++;
		 		}
		 		else {
		      	
                  Lcd_Light_OFF;
                  while(1)
			      {
			        Delay(4);
		 			
					x=800;
		    		y=800;
		    		x= TOUCH_X(); 
		    		if(x!=800){
               			y= TOUCH_Y();
		    		}
			 
		    		if(x!=800&&y!=800){//�Ӵ��˴���
                        Lcd_Light_ON;
						time_counter=0;
                        Delay(40);	 
						break;
					}
				   }//while(1)
				  }//else
			   }//while(1)
			   *id=-1;
			   lock_scan=1;
			   goto loop_A;
			   
	        break;

	  default:
              *id=-1;
	        break;
   }   
 }

}

//��ʾ�оٳ������ļ���
void List_name(unsigned int cnt)
{
	unsigned char i;
	unsigned char *p;
   f_mount(0, &fs);
   f_open(&fsrc,"st1616.bin", FA_OPEN_EXISTING | FA_READ);
    for(i=0;i<6;i++){
		Lcd_ColorBox(WX+35,WY+WH*i+WDY*i,8*12,16,0xffff);
		Lcd_ColorBox(WX+170+35,WY+WH*i+WDY*i,8*12,16,0xffff);
    }
    for(i=0;i<6;i++){
      if(i<cnt){
	  	p = File_list[i].name;
		Lcd_WriteChineseStringClarity(WX+35,WY+WH*i+WDY*i,0,0,p,0);
	  }
      if(i+6<cnt){
	    p = File_list[i+6].name;
		Lcd_WriteChineseStringClarity(WX+170+35,WY+WH*i+WDY*i,0,0,p,0);
	  }
    }
	f_mount(0, NULL);
}


#define  s_X  80
#define  s_Y  60
static char Time_Date[6];//�ļ�������¼   
#define  B_X   10  
#define  B_Y   (240-10-25)
#define  B_L   67
#define  B_H   25  
#define  B_DX  10

void Set_Time_Date(void)
{
	char re,i;
    vu16 x,y;
    char pressed=0,pressed_1=0,pressed_2=0,pressed_3=0,home=0;//�������µı�־
    char slect=0;
	u16 year=2013;
	u8 month=5,day=1,hour=10,min=0,sec=0;
    vu16 time_counter=0;//�صƶ�ʱ��

	for(i=0;i<6;i++){
        Time_Date[i]=0;
    }

	
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!��ʾ��ť!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    virtual_LCM_Rectangle_V2(B_X-1, B_Y-1, B_L, B_H,0xffff,0); // ������
    virtual_LCM_Rectangle_V2(B_X+B_L*1+B_DX*1-1, B_Y-1, B_L, B_H,0xffff,0);  // ������
    virtual_LCM_Rectangle_V2(B_X+B_L*2+B_DX*2-1, B_Y-1, B_L, B_H,0xffff,0); // ������
    virtual_LCM_Rectangle_V2(B_X+B_L*3+B_DX*3-1, B_Y-1, B_L, B_H,0xffff,0); // ������

    virtual_LCM_Rectangle_V2(B_X, B_Y, B_L, B_H,0x7BEF,1); // ������
    virtual_LCM_Rectangle_V2(B_X+B_L*1+B_DX*1, B_Y, B_L, B_H,0x7BEF,1); // ������
    virtual_LCM_Rectangle_V2(B_X+B_L*2+B_DX*2, B_Y, B_L, B_H,0x7BEF,1); // ������
    virtual_LCM_Rectangle_V2(B_X+B_L*3+B_DX*3, B_Y, B_L, B_H,0x7BEF,1); // ������

	GUI_SetBkColor(0x787c78);
  GUI_SetColor(0x000000);
	GUI_DispCharAt('-',B_X+B_L/2-4, B_Y+B_H/2-8);
	GUI_DispCharAt('+',B_X+B_L*1+B_DX*1+B_L/2-4, B_Y+B_H/2-8);
	GUI_DispStringAt("Set" ,B_X+B_L*2+B_DX*2+B_L/2-4-8, B_Y+B_H/2-8);
	GUI_DispStringAt("Back",B_X+B_L*3+B_DX*3+B_L/2-2-8, B_Y+B_H/2-8);
	
	GUI_SetFont(&GUI_Font24_ASCII);
	GUI_SetBkColor(GUI_WHITE);
  GUI_SetColor(GUI_RED);
	//Program Size: Code=96472 RO-data=29776 RW-data=936 ZI-data=17040  
	GUI_DispDecAt(year, s_X+8, s_Y+4, 4);
	GUI_DispCharAt('/',s_X+16*4+4,s_Y+4);
	GUI_DispDecAt(month, s_X+16*5+4, s_Y+4, 2);
	GUI_DispCharAt('/',s_X+16*7+4,s_Y+4);
	GUI_DispDecAt(day, s_X+16*8+4, s_Y+4, 2);
	
	GUI_DispDecAt(hour,s_X+16*1+4,s_Y+64, 2);
	GUI_DispCharAt(':',s_X+16*3+4,s_Y+64);
	GUI_DispDecAt(min, s_X+16*4+4,s_Y+64, 2);
	GUI_DispCharAt(':',s_X+16*6+4,s_Y+64);
	GUI_DispDecAt(sec, s_X+16*7+4,s_Y+64, 2);

	slect=0;
	LCD_Scan_Set(1);
	f_mount(0, &fs);
	f_open(&fsrc,"st1616.bin", FA_OPEN_EXISTING | FA_READ);
	virtual_LCM_Rectangle_V2(s_X-1,s_Y-1,16*4+1, 32+1,BLUE,0); //ѡ��

  while(1)
  {
		GUI_SetFont(&GUI_Font24_ASCII);
		GUI_DispDecAt(year, s_X+8, s_Y+4, 4);
		GUI_DispCharAt('/',s_X+16*4+4,s_Y+4);
		GUI_DispDecAt(month, s_X+16*5+4, s_Y+4, 2);
		GUI_DispCharAt('/',s_X+16*7+4,s_Y+4);
		GUI_DispDecAt(day, s_X+16*8+4, s_Y+4, 2);
//     virtual_LCM_PutNums_32x16(s_X,s_Y,year,"%0.4d",RED,WHITE,1);//��
//     virtual_LCM_PutNums_32x16(s_X+16*5,s_Y,month,"%0.2d",RED,WHITE,1);//��
//     virtual_LCM_PutNums_32x16(s_X+16*8,s_Y,day,"%0.2d",RED,WHITE,1);//��
// 		virtual_LCM_PutNums_32x16(16+s_X,s_Y+60,hour,"%0.2d",RED,WHITE,1);//ʱ
//     virtual_LCM_PutNums_32x16(16+s_X+16*3,s_Y+60,min,"%0.2d",RED,WHITE,1);//��
//     virtual_LCM_PutNums_32x16(16+s_X+16*6,s_Y+60,sec,"%0.2d",RED,WHITE,1);//��

	GUI_DispDecAt(hour,s_X+16*1+4,s_Y+64, 2);
	GUI_DispCharAt(':',s_X+16*3+4,s_Y+64);
	GUI_DispDecAt(min, s_X+16*4+4,s_Y+64, 2);
	GUI_DispCharAt(':',s_X+16*6+4,s_Y+64);
	GUI_DispDecAt(sec, s_X+16*7+4,s_Y+64, 2);
	Delay(4);
	LCD_Scan_Set(1);
	Time_Show(79,168);
	
//-----------------------------LCD�ƿ���---------------------------------
		 if(time_counter<1000)
		 {
			time_counter++;
		 }
		 else {
		 	Lcd_Light_OFF;
			
		    while(1)
			{
			    Delay(4);
				x=800;
		    	y=800;
		    	x= TOUCH_X(); 
		    	if(x!=800){
               		y= TOUCH_Y();
		    	}
			 
		    	if(x!=800&&y<=200)//�Ӵ��˴���
            	{
				    
	          		//re=IsPressed(x, y,326,106, 342-326, 130-106,&book);//book  
	          		//if(re==3){//�������ڳ���
					    time_counter=0;
						Lcd_Light_ON;
			  			break;
			  		//}
            	}

			}//while		 
		 }


     x=800;
		 y=800;
		 x= TOUCH_X(); 
		 if(x!=800){
            y= TOUCH_Y();
		 }
		 
		 if(x!=800&&y!=800)//�Ӵ��˴���
         {
	       time_counter=0;
         }

//-----------------------------------------------------------------------



    if(slect!=0)  virtual_LCM_Rectangle_V2(s_X-1,s_Y-1,16*4+1, 32+1,WHITE,0); //ѡ��ʧЧ

    if(slect!=1)  virtual_LCM_Rectangle_V2(s_X+16*5-1,s_Y-1,16*2+1, 32+1,WHITE,0); //ѡ��ʧЧ

    if(slect!=2)  virtual_LCM_Rectangle_V2(s_X+16*8-1,s_Y-1,16*2+1, 32+1,WHITE,0); //ѡ��ʧЧ

    if(slect!=3)  virtual_LCM_Rectangle_V2(16+s_X-1, s_Y+60-1,16*2+1, 32+1,WHITE,0); //ѡ��ʧЧ

    if(slect!=4)  virtual_LCM_Rectangle_V2(16+s_X+16*3-1,s_Y+60-1,16*2+1, 32+1,WHITE,0); //ѡ��ʧЧ

    if(slect!=5)  virtual_LCM_Rectangle_V2(16+s_X+16*6-1, s_Y+60-1,16*2+1, 32+1,WHITE,0); //ѡ��ʧЧ


	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$�꼰�ļ���$$$$$$$$$$$$$$$$$$$$$$$$$
        re=IsPressed_V3(x, y,s_X,s_Y,16*4, 32,&Time_Date[0]);//��  
	    if(re==3){
		    slect=0;
	    } 

        re=IsPressed_V3(x, y,s_X+16*5,s_Y,16*2,32,&Time_Date[1]);//��   
	    if(re==3){
			slect=1;
         }

        re=IsPressed_V3(x, y,s_X+16*8,s_Y,16*2,32,&Time_Date[2]);//��   
	    if(re==3){
		    slect=2;
         }

        re=IsPressed_V3(x, y,16+s_X, s_Y+60,16*2,32,&Time_Date[3]);//ʱ   
	    if(re==3){
		    slect=3;
         }

        re=IsPressed_V3(x, y,16+s_X+16*3,s_Y+60,16*2,32,&Time_Date[4]);//��   
	    if(re==3){
		    slect=4;
         }

        re=IsPressed_V3(x, y,16+s_X+16*6, s_Y+60,16*2,32,&Time_Date[5]);//��   
	    if(re==3){
		    slect=5;
         }


	   re=IsPressed(x, y,326,201, 342-326, 225-201,&home);//home  
	   if(re==3){
		  break;
	   } 

	   re=IsPressed(x, y,B_X, B_Y, B_L, B_H,&pressed);//����-  
	   if(re==3){
		   switch(slect)
		   {
		   	  case 0:
			      	   year--;
					   if(year==2008)  year=2099;
				        break;

		   	  case 1:
			      	   month--;
					   if(month==0)  month=12;
				        break;

		   	  case 2:
			      	   day--;
					   if(day==0)  day=31;
				        break;

		   	  case 3:
					   if(hour>0)  hour--;
					   else        hour=23;
				        break;

		   	  case 4:
			      	   
					   if(min>0)  min--;
					   else       min=59;
				        break;

		   	  case 5:
			      	   
					   if(sec>0)  sec--;
					   else       sec=59;
				        break;

		   	  default :
				        break;
		   
		   }
	   } 
	      
	   re=IsPressed(x, y,B_X+B_L*1+B_DX*1, B_Y, B_L, B_H,&pressed_1);//����+ 
	   if(re==3){
		   switch(slect)
		   {
		   	  case 0:
			      	   year++;
					   if(year==2100)  year=2009;
				        break;

		   	  case 1:
			      	   month++;
					   if(month==13)  month=1;
				        break;

		   	  case 2:
			      	   day++;
					   if(day==32)  day=1;
				        break;

		   	  case 3:
			      	   hour++;
					   if(hour==24)  hour=0;
				        break;

		   	  case 4:
			      	   min++;
					   if(min==60)  min=0;
				        break;

		   	  case 5:
			      	   sec++;
					   if(sec==60)  sec=0;
				        break;

		   	  default :
				        break;
		   
		   }
	   }    
    
       re=IsPressed(x, y,B_X+B_L*2+B_DX*2, B_Y, B_L, B_H,&pressed_2);//����Set     
	   if(re==3){
		  rtc_set(year,month,day,hour,min,sec);
	   }    

       re=IsPressed(x, y,B_X+B_L*3+B_DX*3, B_Y, B_L, B_H,&pressed_3);//����OK     
	   if(re==3){
		  break;
	   } 
   }
		GUI_SetFont(&GUI_Font8x16);
}
//********************end of file******************

