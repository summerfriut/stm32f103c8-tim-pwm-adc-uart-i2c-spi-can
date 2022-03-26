#ifndef __MY_USART_H__
#define __MY_USART_H__
#include "my_cfg.h"

#ifdef  USE_UART1
#define UART1_RX_BUF_SIZE       (uint16_t)100
extern uint8_t uart1_rx_buf[];
#define UART1_TX_BUF_SIZE       (uint16_t)50
extern uint8_t uart1_tx_buf[];
#endif
#ifdef  USE_UART2
#define UART2_RX_BUF_SIZE       (uint16_t)50
extern uint8_t uart2_rx_buf[];
#define UART2_TX_BUF_SIZE       (uint16_t)30
extern uint8_t uart2_tx_buf[];
#endif
#ifdef  USE_UART3
#define UART3_RX_BUF_SIZE       (uint16_t)50
extern uint8_t uart3_rx_buf[];
#define UART3_TX_BUF_SIZE       (uint16_t)20
extern uint8_t uart3_tx_buf[];
#endif
#ifdef  USE_UART4
#define UART4_RX_BUF_SIZE       (uint16_t)50
extern uint8_t uart4_rx_buf[];
#define UART4_TX_BUF_SIZE       (uint16_t)20
extern uint8_t uart4_tx_buf[];
#endif
#ifdef  USE_UART5
#define UART5_RX_BUF_SIZE       (uint16_t)50
extern uint8_t uart5_rx_buf[];
#define UART5_TX_BUF_SIZE       (uint16_t)20
extern uint8_t uart5_tx_buf[];
#endif

typedef enum 
{
    UART_PORT1 = 0,
    UART_PORT2,
    UART_PORT3,
    UART_PORT4,
    UART_PORT5
}uart_port;

#define huart(a)                huart ## a
#define uart_rx_buf(a)          uart ## a ## _rx_buf
#define uart_tx_buf(a)          uart ## a ## _tx_buf
#define UART_RX_BUF_SIZE(a)     UART ## a ## _RX_BUF_SIZE

typedef enum
{
    NONE_REQUEST,                       //未发生请求
    TX_WAIT_RESPONSE,                   //已发送等待响应
    RX_RESPONSE,                        //接收到响应等待处理
    DEAL_RESPONSE                       //处理响应
}__uart_stat;

//usart的外设ID
//不同的外设使用不同的ID，在接收到新数据时，以该ID为依据
typedef enum
{
    HEX_NO_ID,                                      //没有指定外设解析方式，数据为16进制
    ASCII_NO_ID                                     //没有指定外设解析方式，数据为ASCII
}_usart_peri_id;

#define USART_TX_TIMES_BASE 200         //每发送USART_TX_TIMES_BASE次计算一次通讯质量(丢包率)
#define USART_COMM_QUALITY_MIN  50      //最低通讯质量,否则认为失联

typedef struct
{
    GPIO_TypeDef*   EN485_GPIO_Port;    //指定串口1对应的485使能引脚Port号
    uint16_t        EN485_Pin;          //指定串口1对应的485使能引脚Pin号
    GPIO_PinState   EN485_RX;           //使能串口485为发送时的引脚电平
}__ctl_485;
#ifdef UART1_EN485
    extern __ctl_485    ctl1_485;
#endif
#ifdef UART2_EN485
    extern __ctl_485    ctl2_485;
#endif
#ifdef UART3_EN485
    extern __ctl_485    ctl3_485;
#endif
#ifdef UART4_EN485
    extern __ctl_485    ctl4_485;
#endif
#ifdef UART5_EN485
    extern __ctl_485    ctl5_485;
#endif

//通过serial_port串口发送数据message
typedef void (*_send_message)(uint8_t port);
//串口属性结构体
typedef struct
{
    UART_HandleTypeDef*     huart;
    DMA_HandleTypeDef*      hdma_usart_rx;
    DMA_HandleTypeDef*      hdma_usart_tx;
    uint8_t*    recv_buf;                           //串口接收的数据缓存
    uint8_t     recv_buf_size;                      //接收缓冲区总大小
    uint8_t     recv_buf_len;                       //接收到的数据长度
    
    uint8_t*    send_buf;                           //指向串口的发送数据地址
    uint16_t    send_buf_len;                       //发送的数据长度
    
    uint8_t     usart_rx_tx_stat;                   //0: 未发送请求 1: 已经发送出去等待响应 2: 已经接收到响应 3:对响应已经做处理
    uint8_t     usart_rx_times;                     //串口数据接收到次数,连续接收USART_MIN_RX_TIMES次认为没掉线
    uint8_t     usart_tx_times;                     //串口数据请求次数
    uint8_t     usart_missflag;                     //串口失联标志位 1: 串口失联,BMMU损坏或接口脱落
    uint8_t     usart_switch;                       //串口开关       1: 开启串口,即BGMS会主动与BMMU进行数据请求    //该参数使用中应当存入flash
    uint8_t     port;                               //串口号，用于将结构体与实际的串口对应(如：usart_info[0] 对应 串口号1)
    uint8_t     crc_effctive;                       //校验是否有效 1: 有效
    float       usart_communication_quality;        //串口通讯质量(与丢包率和为1)usart_rx_times/usart_tx_times
    _send_message send_message;                     //数据发送
    __ctl_485*  ctl_485;                            //485引脚控制信息
    uint8_t     tx_ok;                              //发送完成
    uint8_t     send_buf_size;
}__usart_info;
extern __usart_info usart_info[];                    //记录每一个BMS的4个串口状态以及串口数据
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;

uint8_t     my_uart_para_cfg(uint8_t port);
void        my_uart_init(void);
void        usart_send(uint8_t port);
void        uart_send_msg(uint8_t port,uint8_t* pdata,uint8_t msg_len);
void        my_usart_rx_IRQ(UART_HandleTypeDef* huart,StreamBufferHandle_t stream_buf,osSemaphoreId_t myBinarySem);
void        uart_change_bradrate(uint8_t port,uint32_t bradrate,uint8_t word_len,uint8_t stop_bit,uint8_t parity);
void        uart_msg_check(__usart_info* usart_info_point);
void        calculate_communication_quality(uint8_t port);
uint8_t     uart_port2handle(uint8_t port, UART_HandleTypeDef** huart_p);
uint8_t     uart_handle2port(UART_HandleTypeDef* huart);
uint8_t     uart_port2index(uint8_t port);
uint8_t     uart_handle2index(UART_HandleTypeDef* Handle);


osSemaphoreId_t uart2sem_handle(uint8_t port);

#endif



