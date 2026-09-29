#include "stm32f4xx.h"
#include "adc.h"
#include "dma.h"
#include "usart.h"
#include <stdint.h>

#define SAMPLE_BUF_LEN  32
volatile uint16_t adc_buffer[SAMPLE_BUF_LEN];

#ifdef RENODE_DEMO
/* Renode's stock DMA model does not fire per-conversion peripheral
 * requests for ADC->memory transfers, so under RENODE_DEMO the buffer
 * is filled by polling the ADC directly instead of via DMA. The real
 * DMA_Init/DMA_Start path below is unchanged for hardware builds. */
volatile uint16_t renode_write_idx = 0;
#endif

static void uart2_gpio_init(void)
{
    RCC->AHB1ENR |= (1U << 0);

    GPIOA->MODER &= ~((3U << (2*2)) | (3U << (3*2)));
    GPIOA->MODER |=  ((2U << (2*2)) | (2U << (3*2)));

    GPIOA->AFR[0] &= ~((0xFU << (4*2)) | (0xFU << (4*3)));
    GPIOA->AFR[0] |=  ((7U   << (4*2)) | (7U   << (4*3)));
}

static void uart2_send_uint(uint16_t val)
{
    char buf[6];
    int i = 0;
    if (val == 0) { USART_WriteChar(USART2, '0'); return; }
    while (val > 0 && i < 5) { buf[i++] = '0' + (val % 10); val /= 10; }
    while (i > 0) USART_WriteChar(USART2, buf[--i]);
}

static void adc_gpio_init(void)
{
    RCC->AHB1ENR |= (1U << 0);
    GPIOA->MODER |= (3U << (0*2));   /* PA0 analog mode */
}

static void tim2_init(uint32_t period_hz)
{
    RCC->APB1ENR |= (1U << 0);
    TIM2->PSC = (16000000UL / 1000) - 1;
    TIM2->ARR = (1000 / period_hz) - 1;
    TIM2->DIER |= (1U << 0);
    TIM2->CR1  |= (1U << 0);
    NVIC_EnableIRQ(TIM2_IRQn);
}

void TIM2_IRQHandler(void)
{
    if (TIM2->SR & (1U << 0)) {
        TIM2->SR &= ~(1U << 0);

        uint16_t latest_index;

#ifdef RENODE_DEMO
        latest_index = (renode_write_idx == 0) ? (SAMPLE_BUF_LEN - 1) : (renode_write_idx - 1);
#else
        uint16_t written = SAMPLE_BUF_LEN - (uint16_t)DMA2_Stream0->NDTR;
        latest_index = (written == 0) ? (SAMPLE_BUF_LEN - 1) : (written - 1);
#endif

        uint16_t sample = adc_buffer[latest_index];

        USART_WriteString(USART2, "SAMPLE:");
        uart2_send_uint(sample);
        USART_WriteString(USART2, "\r\n");
    }
}

int main(void)
{
    uart2_gpio_init();

    USART_InitTypeDef uartCfg;
    uartCfg.BaudRate   = 9600;
    uartCfg.Mode       = USART_MODE_TX_RX;
    uartCfg.WordLength = USART_WORDLENGTH_8B;
    uartCfg.StopBits   = USART_STOPBITS_1;
    uartCfg.Parity     = USART_PARITY_NONE;
    USART_Init(USART2, &uartCfg);

    adc_gpio_init();

    ADC_InitTypeDef adcCfg;
    adcCfg.Resolution = ADC_RESOLUTION_12BIT;
    adcCfg.Channel    = ADC_CHANNEL_0;
    ADC_Init(ADC1, &adcCfg);

    ADC_EnableContinuousMode(ADC1);

#ifndef RENODE_DEMO
    ADC_EnableDMA(ADC1);

    DMA_InitTypeDef dmaCfg;
    dmaCfg.Channel              = 0;
    dmaCfg.Direction            = DMA_PERIPHERAL_TO_MEMORY;
    dmaCfg.PeripheralDataSize   = DMA_DATA_SIZE_16BIT;
    dmaCfg.MemoryDataSize       = DMA_DATA_SIZE_16BIT;
    dmaCfg.PeripheralIncrement  = 0;
    dmaCfg.MemoryIncrement      = 1;
    dmaCfg.CircularMode         = 1;
    dmaCfg.Priority             = DMA_PRIORITY_HIGH;
    dmaCfg.NumberOfTransfers    = SAMPLE_BUF_LEN;
    dmaCfg.PeripheralAddress    = (uint32_t)&ADC1->DR;
    dmaCfg.MemoryAddress        = (uint32_t)adc_buffer;

    DMA_Init(DMA2_Stream0, &dmaCfg);
    DMA_Start(DMA2_Stream0);
#endif

    ADC_Start(ADC1);
    ADC_StartConversion(ADC1);

    tim2_init(10);

    USART_WriteString(USART2, "Waveform Monitor started\r\n");

#ifdef RENODE_DEMO
    while (1) {
        if (ADC_IsConversionComplete(ADC1)) {
            uint16_t sample = ADC_GetValue(ADC1);
            adc_buffer[renode_write_idx] = sample;
            renode_write_idx = (renode_write_idx + 1) % SAMPLE_BUF_LEN;
        }
    }
#else
    while (1) {
        __asm__("wfi");
    }
#endif
}
