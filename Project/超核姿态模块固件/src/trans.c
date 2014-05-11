#include "trans.h"
#include <string.h>
#include "dma.h"
#include "nrf24l01.h"
#include "board.h"
#include "uart.h"
#include "common.h"

//数据头
const char trans_header_table[3] = {0x88, 0xAF, 0x1C};
//数据内容
typedef __packed struct
{
    uint8_t trans_header[3];
    trans_user_data_t user_data;
    uint8_t trans_reserved[4];
    uint8_t sum;
}trans_packet_t;

trans_packet_t packet;

//安装回调函数
uint8_t trans_init(void)
{
    
    DMA_InitTypeDef DMA_InitStruct1= {0};
    DMA_InitStruct1.chl = HW_DMA_CH1;
    DMA_InitStruct1.chlTriggerSource = UART1_TRAN_DMAREQ;
    DMA_InitStruct1.triggerSourceMode = kDMA_TriggerSource_Normal;
    DMA_InitStruct1.majorLoopCnt = sizeof(packet);
    DMA_InitStruct1.minorLoopByteCnt = 1;
    
    DMA_InitStruct1.sAddr = (uint32_t)&packet;
    DMA_InitStruct1.sLastAddrAdj = -sizeof(packet);
    DMA_InitStruct1.sAddrOffset = 1;
    DMA_InitStruct1.sDataWidth = kDMA_DataWidthBit_8;
    DMA_InitStruct1.sMod = kDMA_ModuloDisable;
        
    DMA_InitStruct1.dAddr = (uint32_t)&UART1->D;
    DMA_InitStruct1.dLastAddrAdj = 0;
    DMA_InitStruct1.dAddrOffset = 0;
    DMA_InitStruct1.dDataWidth = kDMA_DataWidthBit_8;
    DMA_InitStruct1.dMod = kDMA_ModuloDisable;
    DMA_Init(&DMA_InitStruct1);
    return 0;
}


//发送数据包
uint32_t trans_send_pactket(trans_user_data_t data, uint8_t mode)
{
    uint8_t i;
    uint8_t *p = (uint8_t*)&packet;
    trans_packet_t packet_copy;
    uint8_t sum = 0;
    memcpy(packet.trans_header, trans_header_table, sizeof(trans_header_table));
    for(i=0;i<3;i++)
    {
        data.trans_accel[i] = __REV16(data.trans_accel[i]);
        data.trans_gyro[i] = __REV16(data.trans_gyro[i]);
        data.trans_mag[i] = __REV16(data.trans_mag[i]);
    }
    data.trans_yaw = __REV16(data.trans_yaw);
    data.trans_pitch = __REV16(data.trans_pitch);
    data.trans_roll = __REV16(data.trans_roll);
    memcpy(&packet.user_data, &data, sizeof(data));
    memset(packet.trans_reserved,0,sizeof(packet.trans_reserved));
    for(i = 0; i < sizeof(packet) - 1;i++)
    {
      sum += *p++;
    }
    packet.sum = sum;
    packet_copy = packet;
    switch(mode)
    {
        case TRANS_UART_WITH_DMA:
            UART_ITDMAConfig(BOARD_UART_INSTANCE, kUART_DMA_Tx);
            DMA_EnableRequest(HW_DMA_CH1);
            
            break;
        case TRANS_WITH_NRF2401:
            nrf24l01_set_tx_mode();
            nrf24l01_write_packet((uint8_t*)&packet_copy, sizeof(packet_copy));
            nrf24l01_set_rx_mode();
            break;
        default:
            break;
        
    }
    return 0;
}

