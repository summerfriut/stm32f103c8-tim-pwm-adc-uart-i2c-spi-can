#include "my_usart.h"
#include "string.h"
#include "stdio.h"

#ifdef USE_UART1
uint8_t uart1_rx_buf[UART1_RX_BUF_SIZE];
uint8_t uart1_tx_buf[UART1_TX_BUF_SIZE];
#endif
#ifdef USE_UART2
uint8_t uart2_rx_buf[UART2_RX_BUF_SIZE];
uint8_t uart2_tx_buf[UART2_TX_BUF_SIZE];
#endif
#ifdef USE_UART3
uint8_t uart3_rx_buf[UART3_RX_BUF_SIZE];
uint8_t uart3_tx_buf[UART3_TX_BUF_SIZE];
#endif
#ifdef USE_UART4
uint8_t uart4_rx_buf[UART4_RX_BUF_SIZE];
uint8_t uart4_tx_buf[UART4_TX_BUF_SIZE];
#endif
#ifdef USE_UART5
uint8_t uart5_rx_buf[UART5_RX_BUF_SIZE];
uint8_t uart5_tx_buf[UART5_TX_BUF_SIZE];
#endif

#ifdef UART1_EN485
    __ctl_485    ctl1_485 = {.EN485_GPIO_Port = USART1_TxEN_GPIO_Port, .EN485_Pin = USART1_TxEN_Pin, .EN485_RX = GPIO_PIN_SET};
#endif
#ifdef UART2_EN485
    __ctl_485    ctl2_485 = {.EN485_GPIO_Port = USART2_TxEN_GPIO_Port, .EN485_Pin = USART2_TxEN_Pin, .EN485_RX = GPIO_PIN_SET};
#endif
#ifdef UART3_EN485
    __ctl_485    ctl3_485 = {.EN485_GPIO_Port = USART3_TxEN_GPIO_Port, .EN485_Pin = USART3_TxEN_Pin, .EN485_RX = GPIO_PIN_SET};
#endif
#ifdef UART4_EN485
    __ctl_485    ctl4_485 = {.EN485_GPIO_Port = USART4_TxEN_GPIO_Port, .EN485_Pin = USART4_TxEN_Pin, .EN485_RX = GPIO_PIN_SET};
#endif
#ifdef UART5_EN485
    __ctl_485    ctl5_485 = {.EN485_GPIO_Port = USART5_TxEN_GPIO_Port, .EN485_Pin = USART5_TxEN_Pin, .EN485_RX = GPIO_PIN_SET};
#endif

__usart_info     usart_info[] = 
{
#ifdef USE_UART1
    {.huart = &huart1, .hdma_usart_rx = &hdma_usart1_rx, .hdma_usart_tx = &hdma_usart1_tx, .recv_buf = uart1_rx_buf, .recv_buf_size = UART1_RX_BUF_SIZE,\
    .send_buf_size = UART1_TX_BUF_SIZE, .send_buf = uart1_tx_buf, .usart_switch = 1, .port = UART_PORT1, .send_message = usart_send, .ctl_485 = NULL},
#endif
#ifdef USE_UART2
    {.huart = &huart2, .hdma_usart_rx = &hdma_usart2_rx, .hdma_usart_tx = &hdma_usart2_tx, .recv_buf = uart2_rx_buf, .recv_buf_size = UART2_RX_BUF_SIZE,\
    .send_buf_size = UART2_TX_BUF_SIZE, .send_buf = uart2_tx_buf, .usart_switch = 1, .port = UART_PORT2, .send_message = usart_send, .ctl_485 = NULL},
#endif
#ifdef USE_UART3
    {.huart = &huart3, .hdma_usart_rx = NULL, .hdma_usart_tx = NULL, .recv_buf = uart3_rx_buf, .recv_buf_size = UART3_RX_BUF_SIZE,\
    .send_buf_size = UART3_TX_BUF_SIZE, .send_buf = uart3_tx_buf, .usart_switch = 1, .port = UART_PORT3, .send_message = usart_send, .ctl_485 = NULL},
#endif
};

                                        
//���ݴ��ںŻ�ȡ���ھ��                                        
uint8_t uart_port2handle(uint8_t port, UART_HandleTypeDef** huart_p)
{
    *huart_p = NULL;
    uint8_t index;
    for(index = 0 ; index < sizeof(usart_info) / sizeof(__usart_info) ; index++)
    {
        if(usart_info[index].port == port)
        {
            *huart_p = usart_info[index].huart;
            return index;
        }
    }
    return 0xFF;            //ERR
}

uint8_t uart_handle2port(UART_HandleTypeDef* huart)
{
    uint8_t index;
    for(index = 0 ; index < sizeof(usart_info) / sizeof(__usart_info) ; index++)
    {
        if(usart_info[index].huart == huart)
            return usart_info[index].port;
    }
    return 0xFF;            //ERR
}

