/*********************************************************************************************************//**
 * @file    USART/HyperTerminal_TxRx_Interrupt_FIFO/ht32f5xxxx_01_it.c
 * @version $Rev:: 2351         $
 * @date    $Date:: 2017-12-12 #$
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
#include "ht32_board_config.h"

/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup USART_Examples USART
  * @{
  */

/** @addtogroup HyperTerminal_TxRx_Interrupt_FIFO
  * @{
  */


/* Private define ------------------------------------------------------------------------------------------*/
#define USART_BUFSIZE             (4)
/* Private variables ---------------------------------------------------------------------------------------*/

/* USART0 definitions                                                                                       */
u8 LED_Buffer;

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
 * @brief   This function handles UART0 interrupt.
 * @retval  None
 ************************************************************************************************************/
void HTCFG_USART_IRQHandler(void)
{
	int i;
  /* Rx ,We have received four datas from UART.                                                             */
  if (USART_GetFlagStatus(HT_USART1, USART_FLAG_RXDR))
  {
    USART_ClearFlag(HT_USART1, USART_FLAG_RXDR);
    /* Rx, Geting these datas from USART's FIFO                                                             */
    LED_Buffer = USART_ReceiveData(HT_USART1);
    if(LED_Buffer < 0x08)
    {
      if((0x04 & LED_Buffer) == 0x04){
        GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_14,0);
      }else{
        GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_14,1);
      }
      if((0x02 & LED_Buffer) == 0x02) {
        GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_15,0);
      }else{
        GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_15,1);
      }
      if((0x01 & LED_Buffer) == 0x01){
        GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_1,0);
      }else{
        GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_1,1);
      }
      //for(i=0;i<1000000;i++);
    }
  }                                                                                  
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
