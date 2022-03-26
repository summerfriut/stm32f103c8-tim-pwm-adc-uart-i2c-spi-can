#include "my_queue_event.h"

//信号量
osSemaphoreId_t myBinarySem01Handle;
osSemaphoreId_t myBinarySem02Handle;
osSemaphoreId_t myBinarySem03Handle;
osSemaphoreId_t myBinarySem04Handle;
osSemaphoreId_t myBinarySem05Handle;
osSemaphoreId_t myBinarySem06Handle;
osSemaphoreId_t myBinarySem07Handle;
osSemaphoreId_t myBinarySem08Handle;
//流缓冲区
StreamBufferHandle_t StreamBuffer_uart1;
StreamBufferHandle_t StreamBuffer_uart2;
StreamBufferHandle_t StreamBuffer_uart3;
StreamBufferHandle_t StreamBuffer_uart4;
StreamBufferHandle_t StreamBuffer_data2can;
StreamBufferHandle_t StreamBuffer_data2log;
//消息缓冲区
MessageBufferHandle_t MessageBuffer_uart1;
//用户事件组
EventGroupHandle_t event_groups_user1;
EventGroupHandle_t event_groups_user2;
EventGroupHandle_t event_groups_user3;


//初始化信号量,事件组,消息队列,流缓冲区,消息缓冲区......s
void que_event_sem_init()
{
    //创建信号量
    myBinarySem01Handle = xSemaphoreCreateBinary();
    myBinarySem02Handle = xSemaphoreCreateBinary();
    myBinarySem03Handle = xSemaphoreCreateBinary();
    myBinarySem04Handle = xSemaphoreCreateBinary();
    myBinarySem05Handle = xSemaphoreCreateBinary();
    myBinarySem06Handle = xSemaphoreCreateBinary();
    myBinarySem07Handle = xSemaphoreCreateBinary();
    myBinarySem08Handle = xSemaphoreCreateBinary();

    //创建事件组
    event_groups_flag();
    //创建消息队列
    
    
    //创建流缓冲区
    size_t user_StreamBufferSizeBytes = 20,user_TriggerLevel = 20;
    StreamBuffer_uart1 = xStreamBufferCreate(user_StreamBufferSizeBytes,user_TriggerLevel);
    if(!StreamBuffer_uart1)
        return;         //堆空间不足,无法创建流缓冲区
    StreamBuffer_uart2 = xStreamBufferCreate(user_StreamBufferSizeBytes,user_TriggerLevel);
    if(!StreamBuffer_uart2)
        return;         //堆空间不足,无法创建流缓冲区
    StreamBuffer_uart3 = xStreamBufferCreate(user_StreamBufferSizeBytes,user_TriggerLevel);
    if(!StreamBuffer_uart3)
        return;         //堆空间不足,无法创建流缓冲区
    StreamBuffer_uart4 = xStreamBufferCreate(user_StreamBufferSizeBytes,user_TriggerLevel);
    if(!StreamBuffer_uart4)
        return;         //堆空间不足,无法创建流缓冲区
    
    user_StreamBufferSizeBytes = 140+10;
    user_TriggerLevel = 140;
    StreamBuffer_data2can = xStreamBufferCreate(user_StreamBufferSizeBytes,user_TriggerLevel);
    if(!StreamBuffer_data2can)
        return;         //堆空间不足,无法创建流缓冲区
    
    user_StreamBufferSizeBytes = 50;
    user_TriggerLevel = 24;
    StreamBuffer_data2log = xStreamBufferCreate(user_StreamBufferSizeBytes,user_TriggerLevel);
    if(!StreamBuffer_data2log)
        return;         //堆空间不足,无法创建流缓冲区
    
    //创建消息缓冲区
//    const size_t user_MessageBufferSizeBytes = USART_BMMU_BUFSIZE;
//    user_MessageBuffer = xMessageBufferCreate(user_MessageBufferSizeBytes);
//    if(!user_MessageBuffer)
//        return;         //堆空间不足,无法创建消息缓冲区
}

//创建事件组
void event_groups_flag()
{
    //创建事件组，并返回事件组句柄
    event_groups_user1 = xEventGroupCreate();
    //can通讯使用
    event_groups_user2 = xEventGroupCreate();
    
    event_groups_user3 = xEventGroupCreate();
}  









