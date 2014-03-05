/**
  ******************************************************************************
  * @file    adc.c
  * @author  YANDLD
  * @version V2.5
  * @date    2013.12.25
  * @brief   CH KinetisLib: http://github.com/yandld   http://upcmcu.taobao.com 
  ******************************************************************************
  */
  
#include "adc.h"
#include "gpio.h"
  
//!< Leagacy Support for Kineis Z Version(Inital Version)
#if (!defined(ADC_BASES))

    #if (defined(MK60DZ10))
        #define ADC_BASES {ADC0, ADC1}
    #endif
    #if (defined(MK10D5))
        #define ADC_BASES {ADC0}
    #endif
#endif
  
ADC_Type * const ADC_InstanceTable[] = ADC_BASES;
static ADC_CallBackType ADC_CallBackTable[ARRAY_SIZE(ADC_InstanceTable)] = {NULL};

#if (defined(MK60DZ10) || defined(MK60D10))
static const RegisterManipulation_Type SIM_ADCClockGateTable[] =
{
    {(void*)&(SIM->SCGC6), SIM_SCGC6_ADC0_MASK},
    {(void*)&(SIM->SCGC3), SIM_SCGC3_ADC1_MASK},
};
static const IRQn_Type ADC_IRQnTable[] = 
{
    ADC0_IRQn,
    ADC1_IRQn,
};
#elif (defined(MK70F12) || defined(MK70F15))
static const RegisterManipulation_Type SIM_ADCClockGateTable[] =
{
    {(void*)&(SIM->SCGC6), SIM_SCGC6_ADC0_MASK},
    {(void*)&(SIM->SCGC3), SIM_SCGC3_ADC1_MASK},
    {(void*)&(SIM->SCGC3), SIM_SCGC6_ADC2_MASK},
};
static const IRQn_Type ADC_IRQnTable[] = 
{
    ADC0_IRQn,
    ADC1_IRQn,
    ADC2_IRQn,
};
#elif (defined(MK10D5))
static const RegisterManipulation_Type SIM_ADCClockGateTable[] =
{
    {(void*)&(SIM->SCGC6), SIM_SCGC6_ADC0_MASK},
};
static const IRQn_Type ADC_IRQnTable[] = 
{
    ADC0_IRQn,
};
#endif

//! @defgroup CHKinetis
//! @{


//! @defgroup ADC(ģ��ת��ģ��)
//! @brief ADC API functions
//! @{

/**
 * @brief  �ж�ת���Ƿ����
 *         
 * @param  instance: ADC ģ���
 *         @arg HW_ADC0
 *         @arg HW_ADC1
 *         @arg ...
 * @param  mux: ADC ת����ͨ�� ���� ѡ��
 *         @arg kADC_MuxA
 *         @arg kADC_MuxB
 * @retval 
 *         @arg 0:ת�����
 *         @arg 1:ת��ʧ��
 */
int32_t ADC_IsConversionCompleted(uint32_t instance, uint32_t mux)
{
    return (((ADC_InstanceTable[instance]->SC1[mux]) & ADC_SC1_COCO_MASK) >> ADC_SC1_COCO_SHIFT);  
}

static int32_t ADC_Calibration(uint32_t instance)
{
    uint32_t PG, MG;
    ADC_InstanceTable[instance]->SC3 |= ADC_SC3_CALF_MASK; /* Clear the calibration's flag */
    ADC_InstanceTable[instance]->SC3 |= ADC_SC3_CAL_MASK;  /* Enable the calibration */
    ADC_ITDMAConfig(instance, kADC_MuxA, kADC_IT_EOF, DISABLE);
    while(ADC_IsConversionCompleted(instance, 0)) {};      /* Wait conversion is competed */
    if(ADC_InstanceTable[instance]->SC3 & ADC_SC3_CALF_MASK)
    {
        /* cal failed */
        return 1;
    }
    /* Calculate plus-side calibration */
    PG = 0U;
    PG += ADC_InstanceTable[instance]->CLP0;
    PG += ADC_InstanceTable[instance]->CLP1;
    PG += ADC_InstanceTable[instance]->CLP2;
    PG += ADC_InstanceTable[instance]->CLP3;
    PG += ADC_InstanceTable[instance]->CLP4;
    PG += ADC_InstanceTable[instance]->CLPS;
    PG = 0x8000U | (PG>>1U);
    
    /* Calculate minus-side calibration */
    MG = 0U;
    MG += ADC_InstanceTable[instance]->CLM0;
    MG += ADC_InstanceTable[instance]->CLM1;
    MG += ADC_InstanceTable[instance]->CLM2;
    MG += ADC_InstanceTable[instance]->CLM3;
    MG += ADC_InstanceTable[instance]->CLM4;
    MG += ADC_InstanceTable[instance]->CLMS;
    MG = 0x8000U | (MG>>1U);
    ADC_InstanceTable[instance]->SC3 &= ~ADC_SC3_CAL_MASK;  /* Disable the calibration */
    ADC_InstanceTable[instance]->PG = PG;
    ADC_InstanceTable[instance]->MG = MG;
    return 0;
}

