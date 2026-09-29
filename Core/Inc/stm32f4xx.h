#ifndef STM32F4XX_H_
#define STM32F4XX_H_

#include <stdint.h>

typedef struct {
    volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFR[2];
} GPIO_TypeDef;

typedef struct {
    volatile uint32_t CR, PLLCFGR, CFGR, CIR;
    volatile uint32_t AHB1RSTR, AHB2RSTR, AHB3RSTR, RESERVED0;
    volatile uint32_t APB1RSTR, APB2RSTR, RESERVED1[2];
    volatile uint32_t AHB1ENR, AHB2ENR, AHB3ENR, RESERVED2;
    volatile uint32_t APB1ENR, APB2ENR;
} RCC_TypeDef;

typedef struct {
    volatile uint32_t SR, CR1, CR2, SMPR1, SMPR2;
    volatile uint32_t JOFR1, JOFR2, JOFR3, JOFR4, HTR, LTR;
    volatile uint32_t SQR1, SQR2, SQR3, JSQR;
    volatile uint32_t JDR1, JDR2, JDR3, JDR4, DR;
} ADC_TypeDef;

typedef struct {
    volatile uint32_t CSR, CCR, CDR;
} ADC_Common_TypeDef;

typedef struct {
    volatile uint32_t CR, NDTR, PAR, M0AR, M1AR, FCR;
} DMA_Stream_TypeDef;

typedef struct {
    volatile uint32_t LISR, HISR, LIFCR, HIFCR;
} DMA_TypeDef;

typedef struct {
    volatile uint32_t CR1, CR2, SMCR, DIER, SR, EGR, CCMR1, CCMR2, CCER, CNT, PSC, ARR;
} TIM_TypeDef;

typedef struct {
    volatile uint32_t SR, DR, BRR, CR1, CR2, CR3, GTPR;
} USART_TypeDef;

#define RCC_BASE            0x40023800UL
#define GPIOA_BASE          0x40020000UL
#define ADC1_BASE           0x40012000UL
#define ADC_COMMON_BASE     0x40012300UL
#define DMA2_BASE           0x40026400UL
#define DMA2_STREAM0_BASE   (DMA2_BASE + 0x010UL)
#define TIM2_BASE           0x40000000UL
#define USART1_BASE         0x40011000UL
#define USART2_BASE         0x40004400UL
#define USART3_BASE         0x40004800UL

#define RCC          ((RCC_TypeDef*)RCC_BASE)
#define GPIOA        ((GPIO_TypeDef*)GPIOA_BASE)
#define ADC1         ((ADC_TypeDef*)ADC1_BASE)
#define ADC          ((ADC_Common_TypeDef*)ADC_COMMON_BASE)
#define DMA2_Stream0 ((DMA_Stream_TypeDef*)DMA2_STREAM0_BASE)
#define DMA2         ((DMA_TypeDef*)DMA2_BASE)
#define TIM2         ((TIM_TypeDef*)TIM2_BASE)
#define USART1       ((USART_TypeDef*)USART1_BASE)
#define USART2       ((USART_TypeDef*)USART2_BASE)
#define USART3       ((USART_TypeDef*)USART3_BASE)

#define NVIC_ISER0  (*(volatile uint32_t*)0xE000E100UL)
typedef enum { TIM2_IRQn = 28, DMA2_Stream0_IRQn = 56 } IRQn_Type;
static inline void NVIC_EnableIRQ(IRQn_Type irq) {
    NVIC_ISER0 |= (1UL << ((uint32_t)irq & 0x1FUL));
}

#endif
