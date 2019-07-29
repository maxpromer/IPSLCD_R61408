/*��ݴ��뾭Դ���ʽ�����ʽ����
     yang_hx@neusoft.com      */
/*******************************************************************************
*  ���ļ�ΪSPI����SD���ĵײ������ļ�
*  ����SPIģ�鼰���IO�ĳ�ʼ����SPI��дSD����дָ������ݵȣ�
*******************************************************************************/
#include <includes.h>
#include "stm32f10x_lib.h"
#include "SD_driver.h"

u8 SD_Type=0 ;


#define  SD_UCOSII_EN    0
#define  SD_UCOSII_EN_2  0



#if SD_UCOSII_EN || SD_UCOSII_EN_2
extern OS_EVENT  *SPI_EVENT;  	/* ������Ȩ���ź��� semaphore that access card */
#endif 



void StartSD(void)
{
#if SD_UCOSII_EN_2
   unsigned char  err;	
   //OSMutexPend(SPI_EVENT, 0, &err);
   OSSemPend(SPI_EVENT, 0, &err);
#endif 
}


void EndSD(void)
{
#if SD_UCOSII_EN_2
   unsigned char  err;
   //OSMutexPost(SPI_EVENT);
   OSSemPost(SPI_EVENT);
   SD_CS_DISABLE();
#endif 
}

/*******************************************************************************************************************
** ��������: void SD_StartSD()					Name:	  INT8U SD_StartSD()
** ��������: �����ϵͳ�������SD����Ȩ��		Function: request the right of operating sd to OS
** �䡡  ��: ��									Input:	  NULL
** ��	 ��: ��									return:	  NULL
********************************************************************************************************************/
void SD_StartSD(void)
{
#if SD_UCOSII_EN
   unsigned char  err;	
   //OSMutexPend(SPI_EVENT, 0, &err);
   OSSemPend(SPI_EVENT, 0, &err); 	/* �ȴ����ʿ��ź������� wait for semaphore that accessed Card */
   //OSSchedLock();
#endif
}

/*******************************************************************************************************************
** ��������: void SD_EndSD()					Name:	  INT8U SD_EndSD()
** ��������: ����SD����Ȩ�޹黹����ϵͳ			Function: return the right of operating sd to OS
** �䡡  ��: ��									Input:	  NULL
** ��	 ��: ��									return:	  NULL
********************************************************************************************************************/
void SD_EndSD(void)
{
#if SD_UCOSII_EN
    unsigned char  err;
	//OSMutexPost(SPI_EVENT);
	OSSemPost(SPI_EVENT);							/* �����ʿ��ź�����������ϵͳ return the semaphore accessing Card to OS */
	//OSSchedUnlock();
	SD_CS_DISABLE();
#endif
}	


