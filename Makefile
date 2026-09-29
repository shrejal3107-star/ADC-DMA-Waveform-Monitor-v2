TARGET = waveform_monitor
BUILD_DIR = build

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

CPU = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS = $(CPU) -O2 -Wall -ffreestanding -ICore/Inc
LDFLAGS = $(CPU) -nostartfiles -specs=nano.specs -specs=nosys.specs \
          -TSTM32F407VGTX_FLASH.ld -Wl,-Map=$(BUILD_DIR)/$(TARGET).map -Wl,--gc-sections

# Build for Renode simulation (ADC polled instead of DMA-triggered — see
# README for why). Omit for a real-hardware build.
CFLAGS += -DRENODE_DEMO

SOURCES = Core/Src/main.c \
          Core/Src/adc.c \
          Core/Src/dma.c \
          Core/Src/usart.c \
          Core/Src/startup_stm32f407vgtx.c

OBJECTS = $(patsubst Core/Src/%.c,$(BUILD_DIR)/%.o,$(SOURCES))

all: $(BUILD_DIR)/$(TARGET).elf

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: Core/Src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	$(SIZE) $@

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
