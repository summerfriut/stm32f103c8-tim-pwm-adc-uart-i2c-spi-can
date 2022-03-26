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
    NONE_REQUEST,                       //δ��������
    TX_WAIT_RESPONSE,                   //�ѷ��͵ȴ���Ӧ
    RX_RESPONSE,                        //���յ���Ӧ�ȴ�����
    DEAL_RESPONSE                       //������Ӧ
}__uart_stat;

//usart������ID
//��ͬ������ʹ�ò�ͬ��ID���ڽ��յ�������ʱ���Ը�IDΪ����
typedef enum
{
    HEX_NO_ID,                                      //û��ָ�����������ʽ������Ϊ16����
    ASCII_NO_ID                                     //û��ָ�����������ʽ������ΪASCII
}_usart_peri_id;

#define USART_TX_TIMES_BASE 200         //ÿ����USART_TX_TIMES_BASE�μ���һ��ͨѶ����(������)
#define USART_COMM_QUALITY_MIN  50      //���ͨѶ����,������Ϊʧ��

typedef struct
{
    GPIO_TypeDef*   EN485_GPIO_Port;    //ָ������1��Ӧ��485ʹ������Port��
    uint16_t        EN485_Pin;          //ָ������1��Ӧ��485ʹ������Pin��
    GPIO_PinState   EN485_RX;           //ʹ�ܴ���485Ϊ����ʱ�����ŵ�ƽ
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

//ͨ��serial_port���ڷ�������message
typedef void (*_send_message)(uint8_t port);
//�������Խṹ��
typedef struct
{
    UART_HandleTypeDef*     huart;
    DMA_HandleTypeDef*      hdma_usart_rx;
    DMA_HandleTypeDef*      hdma_usart_tx;
    uint8_t*    recv_buf;                           //���ڽ��յ����ݻ���
    uint8_t     recv_buf_size;                      //���ջ������ܴ�С
    uint8_t     recv_buf_len;                       //���յ������ݳ���
    
    uint8_t*    send_buf;                           //ָ�򴮿ڵķ������ݵ�ַ
    uint16_t    send_buf_len;                       //���͵����ݳ���
    
    uint8_t     usart_rx_tx_stat;                   //0: δ�������� 1: �Ѿ����ͳ�ȥ�ȴ���Ӧ 2: �Ѿ����յ���Ӧ 3:����Ӧ�Ѿ�������
    uint8_t     usart_rx_times;                     //�������ݽ��յ�����,��������USART_MIN_RX_TIMES����Ϊû����
    uint8_t     usart_tx_times;                     //���������������
    uint8_t     usart_missflag;                     //����ʧ����־λ 1: ����ʧ��,BMMU�𻵻�ӿ�����
    uint8_t     usart_switch;                       //���ڿ���       1: ��������,��BGMS��������BMMU������������    //�ò���ʹ����Ӧ������flash
    uint8_t     port;                               //���ںţ����ڽ��ṹ����ʵ�ʵĴ��ڶ�Ӧ(�磺usart_info[0] ��Ӧ ���ں�1)
    uint8_t     crc_effctive;                       //У���Ƿ���Ч 1: ��Ч
    float       usart_communication_quality;        //����ͨѶ����(�붪���ʺ�Ϊ1)usart_rx_times/usart_tx_times
    _send_message send_message;                     //���ݷ���
    __ctl_485*  ctl_485;                            //485���ſ�����Ϣ
    uint8_t     tx_ok;                              //�������
    uint8_t     send_buf_size;
}__usart_info;
extern __usart_info usart_info[];                    //��¼ÿһ��BMS��4������״̬�Լ���������
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



