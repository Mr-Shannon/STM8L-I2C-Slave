#include "uart.h"

/*******************************************************************************
* Function Name  : fputc
* Description    : printf从定向
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int fputc(int ch, FILE *f)   //Printf
{
    uart_send_byte((uint8_t)ch);
    while (!(USART1->SR & USART_FLAG_TXE)); 
    return ch;
}
 /*******************************************************************************
* Function Name  : Uart_Configuration
* Description    : USART1的初始化配置
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void uart_config(uint32_t baud_rate)
{
	CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);
    GPIO_ExternalPullUpConfig(GPIOC,GPIO_Pin_3|GPIO_Pin_2,ENABLE);
	USART_DeInit(USART1);      
	USART_Init(USART1, (uint32_t)baud_rate, USART_WordLength_8b, 
               USART_StopBits_1, USART_Parity_No, (USART_Mode_TypeDef)(USART_Mode_Tx|USART_Mode_Rx)); 
	USART_ITConfig(USART1, USART_IT_TC, DISABLE);   
	USART_Cmd(USART1,ENABLE);             
}
 /*******************************************************************************
* Function Name  : Uart_SendByte
* Description    : USART1字节发送函数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void uart_send_byte(uint8_t data)
{
    USART_SendData8(USART1,(uint8_t)data);
    while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}