uint8_t uart_port2index(uint8_t port)
{
    uint8_t index;
    for(index = 0 ; index < sizeof(usart_info) / sizeof(__usart_info) ; index++)
    {
        if(port == usart_info[index].port)
            return index;
    }
    return 0xFF;            //ERR
}

uint8_t uart_handle2index(UART_HandleTypeDef* huart)
{
    uint8_t index;
    for(index = 0 ; index < sizeof(usart_info) / sizeof(__usart_info) ; index++)
    {
        if(huart == usart_info[index].huart)
            return index;
    }
    return 0xFF;            //ERR
}


inline void calculate_communication_quality(uint8_t port)
{
    uint8_t rx_times,tx_times;
    uint8_t index = uart_port2index(port);
    if(IS_INDEX(index) == pdFAIL)
        return;
    if(usart_info[index].usart_tx_times++ == USART_TX_TIMES_BASE)                  //��¼��������Ĵ���(��ǰΪ��201(1)��)
    {
        rx_times = usart_info[index].usart_rx_times;
        tx_times = usart_info[index].usart_tx_times;
        usart_info[index].usart_communication_quality = rx_times / tx_times;
        if(usart_info[index].usart_communication_quality * 100 > USART_COMM_QUALITY_MIN)
            usart_info[index].usart_missflag = 0;
        else
            usart_info[index].usart_missflag = 1;
        usart_info[index].usart_tx_times = 0;
        usart_info[index].usart_rx_times = 0;
    }
}

//���ô��ڲ���
uint8_t my_uart_para_cfg(uint8_t port)
{
    uint8_t index = uart_port2index(port);
    if(IS_INDEX(index) == pdFAIL)
        return 1;
    UART_HandleTypeDef* uartHandle;
    __usart_info*       usart_info_tmp = &usart_info[index];
    __ctl_485*          ctl_485 = usart_info_tmp->ctl_485;
//    //���ؿվ��
//    if(uart_port2handle(port, &uartHandle))
//        return 1;
    uartHandle = usart_info_tmp->huart;
    if(!uartHandle)
        return 1;
    //���IDEL�ж�
    __HAL_UART_CLEAR_IDLEFLAG(uartHandle);
    //ʹ�ܴ��ڿ����ж�
    __HAL_UART_ENABLE_IT(uartHandle,UART_IT_IDLE);
    if(uartHandle->hdmarx == NULL)
        __HAL_UART_ENABLE_IT(uartHandle,UART_IT_RXNE);
    if(uartHandle->hdmatx == NULL)
        __HAL_UART_ENABLE_IT(uartHandle,UART_IT_TC);
    //ʹ�ܴ���
    __HAL_UART_ENABLE(uartHandle);
    //ʹ��DMA
    if(uartHandle->hdmarx != NULL)
        HAL_UART_Receive_DMA(uartHandle,usart_info_tmp->recv_buf,usart_info_tmp->recv_buf_size);
    else
        HAL_UART_Receive_IT(uartHandle,usart_info_tmp->recv_buf,usart_info_tmp->recv_buf_size);
    //Ĭ�Ͽ���485����
    if(ctl_485)
        HAL_GPIO_WritePin(ctl_485->EN485_GPIO_Port,ctl_485->EN485_Pin,ctl_485->EN485_RX);
    return 0;
}

//��ʼ����������,ʹ��485����
void my_uart_init()
{
    uint8_t index;
    #ifdef USE_UART1
    if(my_uart_para_cfg(UART_PORT1))
    {
        //��ʼ������,��ӡ������Ϣ
    }
    index = uart_port2index(UART_PORT1);
    if(IS_INDEX(index) == pdFAIL)
        return;
    if(!usart_info[index].usart_switch)
        usart_info[index].usart_missflag = 1; 
    #endif
    #ifdef USE_UART2
    if(my_uart_para_cfg(UART_PORT2))
    {
        //��ʼ������,��ӡ������Ϣ
    }
    index = uart_port2index(UART_PORT2);
    if(IS_INDEX(index) == pdFAIL)
        return;
    if(!usart_info[index].usart_switch)
        usart_info[index].usart_missflag = 1; 
    #endif
    #ifdef USE_UART3
    if(my_uart_para_cfg(UART_PORT3))
    {
        //��ʼ������,��ӡ������Ϣ
    }
    index = uart_port2index(UART_PORT3);
    if(IS_INDEX(index) == pdFAIL)
        return;
    if(!usart_info[index].usart_switch)
        usart_info[index].usart_missflag = 1; 
    #endif
    #ifdef USE_UART4
    if(my_uart_para_cfg(UART_PORT4))
    {
        //��ʼ������,��ӡ������Ϣ
    }
    index = uart_port2index(UART_PORT4);
    if(IS_INDEX(index) == pdFAIL)
        return;
    if(!usart_info[index].usart_switch)
        usart_info[index].usart_missflag = 1; 
    #endif
    #ifdef USE_UART5
    if(my_uart_para_cfg(UART_PORT5))
    {
        //��ʼ������,��ӡ������Ϣ
    }
    index = uart_port2index(UART_PORT5);
    if(IS_INDEX(index) == pdFAIL)
        return;
    if(!usart_info[index].usart_switch)
        usart_info[index].usart_missflag = 1; 
    #endif
}

