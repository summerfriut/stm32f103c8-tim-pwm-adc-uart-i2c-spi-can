#include "my_adc.h"

#ifdef USE_ADC1
uint16_t         adc1_val[ADC1_CHANNEL];
#endif

#ifdef USE_ADC2
uint16_t         adc2_val[ADC2_CHANNEL];
#endif

#ifdef USE_ADC3
uint16_t         adc3_val[ADC3_CHANNEL];
#endif


__adc_info adc_info[] = 
{
    #ifdef USE_ADC1
    {.hadc = &hadc1, .hdma_adc = &hdma_adc1, .cha_num = ADC1_CHANNEL, .val = adc1_val, .port = ADC_PORT1},
    #endif
    #ifdef USE_ADC2
    {.hadc = &hadc2, .hdma_adc = &hdma_adc2, .cha_num = ADC2_CHANNEL, .val = adc2_val, .port = ADC_PORT2},
    #endif
    #ifdef USE_ADC3
    {.hadc = &hadc3, .hdma_adc = &hdma_adc3, .cha_num = ADC3_CHANNEL, .val = adc3_val, .port = ADC_PORT3},
    #endif
};


void start_adc_dma(uint8_t port)
{
    uint8_t index = adc_port2index(port);
    if(IS_INDEX(index) == pdFAIL)
        return;
    HAL_ADC_Start_DMA(adc_info[index].hadc, (uint32_t*)adc_info[index].val, adc_info[index].cha_num);
}

//根据adc port号获取adc_info索引
uint8_t adc_port2index(uint8_t port)
{
    uint8_t index;
    for(index = 0 ; index < sizeof(adc_info) / sizeof(__adc_info) ; index++)
    {
        if(port == adc_info[index].port)
            return index;
    }
    return 0xFF;            //ERR
}



