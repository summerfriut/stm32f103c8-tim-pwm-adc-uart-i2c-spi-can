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
#define USE_UART1                           //�������ڵĲ������ó�ʼ��
#define USE_UART3                           //�������ڵĲ������ó�ʼ��    

#define UART_PER_NUMS       6               //ʹ�ô��ڵ���������


/*---------------------------------------------------------------UART 485 config--------------------------------------------------------------------*/
//#define UART1_EN485                         //���ڿ���485����ʹ��
//#define UART2_EN485                         //���ڿ���485����ʹ��
//#define UART3_EN485                         //���ڿ���485����ʹ��
//#define UART4_EN485                         //���ڿ���485����ʹ��


/*---------------------------------------------------------------UART peripherals config--------------------------------------------------------------------*/
#ifdef  PV_ENERGY
#define PV_ENERGY_UART      UART_PORT1      //ʹ��MODBUSЭ��
#endif

#ifdef  WIND_ENERGY
#define WIND_ENERGY_UART    UART_PORT1      //�������Э��
#endif

#ifdef  MPPT
#define MPPT_UART           UART_PORT1      //MPPTЭ��
#endif

#define SWITCH_POWER_UART   UART_PORT4      //���ص�Դ
#define METER_UART          UART_PORT2      //����ʹ��modbus(��Ҫż����)
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
    uint8_t     connect_uart_port;          //ĳ�����Ӧ�Ĵ��ں�
    data_anasy  anasy_fun;                  //�������Ӧ�Ĵ�����
}_uart_fun_handle;                          //����ÿ�����������ӵĴ���,�Լ�������
extern _uart_fun_handle uart_fun_handle[UART_PER_NUMS];

void        call_uart_function(uint8_t uart_port);
uint16_t    get_stream_buff(StreamBufferHandle_t* StreamBuffer,uint8_t* recv_buf);
uint16_t    swap(uint16_t data);
uint32_t    uint32_byte_sequence(uint32_t* pdata,uint8_t byte_sequence);
void        mem_byte_sequence_reverse(void* p_source_data,void* p_target_data,uint8_t data_len,uint8_t source_type,uint8_t result_type);
void        my_memcpy_byte2mem(uint8_t* p_source_data,void* p_target_data,uint8_t data_len,uint8_t target_type);


#define IS_INDEX(INDEX)   ((INDEX) != 0xFF)


#endif


