/*********************************************************************************************************//**
 * @file    GPIO/InputOutput/main.c
 * @version $Rev:: 1277         $
 * @date    $Date:: 2016-11-22 #$
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
#include "ht32.h"
#include "ht32_board.h"
#include "ht32_board_config.h"

/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup GPIO_Examples GPIO
  * @{
  */

/** @addtogroup InputOutput
  * @{
  */


/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  Main program.
  * @retval None
  * @note At this stage the microcontroller clock setting should has already been configured.
  *       This can be done through SystemInit() function which is called from startup
  *       file (startup_ht32fxxxx.s) before branching to application main().
  *       To reconfigure the default setting of SystemInit() function, please refer to
  *       system_ht32fxxxx.c file
  * @details The main function works as the following:
  *    - Check if the backup domain is ready for access so as to disable isolation.
  *      Because some GPIO pins are located in the backup domain.
  *    - Enable AFIO peripheral clock
  *    - Configure WEAKUP, KEY1, KEY2 pins as the input function
  *      - Enable GPIO peripheral clock of input pins
  *      - Configure AFIO mode of input pins
  *      - Configure GPIO direction of input pins
  *    - Configure LED1, LED2, LED3 pins as the output function
  *      - Enable GPIO peripheral clock of output pins
  *      - Configure AFIO mode of output pins
  *      - Configure GPIO direction of output pins
  *    - Infinite loop to read data from the input pin and then output to LED
  *      - Read WEAKUP and then output via LED1
  *      - Read KEY1 and then output via LED2
  *      - Read KEY2 and then output via LED3
  *
  ***********************************************************************************************************/
void CKCU_configuration(void)
{
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	CKCUClock.Bit.PC = 1;
	CKCUClock.Bit.PB = 1;
	CKCUClock.Bit.PD = 1;
	
  CKCUClock.Bit.AFIO       = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
}
void GPIO_configuration(void)
{
	/* Configure WEAKUP, KEY1, KEY2 pins as the input function                                                */
  /* 設定映射到腳位上之功能                                                                      */
	AFIO_GPxConfig(GPIO_PB, AFIO_PIN_12, AFIO_FUN_GPIO);
  AFIO_GPxConfig(GPIO_PD, AFIO_PIN_1, AFIO_FUN_GPIO);
  AFIO_GPxConfig(GPIO_PD, AFIO_PIN_2, AFIO_FUN_GPIO);
  /* 設定腳位I/O方向                                                          */
  GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_12, GPIO_DIR_IN);
  GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_1, GPIO_DIR_IN);
  GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_2, GPIO_DIR_IN);
  /* 設定腳位是否加上提升/接地電阻                                                            */
	GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_12, GPIO_PR_DISABLE);
  GPIO_PullResistorConfig(HT_GPIOD, GPIO_PIN_1, GPIO_PR_DISABLE);
  GPIO_PullResistorConfig(HT_GPIOD, GPIO_PIN_2, GPIO_PR_DISABLE);
  /* 設定腳位是否開啟Input功能                                                           */
  GPIO_InputConfig(HT_GPIOB, GPIO_PIN_12, ENABLE);
  GPIO_InputConfig(HT_GPIOD, GPIO_PIN_1, ENABLE);
  GPIO_InputConfig(HT_GPIOD, GPIO_PIN_2, ENABLE);
  /* Configure LED1, LED2, LED3 pins as output function                                                     */
  /* 設定映射到腳位上之功能                                                                     */
  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_14, AFIO_FUN_GPIO);
  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_15, AFIO_FUN_GPIO);
  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_1, AFIO_FUN_GPIO);
  /* 設定腳位I/O方向                                                                */
  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_14, GPIO_DIR_OUT);
  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_15, GPIO_DIR_OUT);
  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_1, GPIO_DIR_OUT);
}
int main(void)
{
  FlagStatus TmpStatus = RESET;
	/*CKCU SET                */
  CKCU_configuration();
	GPIO_configuration();
  /* Infinite loop to read data from input pin and then output to LED                                       */
  while (1)
  {
    /* Read WEAKUP and then output to LED1                                                                  */
    TmpStatus = GPIO_ReadInBit(HT_GPIOB, GPIO_PIN_12);
    GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_14, TmpStatus);

    /* Read KEY1 and then output to LED2                                                                    */
    TmpStatus = GPIO_ReadInBit(HT_GPIOD, GPIO_PIN_1);
    GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_15, TmpStatus);

    /* Read KEY2 and then output to LED3                                                                    */
    TmpStatus = GPIO_ReadInBit(HT_GPIOD, GPIO_PIN_2);
    GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_1, TmpStatus);
  }
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
