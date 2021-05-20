/*********************************************************************************************************//**
 * @file    EXTI/GPIO_Interrupt/main.c
 * @version $Rev:: 2814         $
 * @date    $Date:: 2018-05-17 #$
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

/* Global variables ----------------------------------------------------------------------------------------*/
vu32 guKeyState[3];
u16 LED = 0xFFFF;
/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  Main program.
  * @retval None
  ***********************************************************************************************************/
void Delay(u32 cnt) { while(cnt--); } 
void Key_Process(void);
void CKCU_configuration(void)
{
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  CKCUClock.Bit.PB = 1;
  CKCUClock.Bit.PC = 1;
  CKCUClock.Bit.PD = 1;
  CKCUClock.Bit.AFIO = 1;
  CKCUClock.Bit.EXTI = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
}
void GPIO_configuration(void)
{
  //LED0 使用C14腳位
  AFIO_GPxConfig(GPIO_PC,AFIO_PIN_14,AFIO_FUN_GPIO);
  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_14, GPIO_DIR_OUT);
  //LED1 使用C15腳位
  AFIO_GPxConfig(GPIO_PC,AFIO_PIN_15,AFIO_FUN_GPIO);
  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_15, GPIO_DIR_OUT);
  //LED2 使用C1腳位
  AFIO_GPxConfig(GPIO_PC,AFIO_PIN_1,AFIO_FUN_GPIO);
  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_1, GPIO_DIR_OUT);
  //BUTTON WAKEUP 使用B12腳位
  AFIO_GPxConfig(GPIO_PB,AFIO_PIN_12,AFIO_FUN_GPIO);
  //設定為Input
  GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_12, GPIO_DIR_IN);
  //關閉上拉或下拉電阻功能
  GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_12, GPIO_PR_DISABLE);
  //啟動Input功能
  GPIO_InputConfig(HT_GPIOB, GPIO_PIN_12, ENABLE);
  //BUTTON KEY1 使用D1
  AFIO_GPxConfig(GPIO_PD,AFIO_PIN_1,AFIO_FUN_GPIO);
  GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_1, GPIO_DIR_IN);
  GPIO_PullResistorConfig(HT_GPIOD, GPIO_PIN_1, GPIO_PR_DISABLE);
  GPIO_InputConfig(HT_GPIOD, GPIO_PIN_1, ENABLE);
  //BUTTON KEY2 使用D2
  AFIO_GPxConfig(GPIO_PD,AFIO_PIN_2,AFIO_FUN_GPIO);
  GPIO_DirectionConfig(HT_GPIOD, GPIO_PIN_2, GPIO_DIR_IN);
  GPIO_PullResistorConfig(HT_GPIOD, GPIO_PIN_2, GPIO_PR_DISABLE);
  GPIO_InputConfig(HT_GPIOD, GPIO_PIN_2, ENABLE);
}
void EXTI_configuration(void)
{
  EXTI_InitTypeDef EXTI_InitStruct;

  AFIO_EXTISourceConfig(AFIO_EXTI_CH_12, AFIO_ESS_PB);
  EXTI_InitStruct.EXTI_Channel=EXTI_CHANNEL_12;
  EXTI_InitStruct.EXTI_Debounce=EXTI_DEBOUNCE_ENABLE;
  EXTI_InitStruct.EXTI_DebounceCnt=32000;
  //for 1ms, 1ms*32MHz=32000
  EXTI_InitStruct.EXTI_IntType=EXTI_POSITIVE_EDGE;
  EXTI_Init(&EXTI_InitStruct);
  EXTI_IntConfig(EXTI_CHANNEL_12, ENABLE);

  AFIO_EXTISourceConfig(AFIO_EXTI_CH_1, AFIO_ESS_PD);
  EXTI_InitStruct.EXTI_Channel=EXTI_CHANNEL_1;
  EXTI_InitStruct.EXTI_IntType=EXTI_NEGATIVE_EDGE;
  EXTI_Init(&EXTI_InitStruct);
  EXTI_IntConfig(EXTI_CHANNEL_1, ENABLE);

  AFIO_EXTISourceConfig(AFIO_EXTI_CH_2, AFIO_ESS_PD);
  EXTI_InitStruct.EXTI_Channel=EXTI_CHANNEL_2;
  EXTI_InitStruct.EXTI_IntType=EXTI_NEGATIVE_EDGE;
  EXTI_Init(&EXTI_InitStruct);
  EXTI_IntConfig(EXTI_CHANNEL_2, ENABLE);
}
void NVIC_configuration(void){
  NVIC_EnableIRQ(EXTI12_IRQn);
  NVIC_EnableIRQ(EXTI1_IRQn);
  NVIC_EnableIRQ(EXTI2_IRQn);

  NVIC_SetPriority(EXTI4_15_IRQn, 0);
  NVIC_SetPriority(EXTI0_1_IRQn, 1);
  NVIC_SetPriority(EXTI2_3_IRQn, 2);  
}

int main(void)
{
  CKCU_configuration();//啟用CKCU設定
  GPIO_configuration();//啟用GPIO設定
  EXTI_configuration();//啟用EXTI設定
  NVIC_configuration();//啟用NVIC設定
  //初始時關閉所有LED燈(1為暗，0為亮)
  GPIO_WriteOutData(HT_GPIOC,0xFFFF);
  while (1)                           /* Infinite loop                                                      */
  {
	//讀取旗標，改變LED燈
    Key_Process();
  }
}

/*********************************************************************************************************//**
  * @brief  Key Process
  * @retval None
  ***********************************************************************************************************/
void Key_Process(void)
{
//如果按下第一個按鈕，guKeyState[0]值變為TRUE
  if (guKeyState[0] == TRUE)
  {
	//清空旗標
    guKeyState[0] = FALSE;
	//使用XOR運算使進入中斷的LED燈狀態反向，其他燈狀態不變
    LED ^= 0x4000;
	//輸出LED燈
    GPIO_WriteOutData(HT_GPIOC,LED);
  }
  if (guKeyState[1] == TRUE)
  {
    guKeyState[1] = FALSE;
    LED^=0x8000;
    GPIO_WriteOutData(HT_GPIOC,LED);
  }
  if (guKeyState[2] == TRUE)
  {
    guKeyState[2] = FALSE;
    LED ^=0x0002;
    GPIO_WriteOutData(HT_GPIOC,LED);
  }
}