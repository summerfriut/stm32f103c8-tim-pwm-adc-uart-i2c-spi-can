#include "my_task.h"


uint32_t Unix_timestamp;


//void StartDefaultTask(void *argument)
//{
//  /* USER CODE BEGIN StartDefaultTask */
//  /* Infinite loop */
//    portTickType xLastWakeTime;
//    xLastWakeTime = xTaskGetTickCount();
//    osTimerStart(myTimer01Handle,1000);     //开启1s定时器
//  for(;;)
//  {
//      IWDG_Feed();
//      vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(800));     //每800ms喂狗一次
//  }
//  /* USER CODE END StartDefaultTask */
//}

void my_task_usart()
{
    printf("hello this is a printf");
//    uint8_t test[] = "hello this is a test";
//    uart_send_msg(UART_PORT1, test, sizeof(test));
//    uart_send_msg(UART_PORT2, test, sizeof(test));
}


void my_task_mpu9250()
{
}


unsigned long get_tick_ms(unsigned long* timestamp)
{
    *timestamp = HAL_GetTick();
    return *timestamp;
}

void custom_peripherals_init()
{
    que_event_sem_init();
    can_filter_cfg(CAN_PORT1);
    my_uart_init();
}

