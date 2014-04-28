/**
  ******************************************************************************
  * @file    tsi.h
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.26
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  * @note    此文件为芯片TSI模块的底层功能函数
  ******************************************************************************
  */
#ifndef __CH_LIB_TSI_H__
#define __CH_LIB_TSI_H__

#ifdef __cplusplus
 extern "C" {
#endif
     
#include <stdint.h>
  
//!< TSI 触发模式选择
typedef enum
{
    kTSI_TriggerSoftware,     //软件触发
    kTSI_TriggerPeriodicalScan,//硬件周期性扫描
} TSI_TriggerMode_Type;
  
#define HW_TSI_CH0      (0x00)  //触控的0通道
#define HW_TSI_CH1      (0x01)  //触控的1通道
#define HW_TSI_CH2      (0x02)  //触控的2通道
#define HW_TSI_CH3      (0x03)  //触控的3通道
#define HW_TSI_CH4      (0x04)  //触控的4通道
#define HW_TSI_CH5      (0x05)  //触控的5通道
#define HW_TSI_CH6      (0x06)  //触控的6通道
#define HW_TSI_CH7      (0x07)  //触控的7通道
#define HW_TSI_CH8      (0x08)  //触控的8通道
#define HW_TSI_CH9      (0x09)  //触控的9通道
#define HW_TSI_CH10     (0x0A)  //触控的10通道
#define HW_TSI_CH11     (0x0B)
#define HW_TSI_CH12     (0x0C)
#define HW_TSI_CH13     (0x0D)
#define HW_TSI_CH14     (0x0E)
#define HW_TSI_CH15     (0x0F)  //触控的15通道


//!< TSI 中断DMA配置
typedef enum
{
    kTSI_IT_Disable,        //关闭中断
    kTSI_IT_OutOfRange,     //溢出中断
    kTSI_IT_EndOfScan,      //扫描结束中断
}TSI_ITDMAConfig_Type;

//TSI初始化结构
typedef struct
{
    uint32_t                chl;            //通道号1~15
    TSI_TriggerMode_Type    triggerMode;    //触发模式选择
    uint32_t                threshld;       //发生超范围中断时的判断阀值
}TSI_InitTypeDef;

//!< TSI QuickInit macro
#define TSI0_CH0_PB00   (0x00004008)  //0通道的PTB0引脚 以下类推
#define TSI0_CH1_PA00   (0x00084000)  
#define TSI0_CH2_PA01   (0x00104200)
#define TSI0_CH3_PA02   (0x00184400)
#define TSI0_CH4_PA03   (0x00204600)
#define TSI0_CH5_PA04   (0x00284800)
#define TSI0_CH6_PB01   (0x00304208)
#define TSI0_CH7_PB02   (0x00384408)
#define TSI0_CH8_PB03   (0x00404608)
#define TSI0_CH9_PB16   (0x00486008)
#define TSI0_CH10_PB17  (0x00506208)
#define TSI0_CH11_PB18  (0x00586408)
#define TSI0_CH12_PB19  (0x00606608)
#define TSI0_CH13_PC00  (0x00684010)
#define TSI0_CH14_PC01  (0x00704210)
#define TSI0_CH15_PC02  (0x00784410)


//!< TSI CallBack Type
typedef void (*TSI_CallBackType)(void);

//!< API functions
uint32_t TSI_QuickInit(uint32_t UARTxMAP);
uint32_t TSI_GetCounter(uint32_t chl);
void TSI_ITDMAConfig(TSI_ITDMAConfig_Type config);
void TSI_Init(TSI_InitTypeDef* TSI_InitStruct);
void TSI_CallbackInstall(TSI_CallBackType AppCBFun);

#ifdef __cplusplus
}
#endif

#endif