////////////////////////////////////////////////////////////////////////////////
// ������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ������SD��
////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
* Function Name  : SPI_Configuration
* Description    : SPIģ���ʼ�������������IO�ڵĳ�ʼ����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void picdemo_SPI_Configuration(void)
{
    SPI_InitTypeDef SPI_InitStructure ;
    GPIO_InitTypeDef GPIO_InitStructure ;


#if 1
    //����SPIʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);
    
    //////������SPI���GPIO��ʼ��//////
    //SPI1ģ���Ӧ��SCK��MISO��MOSIΪAF����
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5 ;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz ;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP ;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
	
    
    //PD3 pin:SD_CS
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz ;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP ;
    GPIO_Init(GPIOA,&GPIO_InitStructure); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //��������
    GPIO_Init(GPIOB,&GPIO_InitStructure);	


#endif

    GPIO_SetBits(GPIOA,GPIO_Pin_15);

    //////SPIģ������//////
    //һ��ʼSD��ʼ���׶Σ�SPIʱ��Ƶ�ʱ���<400K
    SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex ;
    SPI_InitStructure.SPI_Mode=SPI_Mode_Master ;
    SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b ;
    SPI_InitStructure.SPI_CPOL=SPI_CPOL_High ;
    SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge ;
    SPI_InitStructure.SPI_NSS= SPI_NSS_Soft;//	   SPI_NSS_Hard
    SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_64;//4 ;
    SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB ;
    SPI_InitStructure.SPI_CRCPolynomial=7 ;

    SPI_Init(SPI3,&SPI_InitStructure);
    SPI_Cmd(SPI3,ENABLE);

}
/*******************************************************************************
* Function Name  : SPI_ReadWriteByte
* Description    : SPI��дһ���ֽڣ�������ɺ󷵻ر���ͨѶ��ȡ�����ݣ�
* Input          : u8 TxData �����͵���
* Output         : None
* Return         : u8 RxData �յ�����
*******************************************************************************/
u8 SPI_ReadWriteByte(u8 TxData)
{
    u8 RxData=0 ;
    
	//GPIO_WriteBit(GPIOC, GPIO_Pin_7, Bit_SET); //���ļ���˸
    
	//�ȴ����ͻ�������
    while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_TXE)==RESET);
    //��һ���ֽ�
    SPI_I2S_SendData(SPI3,TxData);
    
    //�ȴ����ݽ���
    while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_RXNE)==RESET);
    //ȡ����
    RxData=SPI_I2S_ReceiveData(SPI3);
    
	//GPIO_WriteBit(GPIOC, GPIO_Pin_7, Bit_RESET);

    return RxData ;
}

/*******************************************************************************
* Function Name  : SD_WaitReady
* Description    : �ȴ�SD��Ready
* Input          : None
* Output         : None
* Return         : u8
*               0�� �ɹ�
*           other��ʧ��
*******************************************************************************/
u8 SD_WaitReady(void)
{
    u8 r1 ;
    u16 retry ;

	StartSD();
    retry=0 ;
    do 
    {
        r1=SPI_ReadWriteByte(0xFF);
        if(retry==0xfffe)//������쳣������ѭ����
        {
			EndSD();
            return 1 ;
        }
    }
    while(r1!=0xFF);
    
	EndSD();
    return 0 ;
}


/*******************************************************************************
* Function Name  : SD_SendCommand
* Description    : ��SD������һ������
* Input          : u8 cmd   ����
*                  u32 arg  �������
*                  u8 crc   crcУ��ֵ
* Output         : None
* Return         : u8 r1 SD�����ص���Ӧ
*******************************************************************************/
u8 SD_SendCommand(u8 cmd,u32 arg,u8 crc)
{
    unsigned char r1 ;
    unsigned char Retry=0 ;

	StartSD();
    SPI_ReadWriteByte(0xff);
    //Ƭѡ���õͣ�ѡ��SD��
    SD_CS_ENABLE();
    
    //����
    SPI_ReadWriteByte(cmd|0x40);
    //�ֱ�д������
    SPI_ReadWriteByte(arg>>24);
    SPI_ReadWriteByte(arg>>16);
    SPI_ReadWriteByte(arg>>8);
    SPI_ReadWriteByte(arg);
    SPI_ReadWriteByte(crc);
    
    //�ȴ���Ӧ����ʱ�˳�
    while((r1=SPI_ReadWriteByte(0xFF))==0xFF)
    {
        Retry++;
        if(Retry>200)
        {
            break ;
        }
    }  
    //�ر�Ƭѡ
    SD_CS_DISABLE();
    //�������϶�������8��ʱ�ӣ���SD�����ʣ�µĹ���
    SPI_ReadWriteByte(0xFF);
    
	EndSD();
    //����״ֵ̬
    return r1 ;
}


