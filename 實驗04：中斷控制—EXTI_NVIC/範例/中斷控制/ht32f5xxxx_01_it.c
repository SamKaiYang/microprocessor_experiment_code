/*********************************************************************************************************//**
 * @file    EXTI/GPIO_Interrupt/ht32f5xxxx_01_it.c
 * @version $Rev:: 2445         $
 * @date    $Date:: 2017-12-25 #$
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

/** @addtogroup EXTI_Examples EXTI
  * @{
  */

/** @addtogroup GPIO_Interrupt
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

#if defined(USE_HT32F52230_SK)
/*********************************************************************************************************//**
 * @brief   This function handles EXIT line 4~15 interrupt.
 * @retval  None
 ************************************************************************************************************/
void EXTI4_15_IRQHandler(void)
{
  extern vu32 guKeyState[3];
  /*Get weak-up button status*/
  if(EXTI_GetEdgeStatus(WAKEUP_BUTTON_EXTI_CHANNEL,EXTI_EDGE_NEGATIVE))
  {
    EXTI_ClearEdgeFlag(WAKEUP_BUTTON_EXTI_CHANNEL);
  }
  else if(EXTI_GetEdgeStatus(WAKEUP_BUTTON_EXTI_CHANNEL,EXTI_EDGE_POSITIVE))
  {
    guKeyState[0] = TRUE;
    EXTI_ClearEdgeFlag(WAKEUP_BUTTON_EXTI_CHANNEL);
  }
}
/*********************************************************************************************************//**
 * @brief   This function handles EXIT line 2~3 interrupt.
 * @retval  None
 ************************************************************************************************************/
void EXTI2_3_IRQHandler(void)
{
  extern vu32 guKeyState[3];
  /*Get key1 button status*/
  if(EXTI_GetEdgeStatus(KEY1_BUTTON_EXTI_CHANNEL,EXTI_EDGE_NEGATIVE))
  {
    EXTI_ClearEdgeFlag(KEY1_BUTTON_EXTI_CHANNEL);
    guKeyState[1] = TRUE;
  }
  else if(EXTI_GetEdgeStatus(KEY1_BUTTON_EXTI_CHANNEL,EXTI_EDGE_POSITIVE))
  {
    EXTI_ClearEdgeFlag(KEY1_BUTTON_EXTI_CHANNEL);
  }
}
#endif
#if defined(USE_HT32F52241_SK)
/*********************************************************************************************************//**
 * @brief   This function handles EXIT line 4~15 interrupt.
 * @retval  None
 ************************************************************************************************************/
void EXTI4_15_IRQHandler(void)
{
  extern vu32 guKeyState[3];
  /*Get key2 button status*/
  if(EXTI_GetEdgeStatus(KEY1_BUTTON_EXTI_CHANNEL,EXTI_EDGE_NEGATIVE))
  {
    EXTI_ClearEdgeFlag(KEY1_BUTTON_EXTI_CHANNEL);
    guKeyState[1] = TRUE;
  }
  else if(EXTI_GetEdgeStatus(KEY1_BUTTON_EXTI_CHANNEL,EXTI_EDGE_POSITIVE))
  {
    EXTI_ClearEdgeFlag(KEY1_BUTTON_EXTI_CHANNEL);
  }
  /*Get key2 button status*/
  if(EXTI_GetEdgeStatus(KEY2_BUTTON_EXTI_CHANNEL,EXTI_EDGE_NEGATIVE))
  {
    EXTI_ClearEdgeFlag(KEY2_BUTTON_EXTI_CHANNEL);
    guKeyState[2] = TRUE;
  }
  else if(EXTI_GetEdgeStatus(KEY2_BUTTON_EXTI_CHANNEL,EXTI_EDGE_POSITIVE))
  {
    EXTI_ClearEdgeFlag(KEY2_BUTTON_EXTI_CHANNEL);
    
  }   
  /*Get weak-up button status*/
  if(EXTI_GetEdgeStatus(WAKEUP_BUTTON_EXTI_CHANNEL,EXTI_EDGE_NEGATIVE))
  {
    EXTI_ClearEdgeFlag(WAKEUP_BUTTON_EXTI_CHANNEL);
  }
  else if(EXTI_GetEdgeStatus(WAKEUP_BUTTON_EXTI_CHANNEL,EXTI_EDGE_POSITIVE))
  {
    guKeyState[0] = TRUE;
    EXTI_ClearEdgeFlag(WAKEUP_BUTTON_EXTI_CHANNEL);
  }
}
#endif
#if defined(USE_HT32F52341_SK)
/*********************************************************************************************************//**
 * @brief   This function handles EXIT line 4~15 interrupt.
 * @retval  None
 ************************************************************************************************************/
