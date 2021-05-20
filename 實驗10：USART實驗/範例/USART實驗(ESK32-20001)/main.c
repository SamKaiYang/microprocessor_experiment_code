/*********************************************************************************************************//**
 * @file    USART/HyperTerminal_TxRx_Interrupt_FIFO/main.c
 * @version $Rev:: 2351         $
 * @date    $Date:: 2017-12-12 #$
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
#include <string.h>

/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup USART_Examples USART
  * @{
  */

/** @addtogroup HyperTerminal_TxRx_Interrupt_FIFO
  * @{
  */


/* Private variables ---------------------------------------------------------------------------------------*/
void USART_Tx(const u8 TxBuffer, u32 length);
#define delay_count 10000000
/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  Main program.
  * @retval None
  * @details Main program as following
  *  - Enable peripheral clock of AFIO, USART0.
  *  - Config AFIO mode as USARTx_Rx, USARTx_Tx function.
  *  - USARTx configuration:
  *     - BaudRate = 115200 baud
  *     - Word Length = 8 Bits
  *     - One Stop Bit
  *     - None parity bit
  *  - Config FIOF level
  *  - Waiting for inputs from HyperTerminal.
  ***********************************************************************************************************/
void CKCU_configuration(){
  /* Enable peripheral clock of AFIO, USART0                                                                */
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  CKCUClock.Bit.AFIO        = 1;
  CKCUClock.Bit.PA          = 1;
  CKCUClock.Bit.PC          = 1;
  CKCUClock.Bit.USART1      = 1;
  CKCUClock.Bit.ADC         = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
}
void GPIO_configuration(){
  /* Configure AFIO mode of input pins                                                                      */
  //LED0
  AFIO_GPxConfig(GPIO_PC,AFIO_PIN_14,AFIO_FUN_GPIO);
  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_14, GPIO_DIR_OUT);
  //LED1
  AFIO_GPxConfig(GPIO_PC,AFIO_PIN_15,AFIO_FUN_GPIO);
  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_15, GPIO_DIR_OUT);
  //LED2
  AFIO_GPxConfig(GPIO_PC,AFIO_PIN_1,AFIO_FUN_GPIO);
  GPIO_DirectionConfig(HT_GPIOC, GPIO_PIN_1, GPIO_DIR_OUT);
}
void ADC_configuration(void)
{
  /* Config AFIO mode as ADC function                                                                       */
  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_6, AFIO_FUN_ADC);

  /* Continuous Mode, Length 1, SubLength 1                                                                 */
  ADC_RegularGroupConfig(HT_ADC, CONTINUOUS_MODE, 1, 1);

  /* ADC Channel n, Rank 0, Sampling clock is (1.5 + 0) ADCLK
  Conversion time = (sampling clock + 12.5) / ADCLK = 12.4 uS */
  ADC_RegularChannelConfig(HT_ADC, ADC_CH_6, 0);

  /* Use Software Trigger as ADC trigger source                                                             */
  ADC_RegularTrigConfig(HT_ADC, ADC_TRIG_SOFTWARE);

  ADC_Cmd(HT_ADC, ENABLE);
 
  /* Software trigger to start continuous mode                                                              */
  ADC_SoftwareStartConvCmd(HT_ADC, ENABLE);
}
void USART_configuration(){
  USART_InitTypeDef USART_InitStructure;

  /* Config AFIO mode as USART1_Rx and USART1_Tx function.                                                  */
  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_14, AFIO_FUN_USART_UART);		//USART1_Tx
  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_15, AFIO_FUN_USART_UART);		//USART1_Rx

  /* USART0 configuration ----------------------------------------------------------------------------------*/
  USART_InitStructure.USART_BaudRate = 115200;                      //設定鮑率為    115200
  USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;       //設定字長      8Bits
  USART_InitStructure.USART_StopBits = USART_STOPBITS_1;            //設定停止位    1bit
  USART_InitStructure.USART_Parity = USART_PARITY_NO;               //設定校驗位   no
  USART_InitStructure.USART_Mode = USART_MODE_NORMAL;               //設定模式     正常
  USART_Init(HT_USART1, &USART_InitStructure);                      //照上述設定值進行初始化
  /* Seting Rx FIFO Level                                                                                   */
  USART_IntConfig(HTCFG_USART, USART_INT_RXDR, ENABLE);
  USART_IntConfig(HTCFG_USART, USART_INT_TOUT, ENABLE);
  USART_RXTLConfig(HTCFG_USART, USART_RXTL_01);
  USART_TxCmd(HT_USART1, ENABLE);                                   //Tx Enable
  USART_RxCmd(HT_USART1, ENABLE);                                   //Rx Enable
}
int main(void)
{
  u32 Data = 0;
  u8 Data_8 = 0;
  CKCU_configuration();
  GPIO_configuration();
  USART_configuration();
  ADC_configuration();
  ADC_SoftwareStartConvCmd(HT_ADC, ENABLE);
	
  /* Configure USARTx interrupt                                                                             */
  NVIC_EnableIRQ(USART1_IRQn);
	
  GPIO_WriteOutData(HT_GPIOC,0xFFFF);
  //USART_Tx(HelloString, sizeof(HelloString));
  while (1)
  {
		//讀取ADC資料
    Data = ADC_GetConversionData(HT_ADC,ADC_REGULAR_DATA0);
		//將資料由0-4095轉換為0-8
    Data_8 = (Data & 0x0FFF)*8/4095;
		//將資料輸出到電腦顯示
    USART_Tx(Data_8, sizeof(Data_8));
  }
}

/*********************************************************************************************************//**
  * @brief  USART send a buffer.
  * @retval None
  ***********************************************************************************************************/
void USART_Tx(const u8 TxBuffer, u32 length)
{
  int i;

  USART_SendData(HT_USART1, TxBuffer);

  while (USART_GetFlagStatus(HT_USART1, USART_FLAG_TXC) == RESET);
  for(i=0;i<delay_count;i++);
}