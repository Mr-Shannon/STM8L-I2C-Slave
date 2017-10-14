#include "i2c.h"

#define MAX_DATA_LEN 65534

uint8_t receive_buffer[100];
uint16_t receive_length = 0;
uint16_t receive_length_count = 0;
uint8_t receive_success_flag = 0;

uint8_t reg_addr = 0;

uint8_t stop_flag = 0;
uint8_t send_buffer[100]=
{
   0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99,
};
uint16_t send_length = 0 ;

uint8_t i2c_irq_step = 0;


/*******************************************************************************
* Function Name  : interface_i2c_send_data_state
* Description    : I2C发送唤醒
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void i2c_send_data_state(void)
{
  GPIO_Init( GPIOD, GPIO_Pin_3, GPIO_Mode_Out_PP_High_Fast); 
  GPIO_WriteBit( GPIOD, GPIO_Pin_3, SET);
  delay_us(300);
  GPIO_WriteBit( GPIOD, GPIO_Pin_3, RESET);
  delay_us(200);
  GPIO_Init( GPIOD, GPIO_Pin_3, GPIO_Mode_In_PU_No_IT); 
}
/*******************************************************************************
* Function Name  : i2c_config
* Description    : I2C配置
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void i2c_config(uint32_t clock_frequency, uint16_t own_address)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_I2C1, ENABLE);        
                                                         
    I2C_DeInit(I2C1);
    I2C_Init(I2C1, clock_frequency, own_address, I2C_Mode_I2C, 
             I2C_DutyCycle_2, I2C_Ack_Enable, I2C_AcknowledgedAddress_7bit);
    
    I2C_ITConfig(I2C1, (I2C_IT_TypeDef)(I2C_IT_ERR | I2C_IT_EVT | I2C_IT_BUF), ENABLE);     
    I2C_Cmd(I2C1, ENABLE);
}
/*******************************************************************************
* Function Name  : i2c_receive_data
* Description    : I2C接收
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void i2c_receive_handler(void)
{
    uint8_t data = 0;
//    printf("Receive!\n"); 
 
    if ((I2C1->SR1 & 0x40)&&(0 == (I2C1->SR3 & 0x04))) 
    {  
        data = I2C1->DR;
        receive_buffer[receive_length_count] = data;  
//        printf("DATA%d【%x】\n",receive_length_count+1, receive_buffer[receive_length_count]);
        receive_length_count ++;
//        delay_us(1);
    }
}

/*******************************************************************************
* Function Name  : i2c_send_data
* Description    : I2C发送
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void i2c_send_handler(void)
{
//    printf("Send!\n"); 
       
    for (uint8_t i = 0; i < 4; i++)
    {
        while (!((I2C1->SR1 & 0x84)&&(I2C1->SR3 & 0x04))); //数据字节传送完
        I2C1->DR = send_buffer[i];
        if (i == 3)
        {
            while(I2C_GetLastEvent(I2C1) != I2C_EVENT_SLAVE_ACK_FAILURE);
            i2c_irq_step = 0;
            stop_flag = 0;
            printf("send Over!\n");
        } 
    } 
}
/*******************************************************************************
* Function Name  : I2C_IRQHandler
* Description    : I2C中断处理
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#pragma vector = 31  
__interrupt void I2C_IRQHandler(void)
{
    uint8_t data = 0;
//    printf("I2C It! %d\n", i2c_irq_step);
    
    switch (i2c_irq_step)
    {
    case 0:
        if (I2C1->SR1 & 0x02)//地址已经匹配
        {
            data = I2C1->SR3;
             if (data & 0x04)
                 i2c_send_handler();
             else
                i2c_irq_step = 1;
        }
        else if ((I2C1->SR1 & 0x40)&&(0 == (I2C1->SR3 & 0x04))) 
        {
            i2c_receive_handler();
        }
        break;
        
     case 1:
        if ((I2C1->SR1 & 0x40)&&(0 == (I2C1->SR3 & 0x04))) 
        {
            data = I2C1->DR;
            reg_addr = data; 
//            printf("CMD【%x】\n",reg_addr);
            i2c_irq_step = 0; 
        }
        break; 
    
    default:
        break;
    }
    if (I2C1->SR1 & 0x10)//检测停止位
    {
        printf("STOP\n");
        I2C1->CR2 = I2C1->CR2 ;//清除停止位
        receive_length = receive_length_count;
        receive_length_count = 0;
        receive_success_flag = 1;
        i2c_irq_step = 0;
    } 
    I2C_ClearITPendingBit(I2C1, (I2C_IT_TypeDef)(I2C_IT_ERR | I2C_IT_EVT | I2C_IT_BUF));
}

