#include "dma.h"

void DMA_Init(DMA_Stream_TypeDef *DMA_Stream, DMA_InitTypeDef *DMA_InitStruct)
{
    if (DMA_Stream == 0 || !DMA_ValidateConfig(DMA_InitStruct))
    {
        return;
    }

    RCC->AHB1ENR |= (1U << 22);

    DMA_Stream->CR &= ~(1U << 0);
    while (DMA_Stream->CR & (1U << 0)) { }

    DMA_Stream->CR &= ~(7U << 25);
    DMA_Stream->CR |= ((DMA_InitStruct->Channel & 0x7U) << 25);

    DMA_Stream->CR &= ~(3U << 6);
    DMA_Stream->CR |= ((uint32_t)DMA_InitStruct->Direction << 6);

    DMA_Stream->CR &= ~(1U << 9);
    if (DMA_InitStruct->PeripheralIncrement)
    {
        DMA_Stream->CR |= (1U << 9);
    }

    DMA_Stream->CR &= ~(1U << 10);
    if (DMA_InitStruct->MemoryIncrement)
    {
        DMA_Stream->CR |= (1U << 10);
    }

    DMA_Stream->CR &= ~(3U << 11);
    DMA_Stream->CR |= ((uint32_t)DMA_InitStruct->PeripheralDataSize << 11);

    DMA_Stream->CR &= ~(3U << 13);
    DMA_Stream->CR |= ((uint32_t)DMA_InitStruct->MemoryDataSize << 13);

    DMA_Stream->CR &= ~(1U << 8);
    if (DMA_InitStruct->CircularMode)
    {
        DMA_Stream->CR |= (1U << 8);
    }

    DMA_Stream->CR &= ~(3U << 16);
    DMA_Stream->CR |= ((uint32_t)DMA_InitStruct->Priority << 16);

    DMA_Stream->PAR = DMA_InitStruct->PeripheralAddress;
    DMA_Stream->M0AR = DMA_InitStruct->MemoryAddress;
    DMA_Stream->NDTR = DMA_InitStruct->NumberOfTransfers;
}

void DMA_Start(DMA_Stream_TypeDef *DMA_Stream)
{
    DMA_Stream->CR |= (1U << 0);
}

uint8_t DMA_ValidateConfig(DMA_InitTypeDef *DMA_InitStruct)
{
    if (DMA_InitStruct == 0) return 0;
    if (DMA_InitStruct->Channel > 7) return 0;
    if (DMA_InitStruct->Direction > DMA_MEMORY_TO_MEMORY) return 0;
    if (DMA_InitStruct->PeripheralDataSize > DMA_DATA_SIZE_32BIT) return 0;
    if (DMA_InitStruct->MemoryDataSize > DMA_DATA_SIZE_32BIT) return 0;
    if (DMA_InitStruct->PeripheralIncrement > 1) return 0;
    if (DMA_InitStruct->MemoryIncrement > 1) return 0;
    if (DMA_InitStruct->CircularMode > 1) return 0;
    if (DMA_InitStruct->Priority > DMA_PRIORITY_VERY_HIGH) return 0;
    if (DMA_InitStruct->NumberOfTransfers == 0) return 0;
    if (DMA_InitStruct->NumberOfTransfers > 0xFFFFU) return 0;
    return 1;
}

void DMA_Stop(DMA_Stream_TypeDef *DMA_Stream)
{
    DMA_Stream->CR &= ~(1U << 0);
    while (DMA_Stream->CR & (1U << 0)) { }
}

uint8_t DMA_IsTransferComplete(DMA_Stream_TypeDef *DMA_Stream)
{
    return (DMA2->LISR & (1U << 5)) ? 1 : 0;
}

void DMA_ClearTransferComplete(DMA_Stream_TypeDef *DMA_Stream)
{
    DMA2->LIFCR = (1U << 5);
}

void DMA_EnableTransferCompleteInterrupt(DMA_Stream_TypeDef *DMA_Stream)
{
    DMA_Stream->CR |= (1U << 4);
}

void DMA_EnableHalfTransferInterrupt(DMA_Stream_TypeDef *DMA_Stream)
{
    DMA_Stream->CR |= (1U << 3);
}

void DMA_EnableIRQ(void)
{
    NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

void DMA_ClearFlags(DMA_Stream_TypeDef *DMA_Stream)
{
    DMA2->LIFCR = (1U << 0) | (1U << 2) | (1U << 3) | (1U << 4) | (1U << 5);
}

uint8_t DMA_IsEnabled(DMA_Stream_TypeDef *DMA_Stream)
{
    return (DMA_Stream->CR & (1U << 0)) ? 1 : 0;
}

void DMA_DisableTransferCompleteInterrupt(DMA_Stream_TypeDef *DMA_Stream)
{
    DMA_Stream->CR &= ~(1U << 4);
}

void DMA_DisableHalfTransferInterrupt(DMA_Stream_TypeDef *DMA_Stream)
{
    DMA_Stream->CR &= ~(1U << 3);
}

uint8_t DMA_IsTransferError(DMA_Stream_TypeDef *DMA_Stream)
{
    return (DMA2->LISR & (1U << 3)) ? 1 : 0;
}

uint8_t DMA_IsDirectModeError(DMA_Stream_TypeDef *DMA_Stream)
{
    return (DMA2->LISR & (1U << 2)) ? 1 : 0;
}

uint8_t DMA_IsFIFOError(DMA_Stream_TypeDef *DMA_Stream)
{
    return (DMA2->LISR & (1U << 0)) ? 1 : 0;
}
