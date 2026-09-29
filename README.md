# ADC-DMA Based Waveform Monitor

Bare-metal STM32F407 firmware demonstrating CPU-free analog signal
acquisition: ADC1 sampling in continuous mode, DMA2 moving each
conversion result into memory without CPU involvement, and a hardware
timer (TIM2) independently triggering periodic UART reporting of the
latest sample.

All peripheral drivers (ADC, DMA, USART) are written directly against
the register map in RM0090 — no HAL, no CMSIS abstraction layer.

## Architecture

```
ADC1 (continuous mode, channel 0 / PA0)
    --> DMA2 Stream0 (circular, ADC1->DR to SRAM buffer)
TIM2 (10 Hz interrupt)
    --> reads latest DMA buffer sample, sends over USART2
```

The reporting rate (TIM2) is intentionally decoupled from the sampling
rate (ADC/DMA) — the ADC samples as fast as it's configured to, while
TIM2 independently decides how often to report the latest value. This
avoids flooding UART with every single conversion.

## Building

```
make
```

Produces `build/waveform_monitor.elf`.

### Renode simulation note

Renode's stock STM32 DMA model doesn't fire per-conversion
peripheral-to-memory DMA requests the way real silicon does — it
completes the whole configured transfer once, immediately, instead of
waiting for each ADC conversion's DMA request pulse. To demonstrate
the reporting/UART side of the pipeline in Renode, this build defines
`RENODE_DEMO`, which polls the ADC directly to fill the buffer instead
of relying on DMA. The real DMA-driven path (`DMA_Init`/`DMA_Start`)
is unchanged and is what runs on real hardware — build without
`-DRENODE_DEMO` (remove that line from the Makefile) for a
hardware/production build.

## Running in Renode

```
mach create "wf"
machine LoadPlatformDescription @platforms/boards/stm32f4_discovery-kit.repl
machine LoadPlatformDescription @add_adc.repl
showAnalyzer sysbus.usart2
sysbus LoadELF @build/waveform_monitor.elf
start
```

`add_adc.repl` adds an ADC1 peripheral model, since the stock
discovery-kit platform description doesn't include one.

## Files

- `Core/Src/main.c` — application entry point, peripheral configuration
- `Core/Src/adc.c` / `Core/Inc/adc.h` — ADC driver
- `Core/Src/dma.c` / `Core/Inc/dma.h` — DMA driver
- `Core/Src/usart.c` / `Core/Inc/usart.h` — USART driver
- `Core/Src/startup_stm32f407vgtx.c` — vector table, reset handler
- `Core/Inc/stm32f4xx.h` — register definitions for the peripherals used
- `STM32F407VGTX_FLASH.ld` — linker script
- `Makefile` — build system