/*******************************************************************************
* Function Name  : SD_SendCommand_NoDeassert
* Description    : ��SD������һ������(�����ǲ�ʧ��Ƭѡ�����к������ݴ�����
* Input          : u8 cmd   ����
*                  u32 arg  �������
*                  u8 crc   crcУ��ֵ
* Output         : None
* Return         : u8 r1 SD�����ص���Ӧ
*******************************************************************************/
u8 SD_SendCommand_NoDeassert(u8 cmd,u32 arg,u8 crc)
{
    unsigned char r1 ;
    unsigned char Retry=0 ;

	StartSD();
    SPI_ReadWriteByte(0xff);
    //Ƭѡ���õͣ�ѡ��SD��
    SD_CS_ENABLE();
    
    //����
    SPI_ReadWriteByte(cmd|0x40);
    //�ֱ�д������
    SPI_ReadWriteByte(arg>>24);
    SPI_ReadWriteByte(arg>>16);
    SPI_ReadWriteByte(arg>>8);
    SPI_ReadWriteByte(arg);
    SPI_ReadWriteByte(crc);
    
    //�ȴ���Ӧ����ʱ�˳�
    while((r1=SPI_ReadWriteByte(0xFF))==0xFF)
   /* {
        Retry++;
        if(Retry>200)
        {
            break ;
        }
    }*/
	
	EndSD();
    //������Ӧֵ
    return r1 ;
}

/*******************************************************************************
* Function Name  : SD_Init
* Description    : ��ʼ��SD��
* Input          : None
* Output         : None
* Return         : u8
*                  0��NO_ERR
*                  1��TIME_OUT
*                  99��NO_CARD
*******************************************************************************/
u8 SD_Init(void)
{
    u16 i ;// ����ѭ������
    u8 r1 ;// ���SD���ķ���ֵ
	u8 buff[4];

	SD_StartSD(); 
	//�Ȳ���>74�����壬��SD���Լ���ʼ�����
    for(i=0;i<10;i++) SPI_ReadWriteByte(0xFF);

    //-----------------SD����λ��idle��ʼ-----------------
    //ѭ����������CMD0��ֱ��SD������0x01,����IDLE״̬
    do 
    {
        //����CMD0����SD������IDLE״̬
        r1=SD_SendCommand(CMD0,0,0x95);
    }
    while(r1!=0x01);

    //-----------------SD����λ��idle����-----------------

    //��ȡ��Ƭ��SD�汾��Ϣ
    r1=SD_SendCommand_NoDeassert(8,0x1aa,0x87); 
	   
    //������V2.0���ĳ�ʼ��
            //������ʼ��ָ��CMD55+ACMD41
            do 
            {
                r1=SD_SendCommand(CMD55,0,0);
                r1=SD_SendCommand(ACMD41,0x40000000,0);
            }
            while(r1!=0);
	SD_EndSD();
    return 0 ;
}

/*******************************************************************************
* Function Name  : SD_ReceiveData
* Description    : ��SD���ж���ָ�����ȵ����ݣ������ڸ���λ��
* Input          : u8 *data(��Ŷ������ݵ��ڴ�>len)
*                  u16 len(���ݳ��ȣ�
*                  u8 release(������ɺ��Ƿ��ͷ�����CS�ø� 0�����ͷ� 1���ͷţ�
* Output         : None
* Return         : u8
*                  0��NO_ERR
*                  other��������Ϣ
*******************************************************************************/

u8 SD_ReceiveData(u8*data,u16 len,u8 release)
{
    u8 r1 ;
    
	StartSD();
    // ����һ�δ���
    SD_CS_ENABLE();
    //�ȴ�SD������������ʼ����0xFE
    do 
    {
        r1=SPI_ReadWriteByte(0xFF);
    }
    while(r1!=0xFE);
    //��ʼ��������
    while(len--)
    {
        *data=SPI_ReadWriteByte(0xFF);
        data++;
    }
    //������2��αCRC��dummy CRC��
    SPI_ReadWriteByte(0xFF);
    SPI_ReadWriteByte(0xFF);
    //�����ͷ����ߣ���CS�ø�
   /* if(release==RELEASE)
    {
        //�������
        SD_CS_DISABLE();
        SPI_ReadWriteByte(0xFF);
    }  */
    
	EndSD();
    return 0 ;
}


