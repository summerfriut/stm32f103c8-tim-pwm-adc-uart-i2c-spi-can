#ifndef __MY_I2C_H__
#define __MY_I2C_H__
#include "i2c.h"
#include "my_cfg.h"

#ifdef  USE_I2C1
#define I2C1_BUF_SIZE           (uint16_t)20
extern uint8_t i2c1_buf[];
#endif

#ifdef  USE_I2C2
#define I2C2_BUF_SIZE           (uint16_t)20
extern uint8_t i2c2_buf[];
#endif

#ifdef  USE_I2C3
#define I2C3_BUF_SIZE           (uint16_t)20
extern uint8_t i2c3_buf[];
#endif

enum
{
    I2C_PORT1,
    I2C_PORT2,
    I2C_PORT3
};

typedef void (*__callback)(uint8_t port);
typedef struct
{
    I2C_HandleTypeDef*  hi2c;
    DMA_HandleTypeDef*  hdma_i2c_rx;
    DMA_HandleTypeDef*  hdma_i2c_tx;
    uint8_t*    buf;                                //串口接收的数据缓存
    uint8_t     buf_size;                           //接收缓冲区总大小
    uint8_t     buf_len;                            //接收到的数据长度
    uint8_t     port;                               //串口号，用于将结构体与实际的串口对应
    
    uint8_t     missflag;                           //串口失联标志位 1: 失联
    uint8_t     soft_switch;                        //串口软开关       1: 开启
    float       comm_quality;                       //串口通讯质量(与丢包率和为1)usart_rx_times/usart_tx_times
    __callback  write;                              //写数据
    __callback  read;                               //读数据
    uint8_t     i2c_addr;                           //salve addr
    uint8_t     reg_addr;
    uint8_t     tx_ing;                             //正在进行数据发送
}__i2c_info;
extern __i2c_info   i2c_info[];
extern DMA_HandleTypeDef   hdma_i2c1_rx;
extern DMA_HandleTypeDef   hdma_i2c1_tx;



HAL_StatusTypeDef i2c_master_write_byte(uint8_t port,uint8_t i2c_addr,uint8_t reg_addr,uint8_t data);
HAL_StatusTypeDef i2c_master_write(uint8_t port,uint8_t i2c_addr,uint8_t reg_addr,uint8_t data_len,uint8_t* data);
HAL_StatusTypeDef i2c_master_read(uint8_t port,uint8_t i2c_addr,uint8_t reg_addr,uint16_t read_data_len,uint8_t* response_data);


void my_i2c_init(void);
void write_msg(uint8_t port);
void read_msg(uint8_t port);
void i2c_mst_read(uint8_t port,uint8_t i2c_addr,uint8_t reg_addr,uint8_t* pdata,uint8_t data_len);
void i2c_mst_write_bytes(uint8_t port,uint8_t i2c_addr,uint8_t reg_addr,uint8_t* pdata,uint8_t data_len);
void i2c_mst_write_byte(uint8_t port,uint8_t i2c_addr,uint8_t reg_addr,uint8_t data);

uint8_t i2c_port2handle(uint8_t port, I2C_HandleTypeDef** Handle);
uint8_t i2c_handle2port(I2C_HandleTypeDef* Handle);
uint8_t i2c_port2index(uint8_t port);
#endif