void EXTI4_15_IRQHandler(void)
{
  extern vu32 guKeyState[3];
  /*Get weak-up button status*/
  if(EXTI_GetEdgeStatus(WAKEUP_BUTTON_EXTI_CHANNEL,EXTI_EDGE_NEGATIVE))
  {
    EXTI_ClearEdgeFlag(WAKEUP_BUTTON_EXTI_CHANNEL);
  }
  else if(EXTI_GetEdgeStatus(WAKEUP_BUTTON_EXTI_CHANNEL,EXTI_EDGE_POSITIVE))
  {
    guKeyState[0] = TRUE;
    EXTI_ClearEdgeFlag(WAKEUP_BUTTON_EXTI_CHANNEL);
  }
}
/*********************************************************************************************************//**
 * @brief   This function handles EXIT line 0~1 interrupt.
 * @retval  None
 ************************************************************************************************************/
void EXTI0_1_IRQHandler(void)
{
  extern vu32 guKeyState[3];
  /*Get key1 button status*/
  if(EXTI_GetEdgeStatus(KEY1_BUTTON_EXTI_CHANNEL,EXTI_EDGE_NEGATIVE))
  {
    EXTI_ClearEdgeFlag(KEY1_BUTTON_EXTI_CHANNEL);
    guKeyState[1] = TRUE;
  }
  else if(EXTI_GetEdgeStatus(KEY1_BUTTON_EXTI_CHANNEL,EXTI_EDGE_POSITIVE))
  {
    EXTI_ClearEdgeFlag(KEY1_BUTTON_EXTI_CHANNEL);
  }
}
/*********************************************************************************************************//**
 * @brief   This function handles EXIT line 2~3 interrupt.
 * @retval  None
 ************************************************************************************************************/
void EXTI2_3_IRQHandler(void)
{
  extern vu32 guKeyState[3];
  /*Get key2 button status*/
  if(EXTI_GetEdgeStatus(KEY2_BUTTON_EXTI_CHANNEL,EXTI_EDGE_NEGATIVE))
  {
    EXTI_ClearEdgeFlag(KEY2_BUTTON_EXTI_CHANNEL);
    guKeyState[2] = TRUE;
  }
  else if(EXTI_GetEdgeStatus(KEY2_BUTTON_EXTI_CHANNEL,EXTI_EDGE_POSITIVE))
  {
    EXTI_ClearEdgeFlag(KEY2_BUTTON_EXTI_CHANNEL);
  }
}
#endif
#if defined(USE_HT32F52352_SK)
/*********************************************************************************************************//**
 * @brief   This function handles EXIT line 4~15 interrupt.
 * @retval  None
 ************************************************************************************************************/
void EXTI4_15_IRQHandler(void)
{
  int i;	
  //讀取在main.c中的變數guKeyState
  extern vu32 guKeyState[3];
  //取得指定通道中斷狀態 判斷是否觸發
  if(EXTI_GetEdgeStatus(EXTI_CHANNEL_12,EXTI_EDGE_POSITIVE))
  {
    //EXTI_ClearEdgeFlag(EXTI_CHANNEL_12);
		guKeyState[0] = TRUE;
		for(i=0;i<100000;i++);//10000000
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_12);
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_1);
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_2);
  }
}
/*********************************************************************************************************//**
 * @brief   This function handles EXIT line 0~1 interrupt.
 * @retval  None
 ************************************************************************************************************/
