#ifndef __MY_TASK_H__
#define __MY_TASK_H__
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "my_iwdg.h"
#include "my_usart.h"
#include "my_adc.h"
#include "my_i2c.h"
#include "my_can.h"
#include "my_printf.h"
#include <stdio.h>
#include "my_tty.h"



extern uint32_t Unix_timestamp;


void        my_task_usart(void);
void        my_task_mpu9250(void);
unsigned long   get_tick_ms(unsigned long* timestamp);
void            custom_peripherals_init(void);







#endif