/**
 * @brief  ͨ����ʼ���ṹ�� ��ʼ��ADCģ��
 *
 * @code
 *   ADC_InitStruct1.chl = 1; //1ͨ��
 *   ADC_InitStruct1.clockDiv = kADC_ClockDiv8; //ADCת��ʱ��Ϊ����ʱ��(Ĭ��BusClock) ��8��Ƶ
 *   ADC_InitStruct1.instance = HW_ADC0;
 *   ADC_InitStruct1.resolutionMode = kADC_SingleDiff8or9; //����ģʽ��8λ���� ���ģʽ��9λ����
 *   ADC_InitStruct1.SingleOrDifferential = kADC_Single;
 *   ADC_InitStruct1.triggerMode = kADC_TriggleSoftware;
 *   //��ʼ��ADCģ��
 *   ADC_Init(&ADC_InitStruct1);
 * @endcode
 * @param  ADC_InitStruct: ADC��ʼ���ṹ��
 * @retval 0:ת����� 1:ת��ʧ�ܻ�ת��������
 */
void ADC_Init(ADC_InitTypeDef* ADC_InitStruct)
{
    // enable clock gate
    uint32_t * SIM_SCGx = (void*) SIM_ADCClockGateTable[ADC_InitStruct->instance].register_addr;
    *SIM_SCGx |= SIM_ADCClockGateTable[ADC_InitStruct->instance].mask;
	// set clock configuratiom
	ADC_InstanceTable[ADC_InitStruct->instance]->CFG1 &= ~ADC_CFG1_ADICLK_MASK;
	ADC_InstanceTable[ADC_InitStruct->instance]->CFG1 |=  ADC_CFG1_ADICLK(ADC_InitStruct->clockDiv); 
    // resolutionMode
	ADC_InstanceTable[ADC_InitStruct->instance]->CFG1 &= ~(ADC_CFG1_MODE_MASK); 
	ADC_InstanceTable[ADC_InitStruct->instance]->CFG1 |= ADC_CFG1_MODE(ADC_InitStruct->resolutionMode);
    // trigger mode
    (kADC_TriggerHardware == ADC_InitStruct->triggerMode)?(ADC_InstanceTable[ADC_InitStruct->instance]->SC2 |=  ADC_SC2_ADTRG_MASK):(ADC_InstanceTable[ADC_InitStruct->instance]->SC2 &=  ADC_SC2_ADTRG_MASK);
    // do cal
    ADC_Calibration(ADC_InitStruct->instance);
}

/**
 * @brief  ���ٳ�ʼ��һ��ADCͨ��
 *
 * @code
 *    uint32_t instance;
 *    uint32_t value;
 *    //��ʼ�� ADC0 ͨ��20 ����DM1 ���� ���� 12λ
 *    instance = ADC_QuickInit(ADC0_SE20_DM1, kADC_SingleDiff12or13);
 *    //��ȡADת�����
 *    value = ADC_QuickReadValue(ADC0_SE20_DM1);
 *    printf("AD Value:%d\r\n", value);
 * @endcode
 * @param  ADCxMAP: ���ٳ�ʼ����
 * @param  resolutionMode: �ֱ���
 * @retval ADCģ���
 */
uint8_t ADC_QuickInit(uint32_t ADCxMAP, uint32_t resolutionMode)
{
    uint8_t i;
    QuickInit_Type * pADCxMap = (QuickInit_Type*)&(ADCxMAP);
    
    ADC_InitTypeDef AD_InitStruct1;
    AD_InitStruct1.instance = pADCxMap->ip_instance;
    AD_InitStruct1.chl = pADCxMap->channel;
    AD_InitStruct1.clockDiv = kADC_ClockDiv8;
    AD_InitStruct1.resolutionMode = resolutionMode;
    AD_InitStruct1.triggerMode = kADC_TriggleSoftware;
    AD_InitStruct1.SingleOrDifferential = kADC_Single;
    ADC_Init(&AD_InitStruct1);
    // init pinmux
    for(i = 0; i < pADCxMap->io_offset; i++)
    {
        PORT_PinMuxConfig(pADCxMap->io_instance, pADCxMap->io_base + i, (PORT_PinMux_Type) pADCxMap->mux);
        PORT_PinConfig(pADCxMap->io_instance, pADCxMap->io_base + i, kPullDisabled, DISABLE);
    }
    return pADCxMap->ip_instance;
}
  


void ADC_StartConversion(uint32_t instance, uint32_t chl, uint32_t mux)
{
    if(kADC_MuxA ==  mux)
    {
        ADC_InstanceTable[instance]->CFG2 &= ~ADC_CFG2_MUXSEL_MASK;
    }
    if(kADC_MuxB ==  mux)
    {
        ADC_InstanceTable[instance]->CFG2 |= ADC_CFG2_MUXSEL_MASK; 
    }
    ADC_InstanceTable[instance]->SC1[mux] &= ~(ADC_SC1_ADCH_MASK);
    ADC_InstanceTable[instance]->SC1[mux] |= ADC_SC1_ADCH(chl);

}

