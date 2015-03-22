#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "dma.h"

#include <rl_usb.h>


static int USB_SetClockDiv(uint32_t srcClock)
{
    uint8_t frac,div;
    
    /* clear all divivder */
    SIM->CLKDIV2 &= ~SIM_CLKDIV2_USBDIV_MASK;
    SIM->CLKDIV2 &= ~SIM_CLKDIV2_USBFRAC_MASK;
    
    for(frac = 0; frac < 2; frac++)
    {
        for(div = 0; div < 8; div++)
        {
            if(((srcClock*(frac+1))/(div+1)) > 47000000 && ((srcClock*(frac+1))/(div+1)) < 49000000)
            {
                SIM->CLKDIV2 |= SIM_CLKDIV2_USBDIV(div);
                (frac)?(SIM->CLKDIV2 |= SIM_CLKDIV2_USBFRAC_MASK):(SIM->CLKDIV2 &= ~SIM_CLKDIV2_USBFRAC_MASK);
                LIB_TRACE("USB clock OK src:%d frac:%d div:%d 0x%08X\r\n", srcClock, frac, div, SIM->CLKDIV2);
                return 0;
            }
        }
    }
    return 1;
}

uint8_t USB_ClockInit(void)
{
    /* open clock gate */
    SIM->SOPT2 |= SIM_SOPT2_USBSRC_MASK;
    
    /* clock config */
    uint32_t clock;
    CLOCK_GetClockFrequency(kMCGOutClock, &clock);
    if(USB_SetClockDiv(clock))
    {
        LIB_TRACE("USB clock setup fail\r\n");
        return 1;
    }

    /* which MCG generator is to be used */
    SIM->SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK;
#ifdef SIM_SOPT2_PLLFLLSEL
    (MCG->C6 & MCG_C6_PLLS_MASK)?
    (SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL(1)):   /* PLL */
    (SIM->SOPT2 &= ~SIM_SOPT2_PLLFLLSEL(0));  /* FLL */
#else
    (MCG->C6 & MCG_C6_PLLS_MASK)?
    (SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK):   /* PLL */
    (SIM->SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK);  /* FLL */
#endif
	return 0;
}

/*
 *  Get Mouse Input Report -> MouseInReport
 *    Parameters:      report:
 *                               Byte0.0: 1st Button (Left)
 *                               Byte0.1: 2nd Button (Right)
 *                               Byte0.2: 3rd Button
 *                               Byte1:   Relative X Pos
 *                               Byte2:   Relative Y Pos
 *                               Byte3:   Relative Wheel Pos
 *                     size:     report size
 *    Return Value:    None
 */

void GetMouseInReport (S8 *report, U32 size) {
  U32 key;

    report[0] = 0;
    report[1] = 1;
    report[2] = 0;
    report[3] = 0;
}



int main(void)
{
    S8 report[4] = {0, 0, 0, 0};
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
    
    #ifdef FRDM
    UART_QuickInit(UART0_RX_PB16_TX_PB17, 115200);
    #else
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
    #endif
    
    printf("USB\r\n");
    if(USB_ClockInit())
    {
        printf("USB  Init failed, clock must be 96M or 48M\r\n");
        while(1);
    }

    /* disable memory protection */
    MPU->CESR=0;
    
    usbd_init();                          /* USB Device Initialization          */
    usbd_connect(__TRUE);                 /* USB Device Connect                 */

    while(1)
    {
        GetMouseInReport(report, 4);
        usbd_hid_get_report_trigger(0, (U8 *)report, 4);
    }
}


