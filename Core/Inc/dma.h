#ifndef DMA_H_
#define DMA_H_

#include "stm32f4xx.h"
#include <stdint.h>

typedef enum
{
    DMA_PERIPHERAL_TO_MEMORY = 0,
    DMA_MEMORY_TO_PERIPHERAL,
    DMA_MEMORY_TO_MEMORY
} DMA_Direction;

typedef enum
{
    DMA_DATA_SIZE_8BIT = 0,
    DMA_DATA_SIZE_16BIT,
    DMA_DATA_SIZE_32BIT
} DMA_DataSize;

typedef enum
{
    DMA_PRIORITY_LOW = 0,
    DMA_PRIORITY_MEDIUM,
    DMA_PRIORITY_HIGH,
    DMA_PRIORITY_VERY_HIGH
} DMA_Priority;

typedef struct
{
    uint32_t Channel;
    DMA_Direction Direction;
    DMA_DataSize PeripheralDataSize;
    DMA_DataSize MemoryDataSize;
    uint8_t PeripheralIncrement;
    uint8_t MemoryIncrement;
    uint8_t CircularMode;
    DMA_Priority Priority;
    uint32_t NumberOfTransfers;
    uint32_t PeripheralAddress;
    uint32_t MemoryAddress;
} DMA_InitTypeDef;

void DMA_Init(DMA_Stream_TypeDef *DMA_Stream, DMA_InitTypeDef *DMA_InitStruct);
void DMA_Start(DMA_Stream_TypeDef *DMA_Stream);
void DMA_Stop(DMA_Stream_TypeDef *DMA_Stream);
uint8_t DMA_IsTransferComplete(DMA_Stream_TypeDef *DMA_Stream);
void DMA_ClearTransferComplete(DMA_Stream_TypeDef *DMA_Stream);
void DMA_EnableTransferCompleteInterrupt(DMA_Stream_TypeDef *DMA_Stream);
void DMA_EnableHalfTransferInterrupt(DMA_Stream_TypeDef *DMA_Stream);
void DMA_EnableIRQ(void);
void DMA_ClearFlags(DMA_Stream_TypeDef *DMA_Stream);
uint8_t DMA_IsEnabled(DMA_Stream_TypeDef *DMA_Stream);
void DMA_DisableTransferCompleteInterrupt(DMA_Stream_TypeDef *DMA_Stream);
void DMA_DisableHalfTransferInterrupt(DMA_Stream_TypeDef *DMA_Stream);
uint8_t DMA_IsTransferError(DMA_Stream_TypeDef *DMA_Stream);
uint8_t DMA_IsDirectModeError(DMA_Stream_TypeDef *DMA_Stream);
uint8_t DMA_IsFIFOError(DMA_Stream_TypeDef *DMA_Stream);
uint8_t DMA_ValidateConfig(DMA_InitTypeDef *DMA_InitStruct);

#endif
