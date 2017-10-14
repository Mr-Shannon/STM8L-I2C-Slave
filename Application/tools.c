#include "tools.h"

/*******************************************************************************
* Function Name  : System_Init
* Description    : STM8L151单片机系统初始化
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 		
void system_init(void)
{
    CLK_SYSCLKSourceSwitchCmd(ENABLE);
    CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1); 
    CLK_HSICmd(ENABLE);
}

 /******************************************************************************
* Function Name  : delay_ms
* Description    : 定时器4的精确毫秒级延时
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void delay_ms(uint16_t nTime)   
{  
  uint16_t tim4 = 0;

  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);
  TIM4_DeInit();
  TIM4_TimeBaseInit(TIM4_Prescaler_64, 250);  //1ms
  TIM4_Cmd(ENABLE);
  while (tim4 < nTime)
  {   
    TIM4_ClearFlag(TIM4_FLAG_Update);
    while (TIM4_GetFlagStatus(TIM4_FLAG_Update) == RESET);
    tim4 ++;  
  }
  TIM4_Cmd(DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, DISABLE);
}

/******************************************************************************
* Function Name  : delay_us
* Description    : 定时器4的精确毫秒级延时
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void delay_us(uint16_t nTime)   
{  
  uint16_t tim4 = 0;

  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);
  TIM4_TimeBaseInit(TIM4_Prescaler_1, 160);  //10us
  TIM4_Cmd(ENABLE);
  while (tim4 < nTime)
  {   
    TIM4_ClearFlag(TIM4_FLAG_Update);
    while (TIM4_GetFlagStatus(TIM4_FLAG_Update) == RESET);
    tim4 ++;  
  }
  TIM4_Cmd(DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, DISABLE);
}


