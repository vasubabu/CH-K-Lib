#include "gpio.h"
#include "common.h"
#include "uart.h"


void PendSV_Handler(void)
{
    printf("PendSV_Handler\r\n");
}

void SVC_Handler(void)
{
    printf("SVC_Handler\r\n"); 
}
 
int main(void)
{
    DelayInit();
    /* 初始化一个引脚 设置为推挽输出 */
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    /* 初始化一个串口 使用UART0端口的PTD6引脚和PTD7引脚作为接收和发送，默认设置 baud 115200 */
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    
    printf("HelloWorld!\r\n");
    /* 执行一次PendSV */
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
    /* 执行一次SVC */
    SCB->SHCSR |= SCB_SHCSR_SVCALLPENDED_Msk;
    while(1)
    {
        /* 闪烁小灯 */
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(500);
    }
}


