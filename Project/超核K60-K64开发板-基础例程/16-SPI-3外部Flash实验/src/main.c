#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "w25qxx.h"
#include "spi.h"
/* CH Kinetis固件库 V2.50 版本 */
/* 修改主频 请修改 CMSIS标准文件 system_MKxxxx.c 中的 CLOCK_SETUP 宏 */
 
/*
     实验名称：GPIO小灯实验
     实验平台：渡鸦开发板
     板载芯片：MK60DN512ZVQ10
 实验效果: 获取板子上面SPI-Flash的信息通过串口发送出去
      小灯周期性闪烁，闪烁时间间隔500ms     
*/

#include <string.h>

static int w25qxx_test(void)
{
    uint32_t i, block, buf_size,j;
    static uint8_t buf[4*1024];
    
    printf("spi flash id:0x%X\r\n", w25qxx_get_id());
    buf_size = sizeof(buf);
    
    block = 512;
    
    /* erase chip */
    printf("erase all chips...\r\n");
    w25qxx_erase_chip();
    printf("erase complete\r\n");
    
    for(i=0; i<block; i++)
    {
        printf("verify addr:0x%X(%d)...\r\n", i*buf_size, i);
        for(j=0;j<sizeof(buf);j++)
        {
            buf[j] = j % 0xFF;
        }
        w25qxx_write(i*block, buf, buf_size);
        memset(buf, buf_size, 0);
        w25qxx_read(i*block, buf, buf_size);
        
        /* varify */
        for(j=0;j<sizeof(buf);j++)
        {
            if(buf[j] != (j%0xFF))
            {
                printf("%d error\r\n", j);
            }
        }
    }
    return 0;
}

static uint32_t _get_reamin(void)
{
    return 0;
}

static uint32_t xfer(uint8_t *buf_in, uint8_t *buf_out, uint32_t len, uint8_t cs_state)
{
    uint8_t dummy_in;
    
    if(!buf_in)
        buf_in = &dummy_in;
    
    while(len--)
    {
        if(len == 0)
        {
            *buf_in = SPI_ReadWriteByte(HW_SPI2, HW_CTAR0, *buf_out, 1, !cs_state); 
        }
        else
        {
            *buf_in = SPI_ReadWriteByte(HW_SPI2, HW_CTAR0, *buf_out, 1, kSPI_PCS_KeepAsserted); 
        }
        if(buf_out)
            buf_out++;
        if(buf_in != &dummy_in)
            buf_in++;
    }
    return len;
}

int main(void)
{
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    
    printf("w25qxx test\r\n");
    
    /* 初始化SPI2接口 */
    SPI_QuickInit(SPI2_SCK_PD12_SOUT_PD13_SIN_PD14, kSPI_CPOL0_CPHA0, 10*1000*1000);
    
    /* 初始化w25qxx 使用CS1片选 */
    PORT_PinMuxConfig(HW_GPIOD, 15, kPinAlt2); /* SPI2_PCS1 */
    
    /* 获取SPI-Flash的信息 */
    struct w25qxx_init_t init;
    
    init.delayms = DelayMs;
    init.get_reamin = _get_reamin;
    init.xfer = xfer;
    
    if(w25qxx_init(&init))
    {
        printf("w25qxx device no found!\r\n");
    }
    else
    {
        printf("spi_flash found id:0x%X\r\n", w25qxx_get_id());
    }

    w25qxx_test();
    
    printf("w252qxx test complete\r\n");
    
    while(1)
    {
        GPIO_ToggleBit(HW_GPIOE, 6);
        DelayMs(500);  //小灯闪烁
    }
}