void EXTI0_1_IRQHandler(void)
{
  int i;
  //讀取在main.c中的變數guKeyState
  extern vu32 guKeyState[3];
  //取得指定通道中斷狀態 判斷是否觸發
  if(EXTI_GetEdgeStatus(EXTI_CHANNEL_1,EXTI_EDGE_NEGATIVE))
  {
    //EXTI_ClearEdgeFlag(EXTI_CHANNEL_1);
    guKeyState[1] = TRUE;
		for(i=0;i<100000;i++);//10000000
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_12);
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_1);
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_2);
  }
}
/*********************************************************************************************************//**
 * @brief   This function handles EXIT line 2~3 interrupt.
 * @retval  None
 ************************************************************************************************************/
void EXTI2_3_IRQHandler(void)
{
  int i;
  //讀取在main.c中的變數guKeyState
  extern vu32 guKeyState[3];
  //取得指定通道中斷狀態 判斷是否觸發
  if(EXTI_GetEdgeStatus(EXTI_CHANNEL_2,EXTI_EDGE_NEGATIVE))
  {
    //EXTI_ClearEdgeFlag(EXTI_CHANNEL_2);
    guKeyState[2] = TRUE;
		for(i=0;i<100000;i++);//10000000
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_12);
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_1);
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_2);
  }
}
#endif

#if defined(USE_HT32F50230_SK) || defined(USE_HT32F50241_SK) || defined(USE_HT32F0008_SK)
/*********************************************************************************************************//**
 * @brief   This function handles EXIT line 2~3 interrupt.
 * @retval  None
 ************************************************************************************************************/
void EXTI2_3_IRQHandler(void)
{
  extern vu32 guKeyState[3];
  if(EXTI_GetEdgeStatus(KEY1_BUTTON_EXTI_CHANNEL,EXTI_EDGE_NEGATIVE))
  {
    EXTI_ClearEdgeFlag(KEY1_BUTTON_EXTI_CHANNEL);
    guKeyState[1] = TRUE;
  }
  else if(EXTI_GetEdgeStatus(KEY1_BUTTON_EXTI_CHANNEL,EXTI_EDGE_POSITIVE))
  {
    EXTI_ClearEdgeFlag(KEY1_BUTTON_EXTI_CHANNEL);
  }

  if(EXTI_GetEdgeStatus(KEY2_BUTTON_EXTI_CHANNEL,EXTI_EDGE_NEGATIVE))
  {
    EXTI_ClearEdgeFlag(KEY2_BUTTON_EXTI_CHANNEL);
    guKeyState[2] = TRUE;
  }
  else if(EXTI_GetEdgeStatus(KEY2_BUTTON_EXTI_CHANNEL,EXTI_EDGE_POSITIVE))
  {
    EXTI_ClearEdgeFlag(KEY2_BUTTON_EXTI_CHANNEL);
  }
}
/*********************************************************************************************************//**
 * @brief   This function handles EXIT line 4~15 interrupt.
 * @retval  None
 ************************************************************************************************************/
void EXTI4_15_IRQHandler(void)
{
  extern vu32 guKeyState[3];
  /*Get weak-up button status*/
  if(EXTI_GetEdgeStatus(WAKEUP_BUTTON_EXTI_CHANNEL,EXTI_EDGE_NEGATIVE))
  {
    EXTI_ClearEdgeFlag(WAKEUP_BUTTON_EXTI_CHANNEL);
  }
  else if(EXTI_GetEdgeStatus(WAKEUP_BUTTON_EXTI_CHANNEL,EXTI_EDGE_POSITIVE))
  {
    guKeyState[0] = TRUE;
    EXTI_ClearEdgeFlag(WAKEUP_BUTTON_EXTI_CHANNEL);
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
