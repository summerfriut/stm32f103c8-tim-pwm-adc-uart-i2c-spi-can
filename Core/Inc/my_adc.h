#ifndef __MY_ADC_H__
#define __MY_ADC_H__
#include "adc.h"
#include "my_cfg.h"




#ifdef USE_ADC1
#define ADC1_CHANNEL    4
extern uint16_t adc1_val[ADC1_CHANNEL];
#endif

#ifdef USE_ADC2
#define ADC2_CHANNEL    4
extern uint16_t adc2_val[ADC2_CHANNEL];
#endif

#ifdef USE_ADC3
#define ADC3_CHANNEL    4
extern uint16_t adc3_val[ADC3_CHANNEL];
#endif

typedef enum
{
    ADC_PORT1,
    ADC_PORT2,
    ADC_PORT3
}__ADC_PORT;

typedef struct
{
    ADC_HandleTypeDef*  hadc;
    DMA_HandleTypeDef*  hdma_adc;
    uint16_t*          val;
    uint8_t            cha_num;
    uint8_t            port;
}__adc_info;
extern __adc_info adc_info[];

extern DMA_HandleTypeDef hdma_adc1;




void        start_adc_dma(uint8_t port);
uint8_t     adc_port2index(uint8_t port);











#endif

