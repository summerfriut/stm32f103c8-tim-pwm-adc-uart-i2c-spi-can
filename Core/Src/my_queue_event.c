#include "my_queue_event.h"

//�ź���
osSemaphoreId_t myBinarySem01Handle;
osSemaphoreId_t myBinarySem02Handle;
osSemaphoreId_t myBinarySem03Handle;
osSemaphoreId_t myBinarySem04Handle;
osSemaphoreId_t myBinarySem05Handle;
osSemaphoreId_t myBinarySem06Handle;
osSemaphoreId_t myBinarySem07Handle;
osSemaphoreId_t myBinarySem08Handle;
//��������
StreamBufferHandle_t StreamBuffer_uart1;
StreamBufferHandle_t StreamBuffer_uart2;
StreamBufferHandle_t StreamBuffer_uart3;
StreamBufferHandle_t StreamBuffer_uart4;
StreamBufferHandle_t StreamBuffer_data2can;
StreamBufferHandle_t StreamBuffer_data2log;
//��Ϣ������
MessageBufferHandle_t MessageBuffer_uart1;
//�û��¼���
EventGroupHandle_t event_groups_user1;
EventGroupHandle_t event_groups_user2;
EventGroupHandle_t event_groups_user3;


//��ʼ���ź���,�¼���,��Ϣ����,��������,��Ϣ������......s
void que_event_sem_init()
{
    //�����ź���
    myBinarySem01Handle = xSemaphoreCreateBinary();
    myBinarySem02Handle = xSemaphoreCreateBinary();
    myBinarySem03Handle = xSemaphoreCreateBinary();
    myBinarySem04Handle = xSemaphoreCreateBinary();
    myBinarySem05Handle = xSemaphoreCreateBinary();
    myBinarySem06Handle = xSemaphoreCreateBinary();
    myBinarySem07Handle = xSemaphoreCreateBinary();
    myBinarySem08Handle = xSemaphoreCreateBinary();

    //�����¼���
    event_groups_flag();
    //������Ϣ����
    
    
    //������������
    size_t user_StreamBufferSizeBytes = 20,user_TriggerLevel = 20;
    StreamBuffer_uart1 = xStreamBufferCreate(user_StreamBufferSizeBytes,user_TriggerLevel);
    if(!StreamBuffer_uart1)
        return;         //�ѿռ䲻��,�޷�������������
    StreamBuffer_uart2 = xStreamBufferCreate(user_StreamBufferSizeBytes,user_TriggerLevel);
    if(!StreamBuffer_uart2)
        return;         //�ѿռ䲻��,�޷�������������
    StreamBuffer_uart3 = xStreamBufferCreate(user_StreamBufferSizeBytes,user_TriggerLevel);
    if(!StreamBuffer_uart3)
        return;         //�ѿռ䲻��,�޷�������������
    StreamBuffer_uart4 = xStreamBufferCreate(user_StreamBufferSizeBytes,user_TriggerLevel);
    if(!StreamBuffer_uart4)
        return;         //�ѿռ䲻��,�޷�������������
    
    user_StreamBufferSizeBytes = 140+10;
    user_TriggerLevel = 140;
    StreamBuffer_data2can = xStreamBufferCreate(user_StreamBufferSizeBytes,user_TriggerLevel);
    if(!StreamBuffer_data2can)
        return;         //�ѿռ䲻��,�޷�������������
    
    user_StreamBufferSizeBytes = 50;
    user_TriggerLevel = 24;
    StreamBuffer_data2log = xStreamBufferCreate(user_StreamBufferSizeBytes,user_TriggerLevel);
    if(!StreamBuffer_data2log)
        return;         //�ѿռ䲻��,�޷�������������
    
    //������Ϣ������
//    const size_t user_MessageBufferSizeBytes = USART_BMMU_BUFSIZE;
//    user_MessageBuffer = xMessageBufferCreate(user_MessageBufferSizeBytes);
//    if(!user_MessageBuffer)
//        return;         //�ѿռ䲻��,�޷�������Ϣ������
}

//�����¼���
void event_groups_flag()
{
    //�����¼��飬�������¼�����
    event_groups_user1 = xEventGroupCreate();
    //canͨѶʹ��
    event_groups_user2 = xEventGroupCreate();
    
    event_groups_user3 = xEventGroupCreate();
}  









