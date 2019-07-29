#ifndef _MMC_SD_DRIVER_H_
#define _MMC_SD_DRIVER_H_
//#include "COMM.H"	 
#include "stm32f10x.h"
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
#define	 SD_CS_PORT   GPIOB
#define	 SD_CS_PIN	  GPIO_Pin_0
#define	 SD_SPI_NUM	  SPI1
#define	 SPI_PORT	  GPIOA
#define	 SPI_MISO	  GPIO_Pin_6
#define	 SPI_PINS	  (GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7)

/* Select MSD Card: ChipSelect pin low  */
#define MSD_CS_LOW()     GPIO_ResetBits(SD_CS_PORT, SD_CS_PIN)
/* Deselect MSD Card: ChipSelect pin high */
#define MSD_CS_HIGH()    GPIO_SetBits(SD_CS_PORT, SD_CS_PIN)


#define	SD_CS_ENABLE()  GPIO_ResetBits(SD_CS_PORT, SD_CS_PIN) //ѡ��SD��
#define SD_CS_DISABLE() GPIO_SetBits(SD_CS_PORT, SD_CS_PIN)	  //ȡ��ѡ��				    	 

#define RE_TRY_CNT      0x8fff  //���Դ��� 0xfffe

/* SD�����Ͷ��� */
#define SD_TYPE_MMC     0
#define SD_TYPE_V1      1
#define SD_TYPE_V2      2
#define SD_TYPE_V2HC    4

/* SPI�����ٶ�����*/
#define SPI_SPEED_LOW   0
#define SPI_SPEED_HIGH  1

/* SD�������ݽ������Ƿ��ͷ����ߺ궨�� */
#define NO_RELEASE      0
#define RELEASE         1
							  

/* SD��ָ��� */
#define CMD0    0       //����λ
#define CMD9    9       //����9 ����CSD����
#define CMD10   10      //����10����CID����
#define CMD12   12      //����12��ֹͣ���ݴ���
#define CMD16   16      //����16������SectorSize Ӧ����0x00
#define CMD17   17      //����17����sector
#define CMD18   18      //����18����Multi sector
#define ACMD23  23      //����23�����ö�sectorд��ǰԤ�Ȳ���N��block
#define CMD24   24      //����24��дsector
#define CMD25   25      //����25��дMulti sector
#define ACMD41  41      //����41��Ӧ����0x00
#define CMD55   55      //����55��Ӧ����0x01
#define CMD58   58      //����58����OCR��Ϣ
#define CMD59   59      //����59��ʹ��/��ֹCRC��Ӧ����0x00
					 

/* Private function prototypes -----------------------------------------------*/
void SPI_Configuration(void);
void SPI_SetSpeed(u8 SpeedSet);

u8 SPI_ReadWriteByte(u8 TxData);                //SPI���߶�дһ���ֽ�
u8 SD_WaitReady(void);                          //�ȴ�SD������
u8 SD_SendCommand(u8 cmd, u32 arg, u8 crc);     //SD������һ������
u8 SD_SendCommand_NoDeassert(u8 cmd, u32 arg, u8 crc);
u8 SD_Init(void);                               //SD����ʼ��
                                                //
u8 SD_ReceiveData(u8 *data, u16 len, u8 release);//SD��������
u8 SD_GetCID(u8 *cid_data);                     //��SD��CID
u8 SD_GetCSD(u8 *csd_data);                     //��SD��CSD
u32 SD_GetCapacity(void);                       //ȡSD������

u8 SD_ReadSingleBlock(u32 sector, u8 *buffer);  //��һ��sector
u8 SD_WriteSingleBlock(u32 sector, const u8 *buffer); //дһ��sector
u8 SD_ReadMultiBlock(u32 sector, u8 *buffer, u8 count); //�����sector
u8 SD_WriteMultiBlock(u32 sector, const u8 *data, u8 count);  //д���sector
u8 SD_Read_Bytes(unsigned long address,unsigned char *buf,unsigned int offset,unsigned int bytes);//��ȡһbyte
#endif