/*******************************************************************************
* Function Name  : SD_GetCID
* Description    : ��ȡSD����CID��Ϣ��������������Ϣ
* Input          : u8 *cid_data(���CID���ڴ棬����16Byte��
* Output         : None
* Return         : u8
*                  0��NO_ERR
*                  1��TIME_OUT
*                  other��������Ϣ
*******************************************************************************/
u8 SD_GetCID(u8*cid_data)
{
    u8 r1 ;
	
	//StartSD();
    //��CMD10�����CID
    r1=SD_SendCommand(CMD10,0,0xFF);
    if(r1!=0x00)
    {
	    //EndSD();
        return r1 ;
        //û������ȷӦ�����˳�������
    }
    //����16���ֽڵ�����
    SD_ReceiveData(cid_data,16,RELEASE);
    
	//EndSD();
    return 0 ;
}


/*******************************************************************************
* Function Name  : SD_GetCSD
* Description    : ��ȡSD����CSD��Ϣ�������������ٶ���Ϣ
* Input          : u8 *cid_data(���CID���ڴ棬����16Byte��
* Output         : None
* Return         : u8
*                  0��NO_ERR
*                  1��TIME_OUT
*                  other��������Ϣ
*******************************************************************************/
u8 SD_GetCSD(u8 *csd_data)
{
    u8 r1 ;
    
	//StartSD();
    //��CMD9�����CSD
    r1=SD_SendCommand(CMD9,0,0xFF);
    if(r1!=0x00)
    {
	    //EndSD();
        return r1 ;
        //û������ȷӦ�����˳�������
    }
    //����16���ֽڵ�����
    SD_ReceiveData(csd_data,16,RELEASE);
    
	//EndSD();
    return 0 ;
}

/*******************************************************************************
* Function Name  : SD_GetCapacity
* Description    : ��ȡSD��������
* Input          : None
* Output         : None
* Return         : u32 capacity
*                   0�� ȡ��������
*******************************************************************************/
u32 SD_GetCapacity(void)
{
    u8 csd[16];
    u32 Capacity ;
    u8 r1 ;
    u16 i ;
    u16 temp ;
    
	//StartSD();
    //ȡCSD��Ϣ������ڼ��������0
    if(SD_GetCSD(csd)!=0)
    {
		//EndSD();
        return 0 ;
    }
    
    //���ΪSDHC�����������淽ʽ����
    if((csd[0]&0xC0)==0x40)
    {
        Capacity=(((u32)csd[8])<<8+(u32)csd[9]+1)*(u32)1024 ;
    }
    else 
    {
        //�������Ϊ���ϰ汾
        ////////////formula of the capacity///////////////
        //
        //  memory capacity = BLOCKNR * BLOCK_LEN
        //
        //	BLOCKNR = (C_SIZE + 1)* MULT
        //
        //           C_SIZE_MULT+2
        //	MULT = 2
        //
        //               READ_BL_LEN
        //	BLOCK_LEN = 2
        /**********************************************/
        //C_SIZE
        i=csd[6]&0x03 ;
        i<<=8 ;
        i+=csd[7];
        i<<=2 ;
        i+=((csd[8]&0xc0)>>6);
        
        //C_SIZE_MULT
        r1=csd[9]&0x03 ;
        r1<<=1 ;
        r1+=((csd[10]&0x80)>>7);
        
        //BLOCKNR
        r1+=2 ;
        temp=1 ;
        while(r1)
        {
            temp*=2 ;
            r1--;
        }
        Capacity=((u32)(i+1))*((u32)temp);
        
        // READ_BL_LEN
        i=csd[5]&0x0f ;
        //BLOCK_LEN
        temp=1 ;
        while(i)
        {
            temp*=2 ;
            i--;
        }
        //The final result
        Capacity*=(u32)temp ;
        //Capacity /= 512;
    }
	
	//EndSD();
    return (u32)Capacity ;
}


