#include "stm32f10x.h"
#include <stdio.h>
#include "string.h"

#include "fatfs.h"  

#include "uart_debug.h"
#include "rc522.h"
#include "delay.h"
#include "pwm.h"


extern FIL USERFile;        //  file handle do fatfs.c t?o

struct FILE
{
  int handle;
};
/* FILE is typedef'd in stdio.h. */
FILE __stdout;
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == 0){};
  return ch;
}











uint8_t Key[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
uint8_t WtC[] = "Card is accpect";  // write data to card
uint8_t RtC[16];										// read data from card

uint8_t UID[5];

uint8_t UID_accpect_1[5] = {0x74, 0xBC, 0xE6, 0x03, 0x2D}; //White card
uint8_t UID_accpect_2[5] = {0xC1, 0x09, 0x98, 0x1D, 0x4D};	//Blue card

uint8_t str[MAX_LEN];  // bien de luu byte data doc tu card
uint8_t status;

unsigned int check = 0;

void blink_led(void);



void SD_SPI1_Init(void)
{
    GPIO_InitTypeDef g;
    SPI_InitTypeDef  s;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                           RCC_APB2Periph_AFIO  |
                           RCC_APB2Periph_SPI1, ENABLE);

    /* PA5-SCK, PA7-MOSI – AF-PP */
    g.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_7;
    g.GPIO_Mode  = GPIO_Mode_AF_PP;
    g.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&g);

    /* PA6-MISO – input */
    g.GPIO_Pin  = GPIO_Pin_6;
    g.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA,&g);

    /* PA4 – CS_SD – output high (inactive) */
    g.GPIO_Pin  = GPIO_Pin_4;
    g.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA,&g);
    GPIO_SetBits(GPIOA,GPIO_Pin_4);

    /* SPI1 400 kHz d? init th? */
    s.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
    s.SPI_Mode              = SPI_Mode_Master;
    s.SPI_DataSize          = SPI_DataSize_8b;
    s.SPI_CPOL              = SPI_CPOL_Low;
    s.SPI_CPHA              = SPI_CPHA_1Edge;
    s.SPI_NSS               = SPI_NSS_Soft;
    s.SPI_FirstBit          = SPI_FirstBit_MSB;
    s.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; //˜400 kHz
    s.SPI_CRCPolynomial     = 7;
    SPI_Init(SPI1,&s);
    SPI_Cmd(SPI1,ENABLE);
}




int main(void)
{
	UART_Config_debug();
	Tim_init();
	MFRC522_Init();
	SD_SPI1_Init();
	Tim_PWMinit();
	
	
	
	MX_FATFS_Init();        // register driver
    if (f_mount(&USERFatFS,"",1)==FR_OK &&
        f_open(&USERFile,"uidlog.txt",
               FA_OPEN_ALWAYS | FA_WRITE)==FR_OK)
    {
        f_lseek(&USERFile,f_size(&USERFile));   // ghi n?i duôi
        printf("SD ready\r\n");
    }
    else
    {
        printf("SD mount/open fail\r\n");
    }

    delay_ms(500);
	
	
	
//	while(1)
//	{

//		TIM_SetCompare2(TIM2, 6);				//0
//		delay_ms(2000);
//		TIM_SetCompare2(TIM2, 15); 			// 90
//		delay_ms(2000);
//		TIM_SetCompare2(TIM2, 25);			//180
//		delay_ms(2000);
//	}
		delay_ms(500);
	while(1)
	{		
		printf("Start RFID\n");		
		TIM_SetCompare2(TIM2, 6);
		delay_ms(500);
		status = MI_ERR;

		while(status != MI_OK) {	
			status = MFRC522_Request(PICC_REQIDL, str);  // check your card are put into yet??
			delay_ms(100);
			GPIO_SetBits(GPIOA, GPIO_Pin_5);
			//blink_led();
		}
		printf("Find your ID card\n");
		status = MFRC522_Anticoll(str);			// chong de the len nhau va kiem tra data doc co trung ko
		memcpy(UID, str, 5);
		delay_ms(100);
		
		if(status == MI_OK)
		{
			printf("ID card number: %x, %x, %x, %x, %x\n", UID[0], UID[1], UID[2], UID[3], UID[4]);
			
			
			
			/* --- ghi vào file --- */
				char   line[32];    // d?m ch?a chu?i ASCII c?a UID
				UINT   bw;          // bi?n nh?n s? byte th?c s? dã ghi

				// Format 4 byte UID thành chu?i hex, ví d? "74BCE603\r\n"
				sprintf(line,
								"%02X%02X%02X%02X\r\n",
								UID[0], UID[1], UID[2], UID[3]);

				// USERFile.fs != 0 nghia là file dã du?c m? thành công và
				// h? th?ng file (SD) dã mount
				if (USERFile.fs) {
						f_write(&USERFile,      // handle file (d?i tu?ng FIL do fatfs.c c?p)
										line,           // con tr? vào buffer ch?a d? li?u
										strlen(line),   // s? byte c?n ghi
										&bw);           // luu vào bw s? byte dã ghi th?c t?
						f_sync(&USERFile);     // b?t FatFs flush luôn sector xu?ng SD
				}
			
			
			
				
			if((UID[0]==116) && (UID[1]==188) && (UID[2]==230) && (UID[3]==3) && (UID[4]==45) )
      {
				check = 1;
			}else	if((UID[0]==0xC1) && (UID[1]==0x09) && (UID[2]==0x98) && (UID[3]==0x1D) && (UID[4]==0x4D))
			{
				check = 2;
			}
		}
		
		if(check == 1)
		{
			printf("Correct id card_1\n");
			delay_ms(100);
			MFRC522_SelectTag(str);
			MFRC522_Auth(PCD_AUTHENT, 2, Key, UID);
			printf("Sucessfully\n");
			
//			while(status == MI_OK)
//			{
				TIM_SetCompare2(TIM2, 15);

			delay_ms(5000);
			check = 0;
		}else if(check == 2)
		{
			printf("Correct id card_2\n");
			delay_ms(100);
			MFRC522_SelectTag(str);
			MFRC522_Auth(PCD_AUTHENT, 2, Key, UID);
			printf("Sucessfully\n");
			
//			while(status == MI_OK){
				TIM_SetCompare2(TIM2, 25);
			delay_ms(5000);
			check = 0;
		}
		
		//MFRC522_StopCrypto1();
		printf("RFID STOP\n");
	}
}

void blink_led(void)
{
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		delay_ms(500);
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
		delay_ms(500);
}