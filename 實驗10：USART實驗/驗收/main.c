
//ɏƎUSART\HyperTerminal_TxRx_Interrupt_FIFOޤɒΗȯ
/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "ht32_board.h"
#include "ht32_board_config.h"
#include <string.h>

/* Private define ------------------------------------------------------------------------------------------*/
#define BufferSize  256

#define FLASH_WriteAddress     0x100000
#define FLASH_ReadAddress      FLASH_WriteAddress
#define FLASH_SectorToErase    FLASH_WriteAddress

/* Private variables ---------------------------------------------------------------------------------------*/
u8 Tx_Buffer[BufferSize];
u8 Rx_Buffer[BufferSize];
vu32 FLASH_ID = 0;
vu32 count = 0;
/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  Main program.
  * @retval None
  ***********************************************************************************************************/
void CKCU_configuration(){
/* Enable peripheral clock of AFIO, USART0                                                                  */
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  CKCUClock.Bit.AFIO    = 1;
  CKCUClock.Bit.PA      = 1;
  CKCUClock.Bit.PB      = 1;
  CKCUClock.Bit.PC      = 1;
  CKCUClock.Bit.USART1  = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
}
void USART_configuration(){
  USART_InitTypeDef USART_InitStructure;

  /* Config AFIO mode as USART1_Rx and USART1_Tx function.                                                  */
  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_14, AFIO_FUN_USART_UART);        //USART1_Tx
  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_15, AFIO_FUN_USART_UART);        //USART1_Rx

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
void GPIO_configuration(){
  //BUTTON_KEY_WAKE_UP
  AFIO_GPxConfig(GPIO_PB,AFIO_PIN_12,AFIO_FUN_GPIO);
  GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_12, GPIO_DIR_IN);
  GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_12, GPIO_PR_DISABLE);
  GPIO_InputConfig(HT_GPIOB, GPIO_PIN_12, ENABLE);
}
int main(void)
{
  u32 i, init;
  CKCU_configuration();
  USART_configuration();
  GPIO_configuration();
	
  NVIC_EnableIRQ(USART1_IRQn);
	
  /* Initialize the SPI_FLASH driver                                                                            */
  init = SPI_FLASH_Init();

  while(init)
	{
		/* Read SPI Flash ID                                                                                      */
		FLASH_ID = SPI_FLASH_ReadJEDECID();

		/* Clear the Block Protection bit                                                                         */
		SPI_FLASH_WriteStatus(0x00);

		/* Erase SPI FLASH Sector to write on                                                                     */
		SPI_FLASH_SectorErase(FLASH_SectorToErase);

		/* Write Tx_Buffer data to SPI FLASH memory                                                               */
		SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);
		/////------按下Button1輸出存入的資料(最多8bit)------//////////
		if(GPIO_ReadInBit(HT_GPIOB,GPIO_PIN_12)==1)
		{
      /* Read Rx_Buffer data from SPI FLASH memory                                                              */
      SPI_FLASH_BufferDualRead((u16*)Rx_Buffer, FLASH_ReadAddress, BufferSize/2);

      /* Send Rx_Buffer Data                                                                                    */
      for(i = 0;i<count;i++){
        USART_SendData(HT_USART1,Rx_Buffer[i]);
      }
			for(i=0;i<10000000;i++);//避免多次輸出
    }
		///------------------------/////
  }
}
