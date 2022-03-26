#ifndef __MY_QUEUE_EVENT_H__
#define __MY_QUEUE_EVENT_H__
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "semphr.h"
#include "event_groups.h"
#include "stdint.h"
#include "queue.h"
#include "stream_buffer.h"
#include "message_buffer.h"

//osSemaphoreId_t
//extern QueueHandle_t myBinarySem01Handle;
//extern QueueHandle_t myBinarySem02Handle;
extern osSemaphoreId_t myBinarySem01Handle;
extern osSemaphoreId_t myBinarySem02Handle;
extern osSemaphoreId_t myBinarySem03Handle;
extern osSemaphoreId_t myBinarySem04Handle;
extern osSemaphoreId_t myBinarySem05Handle;
extern osSemaphoreId_t myBinarySem06Handle;
extern osSemaphoreId_t myBinarySem07Handle;
extern osSemaphoreId_t myBinarySem08Handle;
//Á÷»º³åÇø
extern StreamBufferHandle_t StreamBuffer_uart1;
extern StreamBufferHandle_t StreamBuffer_uart2;
extern StreamBufferHandle_t StreamBuffer_uart3;
extern StreamBufferHandle_t StreamBuffer_uart4;
extern StreamBufferHandle_t StreamBuffer_data2can;
extern StreamBufferHandle_t StreamBuffer_data2log;

typedef struct
{
    EventGroupHandle_t  event_group;
    uint32_t            event_bits;
}__event_info;

#define SET_EVENT_BIT(a)    (1 << a)
#define CLR_EVENT_BIT(a)    (~(1 << a))

#define MPU9250_INT         SET_EVENT_BIT(0)
extern EventGroupHandle_t event_groups_user1;


extern EventGroupHandle_t event_groups_user2;


extern EventGroupHandle_t event_groups_user3;


void event_groups_flag(void);

void que_event_sem_init(void);




#endif
























