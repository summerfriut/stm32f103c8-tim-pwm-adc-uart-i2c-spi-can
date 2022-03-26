#ifndef __MY_PRINTF_H__
#define __MY_PRINTF_H__
#include "my_usart.h"
#include "my_cfg.h"

#define END_MARK    "\r\n"

typedef struct
{
    UART_HandleTypeDef*     huart;
    uint8_t                 port;
}__printf_info;
extern __printf_info printf_info;

uint8_t* query_end_mark(uint8_t* data);


#endif