//�޸Ĵ���bradrate
void uart_change_bradrate(uint8_t port,uint32_t bradrate,uint8_t word_len,uint8_t stop_bit,uint8_t parity)
{
    UART_HandleTypeDef* uartHandle;
    //���ؿվ��
    if(uart_port2handle(port, &uartHandle))
        return;
    uartHandle->Init.BaudRate = bradrate;
    uartHandle->Init.WordLength = word_len;
    uartHandle->Init.StopBits = stop_bit;
    uartHandle->Init.Parity = parity;
    uartHandle->Init.Mode = UART_MODE_TX_RX;
    uartHandle->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    uartHandle->Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(uartHandle) != HAL_OK)
    {
        Error_Handler();
    }
}

//ͨ������uart_num(1~..)����msg_len���ֽڵ�����pdata
//pdataָ���������Ϊstatic����(������Ҫ�޸Ĳ��ַ���DMA��ʱ)
//��ʹ�øú���ʱ����ͬ������ָ��PERIPHERAL_ID���ڽ������ʱͨ��IDȷ�����ݵĽ�������
void uart_send_msg(uint8_t port,uint8_t* pdata,uint8_t msg_len)
{
    uint8_t index = uart_port2index(port);
    if(IS_INDEX(index) == pdFAIL)
        return;
    usart_info[index].send_buf_len = msg_len;
    memcpy(usart_info[index].send_buf,pdata,msg_len);
    usart_info[index].send_message(port);
}

//�������ݷ��ͺ���
//������messageͨ��serial_port���ڷ���message_len���ֽ�
void usart_send(uint8_t port)
{
    UART_HandleTypeDef* uartHandle;
    uint8_t index = uart_port2index(port);
    if(IS_INDEX(index) == pdFAIL)
        return;
    //������ݹ�����ͨ����̬�ڴ����ʵ�����ݵĻ���,���������Ӧ����ʱ�ͷŶ���,���߲��ñ�־λ��֤�����ظ��������
    if(usart_info[index].send_buf_len > usart_info[index].send_buf_size)
        usart_info[index].send_buf_len = usart_info[index].send_buf_size;
    __ctl_485*          ctl_485;
//    //���ؿվ��
//    if(uart_port2handle(port, &uartHandle))
//        return;
    uartHandle = usart_info[index].huart;
    if(!uartHandle)
        return;
    ctl_485 = usart_info[index].ctl_485;
    //ʹ��485����
    if(ctl_485)
    {
        HAL_GPIO_WritePin(ctl_485->EN485_GPIO_Port,ctl_485->EN485_Pin,(GPIO_PinState)!ctl_485->EN485_RX);
        HAL_Delay(1);                                   //����ĳ485оƬbug��TTL�����������ʱ
    }
    if(uartHandle->hdmatx)
        HAL_UART_Transmit_DMA(uartHandle,usart_info[index].send_buf,usart_info[index].send_buf_len);
    else
        HAL_UART_Transmit_IT(uartHandle,usart_info[index].send_buf,usart_info[index].send_buf_len);
}

