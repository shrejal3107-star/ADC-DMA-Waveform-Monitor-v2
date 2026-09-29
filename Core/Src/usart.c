#include "usart.h"

void USART_Init(USART_TypeDef *USARTx, USART_InitTypeDef *USART_InitStruct)
{
    if (USARTx == USART1)
    {
        RCC->APB2ENR |= (1U << 4);
    }
    else if (USARTx == USART2)
    {
        RCC->APB1ENR |= (1U << 17);
    }
    else if (USARTx == USART3)
    {
        RCC->APB1ENR |= (1U << 18);
    }

    USART_Disable(USARTx);

    USARTx->CR1 &= ~(1U << 12);
    if (USART_InitStruct->WordLength == USART_WORDLENGTH_9B)
    {
        USARTx->CR1 |= (1U << 12);
    }

    USARTx->CR1 &= ~((1U << 3) | (1U << 2));
    if (USART_InitStruct->Mode == USART_MODE_TX)
    {
        USARTx->CR1 |= (1U << 3);
    }
    else if (USART_InitStruct->Mode == USART_MODE_RX)
    {
        USARTx->CR1 |= (1U << 2);
    }
    else
    {
        USARTx->CR1 |= (1U << 3) | (1U << 2);
    }

    USARTx->CR1 &= ~((1U << 10) | (1U << 9));
    if (USART_InitStruct->Parity == USART_PARITY_EVEN)
    {
        USARTx->CR1 |= (1U << 10);
    }
    else if (USART_InitStruct->Parity == USART_PARITY_ODD)
    {
        USARTx->CR1 |= (1U << 10);
        USARTx->CR1 |= (1U << 9);
    }

    USARTx->CR2 &= ~(3U << 12);
    USARTx->CR2 |= (USART_InitStruct->StopBits << 12);

    USARTx->BRR = 16000000U / USART_InitStruct->BaudRate;

    USART_Enable(USARTx);
}

void USART_Enable(USART_TypeDef *USARTx)
{
    USARTx->CR1 |= (1U << 13);
}

void USART_Disable(USART_TypeDef *USARTx)
{
    USARTx->CR1 &= ~(1U << 13);
}

void USART_WriteChar(USART_TypeDef *USARTx, char ch)
{
    while (!(USARTx->SR & (1U << 7)));
    USARTx->DR = ch;
}

void USART_WriteString(USART_TypeDef *USARTx, const char *str)
{
    while (*str != '\0')
    {
        USART_WriteChar(USARTx, *str);
        str++;
    }
}

char USART_ReadChar(USART_TypeDef *USARTx)
{
    while (!(USARTx->SR & (1U << 5)));
    return USARTx->DR;
}