/**
 * @brief  ��ȡADCת������(non-block���� �������ض�ȡ���)
 *         
 * @param  instance: ADC ģ���
 *         @arg HW_ADC0
 *         @arg HW_ADC1
 *         @arg ...
 * @param  mux: ADC ת����ͨ�� ���� ѡ��
 *         @arg kADC_MuxA
 *         @arg kADC_MuxB
 * @retval :��ȡ��� �����ǰ��δ���ת�� �򷵻���һ�ν��
 */
int32_t ADC_ReadValue(uint32_t instance, uint32_t mux)
{
    return ADC_InstanceTable[instance]->R[mux];
}

/**
 * @brief  ��ȡADCת����� ֻ������ADC���ٳ�ʼ���꼴��
 *         
 * @param  ADCxMAP: ADC ���ٳ�ʼ����
 * @retval ת�����
 */
int32_t ADC_QuickReadValue(uint32_t ADCxMAP)
{
    QuickInit_Type * pADCxMap = (QuickInit_Type*)&(ADCxMAP);
    uint32_t instance = pADCxMap->ip_instance;
    uint32_t chl = pADCxMap->channel;
    uint32_t mux = pADCxMap->reserved;
    ADC_StartConversion(instance, chl, mux);
    // waiting for ADC complete
    while(ADC_IsConversionCompleted(instance, mux)) {};
    return ADC_ReadValue(instance, mux);
}

/**
 * @brief  ADC �жϼ�DMA����
 *         
 * @param  instance: ADC ģ���     
 * @param  mux:      ADC ͨ������ѡ��
 * @param  config:   ADC �жϼ�DMA����
 * @param  newStateg:   ADC �жϼ�DMA����
 *         @arg ENABLE:ʹ��
 *         @arg DISABLE:��ֹ
 * @retval None
 */
void ADC_ITDMAConfig(uint8_t instance, uint32_t mux, ADC_ITDMAConfig_Type config, FunctionalState newState)
{
    switch(config)
    {
        case kADC_ITDMA_Disable:
            NVIC_DisableIRQ(ADC_IRQnTable[instance]);
            ADC_InstanceTable[instance]->SC1[mux] &= ~ADC_SC1_AIEN_MASK;
            ADC_InstanceTable[instance]->SC2 &= ~ADC_SC2_DMAEN_MASK;
            break;
        case kADC_IT_EOF:
            (ENABLE == newState)?(ADC_InstanceTable[instance]->SC1[mux] |= ADC_SC1_AIEN_MASK):(ADC_InstanceTable[instance]->SC1[mux] &= ~ADC_SC1_AIEN_MASK);
            (ENABLE == newState)?(NVIC_EnableIRQ(ADC_IRQnTable[instance])):(NVIC_DisableIRQ(ADC_IRQnTable[instance]));
            break;
        case kADC_DMA_EOF:
            (ENABLE == newState)?(ADC_InstanceTable[instance]->SC2 |= ADC_SC2_DMAEN_MASK):(ADC_InstanceTable[instance]->SC2 &= ~ADC_SC2_DMAEN_MASK);
            break;
        default:
            break;
    }        
}

 
/**
 * @brief  ��װADC �жϻص�����
 *
 * @param  instance: ADCģ���
 *         @arg HW_ADC0
 *         @arg HW_ADC1
 *         @arg ...
 * @param  AppCBFun: �û��жϻص�����
 * @retval None
 */
void ADC_CallbackInstall(uint8_t instance, ADC_CallBackType AppCBFun)
{
    if(AppCBFun != NULL)
    {
        ADC_CallBackTable[instance] = AppCBFun;
    }
}

//! @}

//! @}

void ADC0_IRQHandler(void)
{
    uint32_t dummy;
    /* clear COCO bit */
    dummy = ADC_InstanceTable[HW_ADC0]->R[kADC_MuxA];
    dummy = ADC_InstanceTable[HW_ADC0]->R[kADC_MuxB];
    dummy = dummy; /* avoid compiler warnning */
    if(ADC_CallBackTable[HW_ADC0] != NULL)
    {
        ADC_CallBackTable[HW_ADC0]();
    }
    
}

void ADC1_IRQHandler(void)
{
    uint32_t dummy;
    /* clear COCO bit */
    dummy = ADC_InstanceTable[HW_ADC1]->R[kADC_MuxA];
    dummy = ADC_InstanceTable[HW_ADC1]->R[kADC_MuxB];
    dummy = dummy; /* avoid compiler warnning */
    if(ADC_CallBackTable[HW_ADC1] != NULL)
    {
        ADC_CallBackTable[HW_ADC1]();
    } 
}