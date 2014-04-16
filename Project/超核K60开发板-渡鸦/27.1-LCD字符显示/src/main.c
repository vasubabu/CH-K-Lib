#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "ili9320.h"

#include "gui.h"

int LCD_L0_Init(void)
{
    ili9320_init();
    return 0;
}

void LCD_L0_SetPixelIndex(int x, int y, int PixelIndex)
{
    ili9320_write_pixel(x, y, PixelIndex);
}

unsigned int LCD_L0_GetPixelIndex(int x, int y)
{
    return ili9320_read_pixel(x, y); 
}

int main(void)
{
    uint32_t i;
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    
    printf("flexbus lcd test\r\n");
    GUI_Init();
    GUI_DispString("ucGUI");
    GUI_DispString(GUI_GetVersionString());
    GUI_DispString("\r\nHello world!");
    while(1) 
    {
        GUI_DispDecAt( i++, 20,20,4);
        if (i>9999)
        {
            i=0;
        }
    }
}


