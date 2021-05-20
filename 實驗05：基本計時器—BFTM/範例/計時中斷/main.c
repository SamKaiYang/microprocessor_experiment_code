/*********************************************************************************************************//**
 * @file    BFTM/RepetitiveToggle/main.c
 * @version $Rev:: 2191         $
 * @date    $Date:: 2017-11-28 #$
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

/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup BFTM_Examples BFTM
  * @{
  */

/** @addtogroup RepetitiveToggle
  * @{
  */

/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  Main program.
  * @retval None
  ***********************************************************************************************************/
//---main.c---//
void CKCU_Configuration(void){
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	CKCUClock.Bit.PC = 1;
	CKCUClock.Bit.AFIO = 1;
	CKCUClock.Bit.BFTM0      = 1;
	CKCUClock.Bit.BFTM1      = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
}

void BFTM_Configuration(void){
  //將BFTM設為0.5秒為一週期: 1/0.5S = 2Hz <=> 1/2Hz = 0.5s
  //SystemCoreClock 為SDK內建參數，AHB時鐘跑1秒的計數量
  float Time_s = 0.5;
  float Frequency_Hz = 2;
	
  //Compare值擇一宣告
  //int Compare = SystemCoreClock * Time_s;
  int Compare = SystemCoreClock / Frequency_Hz;

  BFTM_SetCompare(HT_BFTM0,Compare);
  //計時器歸0
  BFTM_SetCounter(HT_BFTM0, 0);
  //不啟用單次模式
  BFTM_OneShotModeCmd(HT_BFTM0, DISABLE);
  //BTFM中斷源開啟
  BFTM_IntConfig(HT_BFTM0, ENABLE);
  //BFTM開啟
  BFTM_EnaCmd(HT_BFTM0, ENABLE);
	
  BFTM_SetCompare(HT_BFTM1, SystemCoreClock);
  BFTM_SetCounter(HT_BFTM1, 0);
  BFTM_OneShotModeCmd(HT_BFTM1, DISABLE);
  BFTM_IntConfig(HT_BFTM1, ENABLE);
  BFTM_EnaCmd(HT_BFTM1, ENABLE);
}
void GPIO_configuration(void)
{
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
void NVIC_Configuration(void){
  NVIC_EnableIRQ(BFTM0_IRQn);
  NVIC_EnableIRQ(BFTM1_IRQn);
}

int main(void)
{
	NVIC_Configuration();
	CKCU_Configuration();
	BFTM_Configuration();
	GPIO_configuration();
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_14, 1);
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_15, 1);
	GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_1, 1);

	while (1)                           /* Infinite loop                                                      */
	{
		//----code-------//
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
