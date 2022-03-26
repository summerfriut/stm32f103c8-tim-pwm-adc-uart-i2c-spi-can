#ifndef __USER_CFG_H__
#define __USER_CFG_H__


#define USE_ADC
#define USE_USART

#define USE_SEM_QUEUE



/*---------------------------------------------------------------include header config start---------------------------------------------------------*/
#ifdef USE_SEM_QUEUE
#include "my_queue_event.h"
#endif

#ifdef USE_USART
#include "usart.h"
#endif

#ifdef USE_ADC
#include "adc.h"
#endif

/*---------------------------------------------------------------include header config end---------------------------------------------------------*/


#define USE_ADC1


/*---------------------------------------------------------------UART config--------------------------------------------------------------------*/
#define USE_UART1                           //开启串口的参数配置初始化
#define USE_UART3                           //开启串口的参数配置初始化    

#define UART_PER_NUMS       6               //使用串口的外设数量


/*---------------------------------------------------------------UART 485 config--------------------------------------------------------------------*/
//#define UART1_EN485                         //用于开启485引脚使能
//#define UART2_EN485                         //用于开启485引脚使能
//#define UART3_EN485                         //用于开启485引脚使能
//#define UART4_EN485                         //用于开启485引脚使能


/*---------------------------------------------------------------UART peripherals config--------------------------------------------------------------------*/
#ifdef  PV_ENERGY
#define PV_ENERGY_UART      UART_PORT1      //使用MODBUS协议
#endif

#ifdef  WIND_ENERGY
#define WIND_ENERGY_UART    UART_PORT1      //风机厂家协议
#endif

#ifdef  MPPT
#define MPPT_UART           UART_PORT1      //MPPT协议
#endif

#define SWITCH_POWER_UART   UART_PORT4      //开关电源
#define METER_UART          UART_PORT2      //部分使用modbus(需要偶检验)
#define CLOUD_UART          UART_PORT3      //GPRS


/*---------------------------------------------------------------I2C config--------------------------------------------------------------------*/
#define USE_I2C1




#define USE_CAN1







#define set_bit(byte,bit)        ((byte) |= (1 << (bit)))
#define reset_bit(byte,bit)      ((byte) &= ~(1 << (bit)))
#define clr_bit(byte,bit)        ((byte) &= ~(1 << (bit)))
#define get_bit(byte,bit)        ((byte) & (1 << (bit)))


typedef void(*data_anasy)(uint8_t uart_port,uint8_t* pdata,uint16_t datalen);
typedef struct
{
    uint8_t     connect_uart_port;          //某外设对应的串口号
    data_anasy  anasy_fun;                  //该外设对应的处理函数
}_uart_fun_handle;                          //配置每个外设所连接的串口,以及处理方法
extern _uart_fun_handle uart_fun_handle[UART_PER_NUMS];

void        call_uart_function(uint8_t uart_port);
uint16_t    get_stream_buff(StreamBufferHandle_t* StreamBuffer,uint8_t* recv_buf);
uint16_t    swap(uint16_t data);
uint32_t    uint32_byte_sequence(uint32_t* pdata,uint8_t byte_sequence);
void        mem_byte_sequence_reverse(void* p_source_data,void* p_target_data,uint8_t data_len,uint8_t source_type,uint8_t result_type);
void        my_memcpy_byte2mem(uint8_t* p_source_data,void* p_target_data,uint8_t data_len,uint8_t target_type);


#define IS_INDEX(INDEX)   ((INDEX) != 0xFF)


#endif