/*******************************************************************************
* Function Name  : SD_ReadSingleBlock
* Description    : ��SD����һ��block
* Input          : u32 sector ȡ��ַ��sectorֵ���������ַ��
*                  u8 *buffer ���ݴ洢��ַ����С����512byte��
* Output         : None
* Return         : u8 r1
*                   0�� �ɹ�
*                   other��ʧ��
*******************************************************************************/
u8 SD_ReadSingleBlock(u32 sector,u8*buffer)
{
    SD_StartSD(); 
	SD_SendCommand(CMD17,sector,0);
	SD_ReceiveData(buffer,512,RELEASE);
	SD_EndSD();
	return 0 ;

}

/*******************************************************************************
* Function Name  : SD_WriteSingleBlock
* Description    : д��SD����һ��block
* Input          : u32 sector ������ַ��sectorֵ���������ַ��
*                  u8 *buffer ���ݴ洢��ַ����С����512byte��
* Output         : None
* Return         : u8 r1
*                   0�� �ɹ�
*                   other��ʧ��
*******************************************************************************/
u8 SD_WriteSingleBlock(u32 sector,const u8 *data)
{
    u8 r1 ;
    u16 i ;
    u16 retry ;
    
	SD_StartSD();
    //����Ϊ����ģʽ
    //SPI_SetSpeed(SPI_SPEED_HIGH);
    
    //�������SDHC����������sector��ַ������ת����byte��ַ
    if(SD_Type!=SD_TYPE_V2HC)
    {
        sector=sector<<9 ;
    }
    
    r1=SD_SendCommand(CMD24,sector,0x00);
    if(r1!=0x00)
    {
	    SD_EndSD();
        return r1 ;
        //Ӧ����ȷ��ֱ�ӷ���
    }
    
    //��ʼ׼�����ݴ���
    SD_CS_ENABLE();
    //�ȷ�3�������ݣ��ȴ�SD��׼����
    SPI_ReadWriteByte(0xff);
    SPI_ReadWriteByte(0xff);
    SPI_ReadWriteByte(0xff);
    //����ʼ����0xFE
    SPI_ReadWriteByte(0xFE);
    
    //��һ��sector������
    for(i=0;i<512;i++)
    {
        SPI_ReadWriteByte(*data++);
    }
    //��2��Byte��dummy CRC
    SPI_ReadWriteByte(0xff);
    SPI_ReadWriteByte(0xff);
    
    //�ȴ�SD��Ӧ��
    r1=SPI_ReadWriteByte(0xff);
    if((r1&0x1F)!=0x05)
    {
        SD_CS_DISABLE();
		SD_EndSD();
        return r1 ;
    }
    
    //�ȴ��������
    retry=0 ;
    while(!SPI_ReadWriteByte(0xff))
    {
        retry++;
        //�����ʱ��д��û����ɣ������˳�
        if(retry>0xfffe)
        {
            SD_CS_DISABLE();
			SD_EndSD();
            return 1 ;
            //д�볬ʱ����1
        }
    }
    
    //д����ɣ�Ƭѡ��1
    SD_CS_DISABLE();
    SPI_ReadWriteByte(0xff);
    
	SD_EndSD();
    return 0 ;
}


