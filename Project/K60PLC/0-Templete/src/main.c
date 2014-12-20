#include "chlib_k.h"
#include "dsp_basic_demo.h"
#include "dsp_fast_demo.h"
#include "dsp_complex_demo.h"
int main(void)
{
    DelayInit();
    GPIO_QuickInit(HW_GPIOE, 8, kGPIO_Mode_OPP);
    //UART_QuickInit(UART5_RX_PE08_TX_PE09, 115200);
   // printf("!!\r\n");
    
    
	/**It's clear to display choose 1 demo*/
//	/**basic fun*/
//	arm_abs_f32_demo();			
//	arm_add_f32_demo();			
//	arm_dot_prod_f32_demo();	
//	arm_mult_f32_demo();		
//	arm_negate_f32_demo();		
//	arm_offset_f32_demo();		
//	arm_scale_f32_demo();		
//	arm_sub_f32_demo();

//	/**fast fun*/
//	arm_cos_f32_demo ();
//	arm_sin_f32_demo ();
//	arm_sqrt_f32_demo();
//	
//	/**complex fun*/
//	arm_cmplx_conj_f32_demo();
//	arm_cmplx_dot_prod_f32_demo (); 
//	arm_cmplx_mag_f32_demo ();
//	arm_cmplx_mag_squared_f32_demo();
//	arm_cmplx_mult_cmplx_f32_demo();
    while(1)
    {
        GPIO_ToggleBit(HW_GPIOE, 8);
        DelayMs(500);
    }
}


