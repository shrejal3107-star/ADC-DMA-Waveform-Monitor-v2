#ifndef USART_H_
#define USART_H_

#include "stm32f4xx.h"
#include <stdint.h>

typedef enum
{
    USART_MODE_TX = 0,
    USART_MODE_RX,
    USART_MODE_TX_RX
} USART_Mode;

typedef enum
{
    USART_WORDLENGTH_8B = 0,
    USART_WORDLENGTH_9B
} USART_WordLength;

typedef enum
{
    USART_STOPBITS_1 = 0,
    USART_STOPBITS_0_5,
    USART_STOPBITS_2,
    USART_STOPBITS_1_5
} USART_StopBits;

typedef enum
{
    USART_PARITY_NONE = 0,
    USART_PARITY_EVEN,
    USART_PARITY_ODD
} USART_Parity;

typedef struct
{
    uint32_t BaudRate;
    USART_Mode Mode;
    USART_WordLength WordLength;
    USART_StopBits StopBits;
    USART_Parity Parity;
} USART_InitTypeDef;

void USART_Init(USART_TypeDef *USARTx, USART_InitTypeDef *USART_InitStruct);
void USART_Enable(USART_TypeDef *USARTx);
void USART_Disable(USART_TypeDef *USARTx);
void USART_WriteChar(USART_TypeDef *USARTx, char ch);
char USART_ReadChar(USART_TypeDef *USARTx);
void USART_WriteString(USART_TypeDef *USARTx, const char *str);

#endif
