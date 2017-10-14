#include "tools.h"

extern uint8_t receive_buffer[100];
extern uint16_t receive_length;
extern uint8_t receive_success_flag;
extern uint8_t reg_addr;

int main(int argc, char **argv)
{
    disableInterrupts();    //失能中断
    system_init();
    uart_config(115200);
    i2c_config(400000, 0x50);  
    enableInterrupts();     //使能中断
    printf("I2C Test!\n");
    while(1)                          
    {   
        if(receive_success_flag)
        {
            printf("REG_ADDR:%3x\n", reg_addr);
            printf("DATA_LEN:%3d\n", receive_length);
            printf("DATA:");
            for (uint16_t i = 0; i< receive_length; i++)
                printf("%3x",receive_buffer[i] );
            printf("\n");
            receive_success_flag = 0;
        }
    }
    return 0;
}