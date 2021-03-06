/*********************************************************************************************************//**
 * @file    I2C/EEPROM/main.c
 * @version $Rev:: 219          $
 * @date    $Date:: 2016-02-16 #$
 * @brief   Main program.
 *************************************************************************************************************
 * @attention
 *
 * Firmware Disclaimer Information
 *
 * 1. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, which is supplied by Holtek Semiconductor Inc., (hereinafter referred to as "HOLTEK") is the
 *    proprietary and confidential intellectual property of HOLTEK, and is protected by copyright law and
 *    other intellectual property laws.
 *
 * 2. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, is confidential information belonging to HOLTEK, and must not be disclosed to any third parties
 *    other than HOLTEK and the customer.
 *
 * 3. The program technical documentation, including the code, is provided "as is" and for customer reference
 *    only. After delivery by HOLTEK, the customer shall use the program technical documentation, including
 *    the code, at their own risk. HOLTEK disclaims any expressed, implied or statutory warranties, including
 *    the warranties of merchantability, satisfactory quality and fitness for a particular purpose.
 *
 * <h2><center>Copyright (C) Holtek Semiconductor Inc. All rights reserved</center></h2>
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include "ht32.h"
#include "ht32_board.h"
#include <stdio.h> 

/* Private constants ---------------------------------------------------------------------------------------*/
#define WRITE_BUF_ADDR    (0x20000600)
#define READ_BUF_ADDR     (0x20000700)
#define EEPROM_PAGE_CNT   (I2C_EEPROM_CAPACITY / I2C_EEPROM_PAGE_SIZE)


void CKCU_configuration(){
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  CKCUClock.Bit.PC         = 1;
  CKCUClock.Bit.AFIO       = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
}

void GPIO_configuration(){
  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_14 | 
                          AFIO_PIN_15 , AFIO_FUN_GPIO);
	
  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_14 | 
                                 GPIO_PIN_15 , GPIO_DIR_OUT);
}
/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  Main program.
  * @retval None
  ***********************************************************************************************************/
int main(void)
{
  u32 i, j, k, *pBuf, *pBuf2;
  int length;
	//???????????????EEPROM?????????(??????????????????)
  char sent[]={"hello world"};
	//????????????????????????????????????
  char get[256];

  CKCU_configuration();
  GPIO_configuration();
	//???????????????printf??????????????????
  RETARGET_Configuration();           
	//????????????LED
  GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_14,1);
  GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_15,1);
  //??????sent?????????????????????
  length = strlen(sent);
  //printf("\r\n???????????????%d", length);
  printf("\r\n*** HT32 I2C Example Code ***");
  printf("\r\n24LC02 EEPROM verifying : ");
	
  /* init EEPROM  I??C????????????????????????i2c_eeprom.c??????????????????????????????SCL???A0???SDA???A1                                                                                          */
  I2C_EEPROM_Init();
//============================================================================================================
	k=0;
	/* generate sent pattern                                                                                  */
	for (j = 0; j < EEPROM_PAGE_CNT; j++){
		pBuf =  (u32 *)WRITE_BUF_ADDR;
	  //???sent???????????????WRITE_BUF_ADDR?????????
		for (i = 0; i < (I2C_EEPROM_PAGE_SIZE / 4); i++){
			if(k<length){
				*pBuf++ = sent[k];
				k++;
			}else break;
		}
    //???READ_BUF_ADDR??????????????????????????????0XC3
		memset((u8 *)READ_BUF_ADDR, 0xC3, I2C_EEPROM_PAGE_SIZE);
		//???????????????EEPROM
		I2C_EEPROM_BufferWrite((u8 *)WRITE_BUF_ADDR, I2C_EEPROM_PAGE_SIZE * j, I2C_EEPROM_PAGE_SIZE);	
		//???EEPROM????????????
		I2C_EEPROM_BufferRead((u8 *)READ_BUF_ADDR, I2C_EEPROM_PAGE_SIZE * j, I2C_EEPROM_PAGE_SIZE);

		//??????WRITE_BUF_ADDR???READ_BUF_ADDR???????????????????????????????????????????????????(LED1)???????????????
		if (memcmp((u8 *)WRITE_BUF_ADDR, (u8 *)READ_BUF_ADDR, I2C_EEPROM_PAGE_SIZE) != 0){
			printf(" [Failed]\n");
			GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_15,0);//LED1 light //error
			while (1);
		}
		printf("\r\t\t\t\t%d%%", (int)((j + 1) * 100)/EEPROM_PAGE_CNT);
	}
  //??????????????????????????????(LED0)
	printf(" [Pass]\r\n");
	GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_14,0);//LED0 light //success
	
//===========================================================================================================
  k=0;
  for (j = 0; j < EEPROM_PAGE_CNT; j++){
    pBuf2 =  (u32 *)READ_BUF_ADDR;
	//???EEPROM????????????
    I2C_EEPROM_BufferRead((u8 *)READ_BUF_ADDR, I2C_EEPROM_PAGE_SIZE * j, I2C_EEPROM_PAGE_SIZE);
	//???????????????get?????????
    for (i = 0; i < (I2C_EEPROM_PAGE_SIZE / 4); i++){
      get[k]= *pBuf2++;
	  //???\0??????char??????????????????????????????\0?????????????????????
      if(get[k]!='\0') k++;
      else {
        length=k;break;
      }
    }
  }
  //????????????????????????
  for(i=0;i<length;i++){
    printf("%c",get[i]);
  }
}