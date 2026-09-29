#include "adc.h"

void ADC_Init(ADC_TypeDef *ADCx, ADC_InitTypeDef *ADC_InitStruct)
{
    if (ADCx == ADC1)
    {
        RCC->APB2ENR |= (1U << 8);
    }

    ADC->CCR &= ~(3U << 16);

    ADCx->CR2 &= ~(1U << 0);

    ADCx->CR1 &= ~(3U << 24);

    if (ADC_InitStruct->Resolution == ADC_RESOLUTION_12BIT)
    {
        ADCx->CR1 |= (0U << 24);
    }
    else if (ADC_InitStruct->Resolution == ADC_RESOLUTION_10BIT)
    {
        ADCx->CR1 |= (1U << 24);
    }
    else if (ADC_InitStruct->Resolution == ADC_RESOLUTION_8BIT)
    {
        ADCx->CR1 |= (2U << 24);
    }
    else if (ADC_InitStruct->Resolution == ADC_RESOLUTION_6BIT)
    {
        ADCx->CR1 |= (3U << 24);
    }

    ADCx->SQR3 &= ~(0x1FU << 0);
    ADCx->SQR3 |= ((uint32_t)ADC_InitStruct->Channel << 0);

    ADCx->SQR1 &= ~(0xFU << 20);

    if (ADC_InitStruct->Channel <= ADC_CHANNEL_9)
    {
        uint32_t position = ADC_InitStruct->Channel * 3;
        ADCx->SMPR2 &= ~(0x7U << position);
    }
    else if (ADC_InitStruct->Channel <= ADC_CHANNEL_18)
    {
        uint32_t position = (ADC_InitStruct->Channel - 10) * 3;
        ADCx->SMPR1 &= ~(0x7U << position);
    }
}

void ADC_Start(ADC_TypeDef *ADCx)
{
    ADCx->CR2 |= (1U << 0);
}

void ADC_Stop(ADC_TypeDef *ADCx)
{
    ADCx->CR2 &= ~(1U << 0);
}

void ADC_StartConversion(ADC_TypeDef *ADCx)
{
    ADCx->CR2 |= (1U << 30);
}

uint8_t ADC_IsConversionComplete(ADC_TypeDef *ADCx)
{
    if (ADCx->SR & (1U << 1))
    {
        return 1;
    }
    return 0;
}

uint16_t ADC_GetValue(ADC_TypeDef *ADCx)
{
    return ADCx->DR;
}

void ADC_EnableContinuousMode(ADC_TypeDef *ADCx)
{
    ADCx->CR2 |= (1U << 1);
}

void ADC_DisableContinuousMode(ADC_TypeDef *ADCx)
{
    ADCx->CR2 &= ~(1U << 1);
}

void ADC_EnableDMA(ADC_TypeDef *ADCx)
{
    ADCx->CR2 |= (1U << 8);
    ADCx->CR2 |= (1U << 9);
}

void ADC_DisableDMA(ADC_TypeDef *ADCx)
{
    ADCx->CR2 &= ~(1U << 8);
    ADCx->CR2 &= ~(1U << 9);
}
