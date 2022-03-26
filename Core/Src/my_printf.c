#include "my_printf.h"
#include "stdio.h"
#include "string.h"

__printf_info printf_info = 
{
    .huart = &huart1, .port = UART_PORT1,
};

//#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{
    while(__HAL_UART_GET_FLAG(printf_info.huart, UART_FLAG_TXE) != SET){}
//    uart_send_msg(printf_info.port,(uint8_t*)&ch,1);
    USART1->DR = (uint8_t)ch;
	return ch;
}

//����data�е�һ�γ���END_MARK��λ��
uint8_t* query_end_mark(uint8_t* data)
{
    return (uint8_t*)strstr((char*)data, END_MARK);
}


