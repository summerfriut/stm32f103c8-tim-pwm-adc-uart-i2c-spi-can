#ifndef __MY_CAN_H__
#define __MY_CAN_H__
#include "can.h"
#include "my_cfg.h"

#ifdef USE_CAN1
extern CAN_RxHeaderTypeDef CAN1_Rx_Head;    //存放接收到的数据帧属性信息
extern CAN_TxHeaderTypeDef CAN1_Tx_Head;
extern uint8_t             can1_rx_data[8];
extern uint8_t             can1_tx_data[8];
#endif

#ifdef USE_CAN2
extern CAN_RxHeaderTypeDef CAN2_Rx_Head;    //存放接收到的数据帧属性信息
extern CAN_TxHeaderTypeDef CAN2_Tx_Head;
extern uint8_t             can2_rx_data[8];
extern uint8_t             can2_tx_data[8];
#endif

typedef enum
{
    CAN_PORT1,
    CAN_PORT2
}__can_port;

typedef struct
{
    CAN_HandleTypeDef*      hcan;
    CAN_RxHeaderTypeDef*    rx_head;    //存放接收到的数据帧属性信息
    CAN_TxHeaderTypeDef*    tx_head;
    uint8_t*                rx_data;
    uint8_t*                tx_data;
    uint8_t                 port;
}__can_info;
extern __can_info   can_info[];

void can_filter_cfg(uint8_t port);
void can_tx_msg(uint8_t port,uint32_t canId,uint8_t* pdata,uint16_t data_Len);


#endif