//������ɻص�,�ָ�485оƬΪ����,
//ʹ��DMAʱ��UART_DMATransmitCplt()�������ô˺���
//ʹ��DMAʱ,HAL_UART_Transmit_DMA()��������ʱ����ע�ᷢ����ɻص�����ΪUART_DMATransmitCplt()
//huart->hdmatx->XferCpltCallback = UART_DMATransmitCplt;��hdma_usart1_tx.XferCpltCallback = UART_DMATransmitCplt;
//��HAL_DMA_IRQHandler() -->  huart->hdmatx->XferCpltCallback  -->  HAL_UART_TxCpltCallback()
//��ʹ��DMAʱ��UART_EndTransmit_IT()�������ô˺�����HAL_UART_IRQHandler()��������ô˺���
//���ͨ��DMA��������dma����жϵ���
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(__HAL_UART_GET_FLAG(huart,UART_FLAG_TC) != RESET)    //UART_FLAG_TC == USART_ISR_TC
    {
        __HAL_UART_CLEAR_FLAG(huart,UART_FLAG_TC);
        //��Ƿ������,���Խ����´η���
        
        uint8_t port = uart_handle2port(huart);
        //����ͨѶ����
        calculate_communication_quality(port);
        #ifdef USE_UART1 
        if(huart == &huart1)
        {
            #ifdef UART1_EN485
            HAL_GPIO_WritePin(EN1_485_GPIO_Port,EN1_485_Pin,GPIO_PIN_SET);
            #endif
        }
        #endif
        #ifdef USE_UART2 
        if(huart == &huart2)
        {
            #ifdef UART2_EN485
            HAL_GPIO_WritePin(EN2_485_GPIO_Port,EN2_485_Pin,GPIO_PIN_RESET);
            #endif
        }
        #endif
        #ifdef USE_UART3 
        if(huart == &huart3)
        {
            #ifdef UART3_EN485
            HAL_GPIO_WritePin(EN3_485_GPIO_Port,EN3_485_Pin,GPIO_PIN_RESET);
            #endif
        }
        #endif
        #ifdef USE_UART4 
        if(huart == &huart4)
        {
            #ifdef UART4_EN485
            HAL_GPIO_WritePin(EN4_485_GPIO_Port,EN4_485_Pin,GPIO_PIN_SET);
            #endif
        }
        #endif
        #ifdef USE_UART5 
        if(huart == &huart5)
        {
            #ifdef UART5_EN485
            HAL_GPIO_WritePin(EN5_485_GPIO_Port,EN5_485_Pin,GPIO_PIN_RESET);
            #endif
        }
        #endif
    }
}

//������ɻص�����(���յ�ָ���Ľ������ݳ���)
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
}

//���ڽ��������жϴ�����,�ɺ���USART1_IRQHandler����
//extern inline void my_usart_rx_IRQ(UART_HandleTypeDef* huart,StreamBufferHandle_t stream_buf,osSemaphoreId_t myBinarySem,uint8_t serial_port);
inline void my_usart_rx_IRQ(UART_HandleTypeDef* huart,StreamBufferHandle_t stream_buf,osSemaphoreId_t myBinarySem)
{
    uint8_t index = uart_handle2index(huart);
    if(IS_INDEX(index) == pdFAIL)
        return;
    if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) == RESET))
        return;
    portBASE_TYPE pxHigherPriorityTaskWoken = pdFALSE;
    //��������˿����жϣ�����������жϱ�־λ
    __HAL_UART_CLEAR_IDLEFLAG(huart);
    if(huart->hdmarx)
        usart_info[index].recv_buf_len = huart->RxXferSize - __HAL_DMA_GET_COUNTER(huart->hdmarx);
    else
        usart_info[index].recv_buf_len = huart->RxXferSize - huart->RxXferCount;
    if(usart_info[index].recv_buf_len)
    {
        if(huart->hdmarx)
            HAL_UART_DMAStop(huart);            //DMAָ��ָ��Ļ���������ָ�븴λ
        else
            HAL_UART_AbortReceive_IT(huart);    //�ж���д���ݵĻ���������ָ�븴λ
        if(stream_buf)
            xStreamBufferSendFromISR(stream_buf, \
                                    usart_info[index].recv_buf, \
                                    usart_info[index].recv_buf_len, \
                                    &pxHigherPriorityTaskWoken);
        if(myBinarySem)
        {
            xSemaphoreGiveFromISR(myBinarySem,&pxHigherPriorityTaskWoken);                          //�ͷ��ź��������������������ݴ���
            portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);                                          //�ж���ɺ����һ�������л�
        }
    }
    if(huart->hdmarx)
        //�����µ�DMA���ݽ���
        HAL_UART_Receive_DMA(huart,usart_info[index].recv_buf,usart_info[index].recv_buf_size);
    else
        HAL_UART_Receive_IT(huart,usart_info[index].recv_buf,usart_info[index].recv_buf_size);
}




//�Խ��յ������ݽ���У��,�Ȳ���
void uart_msg_check(__usart_info* usart_info_point)
{
#ifdef CRC_16
    if(CrcCalculate(usart_info_point->recv_buf,usart_info_point->recv_buf_len,0xffff) == 0)
        usart_info_point->crc_effctive = 1;     //����У����Ч
    else
        usart_info_point->crc_effctive = 0;     //������Ч
#endif
}

//������������ȡ�ź������
osSemaphoreId_t uart2sem_handle(uint8_t port)
{
    switch(port)
    {
        case UART_PORT1:
            return myBinarySem01Handle;
        case UART_PORT2:
            return myBinarySem02Handle;
        case UART_PORT3:
            return myBinarySem03Handle;
        case UART_PORT4:
            return myBinarySem04Handle;
        default:
            return NULL;
    }
}







