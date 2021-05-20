/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "ht32_board.h"
#include "ht32_board_config.h"

void CKCU_Configuration(void)
{
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
  CKCUClock.Bit.AFIO       = 1;
  CKCUClock.Bit.ADC        = 1;
  CKCUClock.Bit.PC         = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
}

void GPIO_configuration(void)
{
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
int main(void)
{
	
  u32 Data = 0;
  u32 Data_10 = 0;
	
  CKCU_Configuration();
  GPIO_configuration();
  ADC_configuration();
  //初始關閉所有LED燈
  GPIO_WriteOutData(HT_GPIOC,0xFFFF);
  while (1)
  {
	//讀取ADC數值
    Data = ADC_GetConversionData(HT_ADC,ADC_REGULAR_DATA0);
	//將得到的數值(0-4095)轉換為(0-10) 
    Data_10 = (Data & 0x0FFF)*10/4095;
	//Data小於3 關閉所有LED
    if(Data_10<3){
      GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_1,1);
      GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_15,1);
      GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_14,1);
    }
	//Data小於6 開啟LED0
    else if(Data_10<6){
      GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_1,0);
      GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_15,1);
      GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_14,1);
    }
	//Data小於9 開啟LED1
    else if(Data_10<9){
      GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_1,0);
      GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_15,0);
      GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_14,1);
    }
	//其他狀況 開啟所有LED
    else{
      GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_1,0);
      GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_15,0);
      GPIO_WriteOutBits(HT_GPIOC,GPIO_PIN_14,0);
    }
  }
}
