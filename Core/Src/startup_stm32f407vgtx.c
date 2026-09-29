#include <stdint.h>

extern uint32_t _estack;
extern uint32_t _sdata, _edata, _sidata;
extern uint32_t _sbss, _ebss;

void Reset_Handler(void);
void Default_Handler(void);
void TIM2_IRQHandler(void);

#define WEAK_ALIAS __attribute__((weak, alias("Default_Handler")))

void NMI_Handler(void) WEAK_ALIAS;
void HardFault_Handler(void) WEAK_ALIAS;
void MemManage_Handler(void) WEAK_ALIAS;
void BusFault_Handler(void) WEAK_ALIAS;
void UsageFault_Handler(void) WEAK_ALIAS;
void SVC_Handler(void) WEAK_ALIAS;
void DebugMon_Handler(void) WEAK_ALIAS;
void PendSV_Handler(void) WEAK_ALIAS;
void SysTick_Handler(void) WEAK_ALIAS;

__attribute__((section(".isr_vector")))
void (* const vector_table[])(void) = {
    (void (*)(void))&_estack,
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0, 0, 0, 0,
    SVC_Handler,
    DebugMon_Handler,
    0,
    PendSV_Handler,
    SysTick_Handler,
    Default_Handler,  /* WWDG */
    Default_Handler,  /* PVD */
    Default_Handler,  /* TAMP_STAMP */
    Default_Handler,  /* RTC_WKUP */
    Default_Handler,  /* FLASH */
    Default_Handler,  /* RCC */
    Default_Handler,  /* EXTI0 */
    Default_Handler,  /* EXTI1 */
    Default_Handler,  /* EXTI2 */
    Default_Handler,  /* EXTI3 */
    Default_Handler,  /* EXTI4 */
    Default_Handler,  /* DMA1_Stream0 */
    Default_Handler,  /* DMA1_Stream1 */
    Default_Handler,  /* DMA1_Stream2 */
    Default_Handler,  /* DMA1_Stream3 */
    Default_Handler,  /* DMA1_Stream4 */
    Default_Handler,  /* DMA1_Stream5 */
    Default_Handler,  /* DMA1_Stream6 */
    Default_Handler,  /* ADC */
    Default_Handler,  /* CAN1_TX */
    Default_Handler,  /* CAN1_RX0 */
    Default_Handler,  /* CAN1_RX1 */
    Default_Handler,  /* CAN1_SCE */
    Default_Handler,  /* EXTI9_5 */
    Default_Handler,  /* TIM1_BRK_TIM9 */
    Default_Handler,  /* TIM1_UP_TIM10 */
    Default_Handler,  /* TIM1_TRG_COM_TIM11 */
    Default_Handler,  /* TIM1_CC */
    TIM2_IRQHandler,
};

void Default_Handler(void) {
    while (1) { }
}

void Reset_Handler(void) {
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata) *dst++ = *src++;

    dst = &_sbss;
    while (dst < &_ebss) *dst++ = 0;

    extern int main(void);
    main();

    while (1) { }
}
