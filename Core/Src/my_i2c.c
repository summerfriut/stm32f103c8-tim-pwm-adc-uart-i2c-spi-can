#include "my_i2c.h"
#include "string.h"

#ifdef USE_I2C1
uint8_t i2c1_buf[I2C1_BUF_SIZE];
#endif

#ifdef USE_I2C2
uint8_t i2c2_buf[I2C2_BUF_SIZE];
#endif

#ifdef USE_I2C3
uint8_t i2c3_buf[I2C3_BUF_SIZE];
#endif


__i2c_info   i2c_info[] =
{
#ifdef USE_I2C1
    {.hi2c = &hi2c1, .hdma_i2c_rx = &hdma_i2c1_rx, .hdma_i2c_tx = &hdma_i2c1_tx, .port = I2C_PORT1, .buf = i2c1_buf, .buf_size = I2C1_BUF_SIZE, .write = write_msg, .read = read_msg},
#endif
#ifdef USE_I2C2
    {.hi2c = &hi2c2, .hdma_i2c_rx = &hdma_i2c2_rx, .hdma_i2c_tx = &hdma_i2c2_tx, .port = I2C_PORT2, .buf = i2c2_buf, .buf_size = I2C2_BUF_SIZE, .write = write_msg, .read = read_msg},
#endif
#ifdef USE_I2C3
    {.hi2c = &hi2c3, .hdma_i2c_rx = &hdma_i2c3_rx, .hdma_i2c_tx = &hdma_i2c3_tx, .port = I2C_PORT3, .buf = i2c3_buf, .buf_size = I2C3_BUF_SIZE, .write = write_msg, .read = read_msg},
#endif
};

//等待i2c总线空闲
HAL_I2C_StateTypeDef wait_i2c_ready(uint32_t Timeout, I2C_HandleTypeDef *hi2c)
{
    HAL_I2C_StateTypeDef i2c_status;
    uint32_t start_time;
    start_time = HAL_GetTick();
    while(Timeout > (HAL_GetTick() - start_time))
    {
        i2c_status = HAL_I2C_GetState(hi2c);
        if(i2c_status == HAL_I2C_STATE_READY)
            break;
    }
    return i2c_status;
}


//向从机地址为i2c_addr的寄存器reg_addr写入data
HAL_StatusTypeDef i2c_master_write_byte(uint8_t port,uint8_t i2c_addr,uint8_t reg_addr,uint8_t data)
{
    I2C_HandleTypeDef* Handle;
    uint8_t pdata[2];
    pdata[0] = reg_addr;
    pdata[1] = data;
    if(i2c_port2handle(port, &Handle))
        return HAL_ERROR;
    HAL_StatusTypeDef sta = HAL_TIMEOUT;
    if(wait_i2c_ready(1000,Handle) == HAL_I2C_STATE_READY)
        sta = HAL_I2C_Master_Transmit(Handle, i2c_addr, pdata, 2, 1000);
    return sta;
}

HAL_StatusTypeDef i2c_master_write(uint8_t port,uint8_t i2c_addr,uint8_t reg_addr,uint8_t data_len,uint8_t* data)
{
    I2C_HandleTypeDef* Handle;
    uint8_t pdata[data_len+1];
    pdata[0] = reg_addr;
    if(i2c_port2handle(port, &Handle))
        return HAL_ERROR;
    HAL_StatusTypeDef sta = HAL_TIMEOUT;
    memcpy(pdata, data, data_len);
    if(wait_i2c_ready(3000,Handle) == HAL_I2C_STATE_READY)
        sta = HAL_I2C_Master_Transmit(Handle, i2c_addr, pdata, data_len+1, 1000);
    return sta;
}

//通过i2c读取数据
HAL_StatusTypeDef i2c_master_read(uint8_t port,uint8_t i2c_addr,uint8_t reg_addr,uint16_t read_data_len,uint8_t* response_data)
{
    I2C_HandleTypeDef* Handle;
    if(i2c_port2handle(port, &Handle))
        return HAL_ERROR;
    HAL_StatusTypeDef sta = HAL_TIMEOUT;
    if(wait_i2c_ready(3000,Handle) == HAL_I2C_STATE_READY)
    {
        HAL_I2C_Master_Transmit(Handle, i2c_addr, &reg_addr, 1, 1000);
        sta = HAL_I2C_Master_Receive(Handle, i2c_addr, response_data, read_data_len, 1000);
    }
    return sta;
}

uint8_t i2c_handle2port(I2C_HandleTypeDef* Handle)
{
    uint8_t index;
    for(index = 0 ; index < sizeof(i2c_info) / sizeof(__i2c_info) ; index++)
    {
        if(i2c_info[index].hi2c == Handle)
            return i2c_info[index].port;
    }
    return 0xFF;            //ERR
}

//根据串口号获取句柄                                        
uint8_t i2c_port2handle(uint8_t port, I2C_HandleTypeDef** Handle)
{
    uint8_t index;
    for(index = 0 ; index < sizeof(i2c_info) / sizeof(__i2c_info) ; index++)
    {
        if(i2c_info[index].port == port)
        {
            *Handle = i2c_info[index].hi2c;
            return index;
        }
    }
    return 0xFF;            //ERR
}