/*******************************************************************************
* Function Name  : SD_ReadMultiBlock
* Description    : ��SD���Ķ��block
* Input          : u32 sector ȡ��ַ��sectorֵ���������ַ��
*                  u8 *buffer ���ݴ洢��ַ����С����512byte��
*                  u8 count ������count��block
* Output         : None
* Return         : u8 r1
*                   0�� �ɹ�
*                   other��ʧ��
*******************************************************************************/
u8 SD_ReadMultiBlock(u32 sector,u8 *buffer,u8 count)
{
    u8 r1 ;

	SD_StartSD();
    //�����������
    r1=SD_SendCommand(CMD18,sector,0);
    //������
    if(r1!=0x00)
    {
	    SD_EndSD();
        return r1 ;
    }
    //��ʼ��������
    do 
    {
        if(SD_ReceiveData(buffer,512,NO_RELEASE)!=0x00)
        {
            break ;
        }
        buffer+=512 ;
    }
    while(--count);
    
    //ȫ��������ϣ�����ֹͣ����
    SD_SendCommand(CMD12,0,0);
    //�ͷ�����
    SD_CS_DISABLE();
    SPI_ReadWriteByte(0xFF);
    
    SD_EndSD();

    if(count!=0)
    {
        return count ;
        //���û�д��꣬����ʣ�����
    }
    else 
    {
        return 0 ;
    }
}


/*******************************************************************************
* Function Name  : SD_WriteMultiBlock
* Description    : д��SD����N��block
* Input          : u32 sector ������ַ��sectorֵ���������ַ��
*                  u8 *buffer ���ݴ洢��ַ����С����512byte��
*                  u8 count д���block��Ŀ
* Output         : None
* Return         : u8 r1
*                   0�� �ɹ�
*                   other��ʧ��
*******************************************************************************/
u8 SD_WriteMultiBlock(u32 sector,const u8*data,u8 count)
{
    u8 r1 ;
    u16 i ;
    
	SD_StartSD();
    //����Ϊ����ģʽ
    //SPI_SetSpeed(SPI_SPEED_HIGH);
    
    //�������SDHC����������sector��ַ������ת����byte��ַ
    if(SD_Type!=SD_TYPE_V2HC)
    {
        sector=sector<<9 ;
    }
    //���Ŀ�꿨����MMC��������ACMD23ָ��ʹ��Ԥ����
    if(SD_Type!=SD_TYPE_MMC)
    {
        r1=SD_SendCommand(ACMD23,count,0x00);
    }
    //�����д��ָ��
    r1=SD_SendCommand(CMD25,sector,0x00);
    if(r1!=0x00)
    {
	    SD_EndSD();
        return r1 ;
        //Ӧ����ȷ��ֱ�ӷ���
    }
    
    //��ʼ׼�����ݴ���
    SD_CS_ENABLE();
    //�ȷ�3�������ݣ��ȴ�SD��׼����
    SPI_ReadWriteByte(0xff);
    SPI_ReadWriteByte(0xff);
    
    //--------������N��sectorд���ѭ������
    do 
    {
        //����ʼ����0xFC �����Ƕ��д��
        SPI_ReadWriteByte(0xFC);
        
        //��һ��sector������
        for(i=0;i<512;i++)
        {
            SPI_ReadWriteByte(*data++);
        }
        //��2��Byte��dummy CRC
        SPI_ReadWriteByte(0xff);
        SPI_ReadWriteByte(0xff);
        
        //�ȴ�SD��Ӧ��
        r1=SPI_ReadWriteByte(0xff);
        if((r1&0x1F)!=0x05)
        {
            SD_CS_DISABLE();
            //���Ӧ��Ϊ��������������ֱ���˳�
			SD_EndSD();
            return r1 ;
        }
        
        //�ȴ�SD��д�����
        if(SD_WaitReady()==1)
        {
            SD_CS_DISABLE();
            //�ȴ�SD��д����ɳ�ʱ��ֱ���˳�����
			SD_EndSD();
            return 1 ;
        }
        
        //��sector���ݴ������
    }
    while(--count);
    
    //��������������0xFD
    r1=SPI_ReadWriteByte(0xFD);
    if(r1==0x00)
    {
        count=0xfe ;
    }
    
    if(SD_WaitReady())
    {
        while(1){}
    }
    
    //д����ɣ�Ƭѡ��1
    SD_CS_DISABLE();
    SPI_ReadWriteByte(0xff);

    SD_EndSD();
    return count ;
    //����countֵ�����д����count=0������count=1
}
