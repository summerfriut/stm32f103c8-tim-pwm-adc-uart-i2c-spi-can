#include "my_can.h"
#include "string.h"
#include "my_cfg.h"



#ifdef USE_CAN1
CAN_RxHeaderTypeDef CAN1_Rx_Head;    //存放接收到的数据帧属性信息
CAN_TxHeaderTypeDef CAN1_Tx_Head;
uint8_t             can1_rx_data[8];
uint8_t             can1_tx_data[8];
#endif

#ifdef USE_CAN2
CAN_RxHeaderTypeDef CAN2_Rx_Head;    //存放接收到的数据帧属性信息
CAN_TxHeaderTypeDef CAN2_Tx_Head;
uint8_t             can2_rx_data[8];
uint8_t             can2_tx_data[8];
#endif

__can_info   can_info[] = 
{
    #ifdef USE_CAN1
    {.hcan = &hcan, .rx_head = &CAN1_Rx_Head, .tx_head = &CAN1_Tx_Head, .rx_data = can1_rx_data, .tx_data = can1_tx_data}
    #endif
    #ifdef USE_CAN2
    {.hcan = &hcan2, .rx_head = &CAN2_Rx_Head, .tx_head = &CAN2_Tx_Head, .rx_data = can2_rx_data, .tx_data = can2_tx_data}
    #endif
};

uint8_t can_port2index(uint8_t port)
{
    uint8_t index;
    for(index = 0 ; index < sizeof(can_info) / sizeof(__can_info) ; index++)
    {
        if(can_info[index].port == port)
            return index;
    }
    return 0xFF;                //ERR
}

uint8_t can_handle2index(CAN_HandleTypeDef *hcan)
{
    uint8_t index;
    for(index = 0 ; index < sizeof(can_info) / sizeof(__can_info) ; index++)
    {
        if(can_info[index].hcan == hcan)
            return index;
    }
    return 0xFF;                //ERR
}

void can_filter_cfg(uint8_t port)
{
    CAN_FilterTypeDef sFilterConfig;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000; 
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh =0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    sFilterConfig.FilterActivation = CAN_FILTER_ENABLE;
    sFilterConfig.FilterBank = 0;
    HAL_CAN_ConfigFilter(&hcan,&sFilterConfig);         //配置过滤器
    //开启FIFO0接收中断
    HAL_CAN_ActivateNotification(&hcan,CAN_IT_RX_FIFO0_MSG_PENDING);
    HAL_CAN_Start(&hcan);
}

void can_tx_msg(uint8_t port,uint32_t canId,uint8_t* pdata,uint16_t data_Len)
{
    uint32_t start_time;
    uint32_t TxMailbox;
    uint16_t i;
    uint8_t index = can_port2index(port);
    if(IS_INDEX(index) == pdFAIL)
        return;
    //设定canId
    if((canId & 0xFFFFFC00) != 0)                         //the id is extId
    {
        can_info[index].tx_head->StdId = 0;
        can_info[index].tx_head->ExtId = canId;
        can_info[index].tx_head->RTR   = CAN_RTR_DATA;
        can_info[index].tx_head->IDE   = CAN_ID_EXT;
    } 
    else
    {
        can_info[index].tx_head->StdId = (uint16_t)canId;
        can_info[index].tx_head->ExtId = 0x00;
        can_info[index].tx_head->RTR   = CAN_RTR_DATA;
        can_info[index].tx_head->IDE   = CAN_ID_STD;
    }
    for(i = 0 ; i < data_Len / 8 ; i++)
    {
        start_time = HAL_GetTick();
        can_info[index].tx_head->DLC = 8;
        while(!HAL_CAN_GetTxMailboxesFreeLevel(&hcan))
        {
            if(HAL_GetTick() - start_time > 2000)
                return;
        }
        HAL_CAN_AddTxMessage(&hcan, can_info[index].tx_head, pdata+i*8, &TxMailbox);
        data_Len -= 8;
    }
    if(data_Len % 8)
    {
        can_info[index].tx_head->DLC = data_Len % 8;
        start_time = HAL_GetTick();
        while(!HAL_CAN_GetTxMailboxesFreeLevel(&hcan))
        {
            if(HAL_GetTick() - start_time > 2000)
                return;
        }
        HAL_CAN_AddTxMessage(&hcan, can_info[index].tx_head, pdata+i*8, &TxMailbox);
    }
}

//接收数据中断处理
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    BaseType_t xHigherPriorityTaskWoken,xResult;
    uint8_t index = can_handle2index(hcan);
    if(IS_INDEX(index) == pdFAIL)
        return;
    xHigherPriorityTaskWoken = pdFALSE;
    //获取接收到的数据
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, can_info[index].rx_head, can_info[index].rx_data);
    
    if(!myBinarySem07Handle)
        return;
    //释放信号量，开启can接收数据任务处理
    xResult = xSemaphoreGiveFromISR(myBinarySem07Handle,&xHigherPriorityTaskWoken);                   
    if(xResult != pdFAIL)
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken); 
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
}

void HAL_CAN_SleepCallback(CAN_HandleTypeDef *hcan)
{
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
    MX_CAN_Init();
    can_filter_cfg(CAN_PORT1);
}



