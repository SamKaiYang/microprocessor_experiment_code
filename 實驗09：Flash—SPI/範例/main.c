/*********************************************************************************************************//**
 * @file    SPI/Flash/main.c
 * @version $Rev:: 164          $
 * @date    $Date:: 2015-12-04 #$
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
// <<< Use Configuration Wizard in Context Menu >>>

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "ht32_board.h"

/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup SPI_Examples SPI
  * @{
  */

/** @addtogroup Flash
  * @{
  */


/* Private define ------------------------------------------------------------------------------------------*/
#define BufferSize  256

#define FLASH_WriteAddress     0x100000
#define FLASH_ReadAddress      FLASH_WriteAddress
#define FLASH_SectorToErase    FLASH_WriteAddress

/* Private variables ---------------------------------------------------------------------------------------*/
u8 Tx_Buffer[BufferSize];
u8 Rx_Buffer[BufferSize];
vu32 FLASH_ID = 0;

/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  Main program.
  * @retval None
  ***********************************************************************************************************/
void GPIO_configuration(void)
{
	/* Configure WEAKUP, KEY1, KEY2 pins as the input function                                                */
  /* 設定映射到腳位上之功能                                                                      */
  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_14, AFIO_FUN_GPIO);
  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_15, AFIO_FUN_GPIO);
  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_1, AFIO_FUN_GPIO);
  /* 設定腳位I/O方向                                                                */
  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_14, GPIO_DIR_OUT);
  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_15, GPIO_DIR_OUT);
  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_1, GPIO_DIR_OUT);
}
void CKCU_configuration(void)
{
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	CKCUClock.Bit.PC = 1;
	CKCUClock.Bit.AFIO = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
}
int main(void)
{
  u32 i, err = 0, result;
	CKCU_configuration();
	GPIO_configuration();
  for (i = 0; i < BufferSize; i++)
  {
    Tx_Buffer[i] = i;
  }
  /* Initialize the SPI_FLASH driver                                                                        */
  result = SPI_FLASH_Init();
  /* Check the SPI Flash ID                                                                                 */
  if (result == TRUE)
  {
    /* Turn on LED1                                                                                         */
		GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_14, 0);
  }
  else
  {
    /* Turn off LED1                                                                                        */
		GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_14, 1);
  }
  /* Read SPI Flash ID                                                                                      */
  FLASH_ID = SPI_FLASH_ReadJEDECID();
  /* Clear the Block Protection bit                                                                         */
  SPI_FLASH_WriteStatus(0x00);
  /* Erase SPI FLASH Sector to write on                                                                     */
  SPI_FLASH_SectorErase(FLASH_SectorToErase);
  /* Write Tx_Buffer data to SPI FLASH memory                                                               */
  SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);
  /* Read Rx_Buffer data from SPI FLASH memory                                                              */
  SPI_FLASH_BufferDualRead((u16*)Rx_Buffer, FLASH_ReadAddress, BufferSize/2);
  /* Check the correctness of written data                                                                  */
  for (i = 0; i < BufferSize; i++)
  {
    if (Rx_Buffer[i] != Tx_Buffer[i])
    {
      err++;
    }
  }
  if (err == 0)
  {
    /*  Turn on LED2                                                                                        */
		GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_15, 0);
  }
  else
  {
    /*  Turn off LED2                                                                                       */
		GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_15, 1);
  }
  err = 0;
  /* Erase SPI FLASH Sector to write on                                                                     */
  SPI_FLASH_SectorErase(FLASH_SectorToErase);
  /* Read Rx_Buffer data from SPI FLASH memory                                                              */
  SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
  /* Check the correctness of written data                                                                  */
  for (i = 0; i < BufferSize; i++)
  {
    if (Rx_Buffer[i] != 0xFF)
    {
      err++;
    }
  }
  if (err == 0)
  {
    /*  Turn on LED3                                                                                        */
    GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_1, 0);
  }
  else
  {
    /*  Turn off LED3                                                                                       */
    GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_1, 1);
  }
  while (1);
}

#if (HT32_LIB_DEBUG == 1)
/*********************************************************************************************************//**
  * @brief  Report both the error name of the source file and the source line number.
  * @param  filename: pointer to the source file name.
  * @param  uline: error line source number.
  * @retval None
  ***********************************************************************************************************/
void assert_error(u8* filename, u32 uline)
{
  /*
     This function is called by IP library that the invalid parameters has been passed to the library API.
     Debug message can be added here.
     Example: printf("Parameter Error: file %s on line %d\r\n", filename, uline);
  */

  while (1)
  {
  }
}
#endif


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
