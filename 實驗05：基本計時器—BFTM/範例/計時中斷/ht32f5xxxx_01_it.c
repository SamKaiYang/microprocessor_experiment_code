/*********************************************************************************************************//**
 * @file    BFTM/RepetitiveToggle/ht32f5xxxx_01_it.c
 * @version $Rev:: 1704         $
 * @date    $Date:: 2017-08-17 #$
 * @brief   This file provides all interrupt service routine.
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
 * @brief   This function handles NMI exception.
 * @retval  None
 ************************************************************************************************************/
void NMI_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles Hard Fault exception.
 * @retval  None
 ************************************************************************************************************/
void HardFault_Handler(void)
{
  while (1);
}

/*********************************************************************************************************//**
 * @brief   This function handles SVCall exception.
 * @retval  None
 ************************************************************************************************************/
void SVC_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles PendSVC exception.
 * @retval  None
 ************************************************************************************************************/
void PendSV_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles SysTick Handler.
 * @retval  None
 ************************************************************************************************************/
void SysTick_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles BFTM0 interrupt.
 * @retval  None
 ************************************************************************************************************/
bool LED1_flag = FALSE;
void BFTM0_IRQHandler(void)
{
	
	if(LED1_flag == FALSE)
	{
		GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_14, 0);
		LED1_flag = TRUE;
	}
	else
	{
		GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_14, 1);
		LED1_flag = FALSE;
	}
  BFTM_ClearFlag(HT_BFTM0);
}

/*********************************************************************************************************//**
 * @brief   This function handles BFTM1 interrupt.
 * @retval  None
 ************************************************************************************************************/
bool LED2_flag = FALSE;
void BFTM1_IRQHandler(void)
{
	if(LED2_flag == FALSE)
	{
		GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_15, 0);
		LED2_flag = TRUE;
	}
	else
	{
		GPIO_WriteOutBits(HT_GPIOC, GPIO_PIN_15, 1);
		LED2_flag = FALSE;
	}
  BFTM_ClearFlag(HT_BFTM1);
}


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
