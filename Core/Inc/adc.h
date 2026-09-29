#ifndef ADC_H_
#define ADC_H_

#include "stm32f4xx.h"
#include <stdint.h>

typedef enum
{
    ADC_RESOLUTION_12BIT,
    ADC_RESOLUTION_10BIT,
    ADC_RESOLUTION_8BIT,
    ADC_RESOLUTION_6BIT
} ADC_Resolution;

typedef enum
{
    ADC_CHANNEL_0,
    ADC_CHANNEL_1,
    ADC_CHANNEL_2,
    ADC_CHANNEL_3,
    ADC_CHANNEL_4,
    ADC_CHANNEL_5,
    ADC_CHANNEL_6,
    ADC_CHANNEL_7,
    ADC_CHANNEL_8,
    ADC_CHANNEL_9,
    ADC_CHANNEL_10,
    ADC_CHANNEL_11,
    ADC_CHANNEL_12,
    ADC_CHANNEL_13,
    ADC_CHANNEL_14,
    ADC_CHANNEL_15,
    ADC_CHANNEL_16,
    ADC_CHANNEL_17,
    ADC_CHANNEL_18
} ADC_Channel;

typedef struct
{
    ADC_Resolution Resolution;
    ADC_Channel Channel;
} ADC_InitTypeDef;

void ADC_Init(ADC_TypeDef *ADCx, ADC_InitTypeDef *ADC_InitStruct);
void ADC_Start(ADC_TypeDef *ADCx);
void ADC_Stop(ADC_TypeDef *ADCx);
void ADC_StartConversion(ADC_TypeDef *ADCx);
uint8_t ADC_IsConversionComplete(ADC_TypeDef *ADCx);
uint16_t ADC_GetValue(ADC_TypeDef *ADCx);
void ADC_EnableContinuousMode(ADC_TypeDef *ADCx);
void ADC_DisableContinuousMode(ADC_TypeDef *ADCx);
void ADC_EnableDMA(ADC_TypeDef *ADCx);
void ADC_DisableDMA(ADC_TypeDef *ADCx);

#endif