//根据i2c port号获取i2c_info索引
uint8_t i2c_port2index(uint8_t port)
{
    uint8_t index;
    for(index = 0 ; index < sizeof(i2c_info) / sizeof(__i2c_info) ; index++)
    {
        if(port == i2c_info[index].port)
            return index;
    }
    return 0xFF;            //ERR
}

uint8_t i2c_handle2index(I2C_HandleTypeDef* Handle)
{
    uint8_t index;
    for(index = 0 ; index < sizeof(i2c_info) / sizeof(__i2c_info) ; index++)
    {
        if(Handle == i2c_info[index].hi2c)
            return index;
    }
    return 0xFF;            //ERR
}

void i2c_mst_write_bytes(uint8_t port,uint8_t i2c_addr,uint8_t reg_addr,uint8_t* pdata,uint8_t data_len)
{
    uint8_t index = i2c_port2index(port);
    if(IS_INDEX(index) == pdFAIL)
        return;
    i2c_info[index].buf_len  = data_len + 1;
    i2c_info[index].buf[0]   = reg_addr;
    i2c_info[index].i2c_addr = i2c_addr;
    i2c_info[index].tx_ing   = 1;
    memcpy(i2c_info[index].buf + 1,pdata,data_len);
    i2c_info[index].write(port);
}

void i2c_mst_write_byte(uint8_t port,uint8_t i2c_addr,uint8_t reg_addr,uint8_t data)
{
    uint8_t index = i2c_port2index(port);
    if(IS_INDEX(index) == pdFAIL)
        return;
    i2c_info[index].buf_len  = 1 + 1;
    i2c_info[index].buf[0]   = reg_addr;
    i2c_info[index].buf[1]   = data;
    i2c_info[index].i2c_addr = i2c_addr;
    i2c_info[index].tx_ing    = 1;
    i2c_info[index].write(port);
}

void i2c_mst_read(uint8_t port,uint8_t i2c_addr,uint8_t reg_addr,uint8_t* pdata,uint8_t data_len)
{
    uint8_t index = i2c_port2index(port);
    if(IS_INDEX(index) == pdFAIL)
        return;
    i2c_info[index].buf_len = data_len;
    i2c_info[index].reg_addr= reg_addr;
    i2c_info[index].i2c_addr= i2c_addr;
    if(pdata)
        i2c_info[index].buf = pdata;
    else
    {
        #ifdef USE_I2C1
            i2c_info[index].buf = i2c1_buf;
        #endif
        #ifdef USE_I2C2
            i2c_info[index].buf = i2c2_buf;
        #endif
        #ifdef USE_I2C3
            i2c_info[index].buf = i2c3_buf;
        #endif
    }
    i2c_info[index].read(port);
}

//串口数据发送函数
//将数据message通过serial_port串口发送message_len个字节
void write_msg(uint8_t port)
{
    I2C_HandleTypeDef* Handle;
    //如果数据过长可通过动态内存分配实现数据的缓存,但发送完成应当及时释放堆区,或者采用标志位保证不再重复分配堆区
    if(i2c_info[port].buf_len > i2c_info[port].buf_size)
        i2c_info[port].buf_len = i2c_info[port].buf_size;
    //获取I2C句柄
    if(i2c_port2handle(port, &Handle))
        return;
    wait_i2c_ready(500,Handle);
    if(Handle->hdmatx)
        HAL_I2C_Master_Transmit_DMA(Handle, i2c_info[port].i2c_addr, i2c_info[port].buf, i2c_info[port].buf_len);
    else
        HAL_I2C_Master_Transmit_IT(Handle, i2c_info[port].i2c_addr, i2c_info[port].buf, i2c_info[port].buf_len);
}

void read_msg(uint8_t port)
{
    I2C_HandleTypeDef* Handle;
    //如果数据过长可通过动态内存分配实现数据的缓存,但发送完成应当及时释放堆区,或者采用标志位保证不再重复分配堆区
    if(i2c_info[port].buf_len > i2c_info[port].buf_size)
        i2c_info[port].buf_len = i2c_info[port].buf_size;
    //获取I2C句柄
    if(i2c_port2handle(port, &Handle))
        return;
    wait_i2c_ready(500,Handle);
    HAL_I2C_Master_Transmit(Handle, i2c_info[port].i2c_addr, &i2c_info[port].reg_addr, 1, 1000);
    if(Handle->hdmarx)
        HAL_I2C_Master_Receive_DMA(Handle, i2c_info[port].i2c_addr, i2c_info[port].buf, i2c_info[port].buf_len);
    else
        HAL_I2C_Master_Receive_IT(Handle, i2c_info[port].i2c_addr, i2c_info[port].buf, i2c_info[port].buf_len);
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    uint8_t index = i2c_handle2index(hi2c);
    if(IS_INDEX(index) == pdFAIL)
        return;
    i2c_info[index].tx_ing = 0;
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    //将发送的寄存器地址、数据长度、数据内容压入信号量
    
}

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
}
