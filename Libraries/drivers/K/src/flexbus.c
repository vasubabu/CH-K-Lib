/**
  ******************************************************************************
  * @file    flexbus.c
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.26
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  * @note    此文件为芯片FlexBus模块的底层功能函数
  ******************************************************************************
  */
#include "flexbus.h"
#include "common.h"

#if (!defined(MK10D5))

/**
 * @brief  初始化FlexBus模块
 * @note   具体的配置应用详见关于FlexBus的使用例程  
 * @param  FLEXBUS_InitStruct :FlexBus初始化配置结构体，详见FlexBus.h
 * @retval None
 */
void FLEXBUS_Init(FLEXBUS_InitTypeDef* FLEXBUS_InitStruct)
{
    // enable clock gate enable seruriy mode
    SIM->SOPT2 |= SIM_SOPT2_FBSL(3);
    SIM->SCGC7 |= SIM_SCGC7_FLEXBUS_MASK;
    // we must set V_MASK in CS0, because CS0.CSMR.V_MASK act as a global CS
    FB->CS[0].CSMR |= FB_CSMR_V_MASK; 
    // clear registers
    FB->CS[FLEXBUS_InitStruct->CSn].CSCR = 0;
    // base address
    FB->CS[FLEXBUS_InitStruct->CSn].CSAR = FLEXBUS_InitStruct->baseAddress;
    // address space
    FB->CS[FLEXBUS_InitStruct->CSn].CSMR = FB_CSMR_BAM(FLEXBUS_InitStruct->ADSpaceMask) | FB_CSMR_V_MASK;
    //port size
    FB->CS[FLEXBUS_InitStruct->CSn].CSCR &= FB_CSCR_PS_MASK;
    FB->CS[FLEXBUS_InitStruct->CSn].CSCR |= FB_CSCR_PS(FLEXBUS_InitStruct->dataWidth);
    // AutoAcknogement(AA) Config
    if(FLEXBUS_InitStruct->autoAckMode == kFLEXBUS_AutoAckEnable)
    {
        FB->CS[FLEXBUS_InitStruct->CSn].CSCR |= FB_CSCR_AA_MASK;
    }
    else
    {
        FB->CS[FLEXBUS_InitStruct->CSn].CSCR &= ~FB_CSCR_AA_MASK;
    }
    // data align
    if(FLEXBUS_InitStruct->dataAlignMode == kFLEXBUS_DataLeftAligned)
    {
        FB->CS[FLEXBUS_InitStruct->CSn].CSCR &= ~FB_CSCR_BLS_MASK;
    }
    else
    {
        FB->CS[FLEXBUS_InitStruct->CSn].CSCR |= FB_CSCR_BLS_MASK;
    }
    // byte enable mode
    if(FLEXBUS_InitStruct->ByteEnableMode == kFLEXBUS_BE_AssertedWrite)
    {
        FB->CS[FLEXBUS_InitStruct->CSn].CSCR &= ~FB_CSCR_BEM_MASK;
    }
    else
    {
        FB->CS[FLEXBUS_InitStruct->CSn].CSCR |= FB_CSCR_BEM_MASK;
    }

}


void FLEXBUS_PortMuxConfig(uint32_t config)
{
    // CS Port Multiplexing Cotrol
    FB->CSPMCR = config;
}



#endif



















