##########################################################################################################################
# File automatically-generated by tool: [projectgenerator] version: [3.2.0] date: [Mon Oct 19 01:09:32 CST 2020] 
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# ChangeLog :
#	2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
# ------------------------------------------------

######################################
# target
######################################
TARGET = SmartFridge


######################################
# building variables
######################################
# debug build?
DEBUG = 0
# optimization
OPT = -O3

TOP_DIR = ../
LVGL_DIR = Drivers
LVGL_DIR_NAME = lvgl
#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
Src/main.c \
Src/gpio.c \
Src/stm32f1xx_it.c \
Src/stm32f1xx_hal_msp.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c \
Src/system_stm32f1xx.c \
Src/usart.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c \
Src/fsmc.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_fsmc.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_sram.c

USER_SOURCES = \
Src/debug.c \
Src/lcd_tft.c\
Src/task.c

C_SOURCES += $(USER_SOURCES)

ARCH_SRC = \
${wildcard $(TOP_DIR)/arch/arm/arm-v7m/cortex-m3/gcc/*.c} \
${wildcard $(TOP_DIR)/arch/arm/arm-v7m/common/*.c}

C_SOURCES += $(ARCH_SRC)

KERNEL_SRC = \
${wildcard $(TOP_DIR)/kernel/core/*.c}
C_SOURCES += $(KERNEL_SRC)

CMSIS_SRC = \
${wildcard $(TOP_DIR)/osal/cmsis_os/*.c}
C_SOURCES += $(CMSIS_SRC)

LVGL_SRC = \
${wildcard $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_core/*.c} \
${wildcard $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_draw/*.c} \
${wildcard $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_font/*.c} \
${wildcard $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_gpu/*.c} \
${wildcard $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_hal/*.c} \
${wildcard $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_misc/*.c} \
${wildcard $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_themes/*.c} \
${wildcard $(LVGL_DIR)/$(LVGL_DIR_NAME)/src/lv_widgets/*.c}
C_SOURCES += $(LVGL_SRC)






# ASM sources
ASM_SOURCES =  \
startup_stm32f103xe.s

ASM_SOURCES_S = \
$(TOP_DIR)/arch/arm/arm-v7m/cortex-m3/gcc/port_s.S


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m3

# fpu
# NONE for Cortex-M0/M0+/M3

# float-abi


# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32F103xE


# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-IInc \
-IDrivers/STM32F1xx_HAL_Driver/Inc \
-IDrivers/STM32F1xx_HAL_Driver/Inc/Legacy \
-IDrivers/CMSIS/Device/ST/STM32F1xx/Include \
-IDrivers/CMSIS/Include   \
-IDrivers/CMSIS/Include

KERNEL_INC = \
-I$(TOP_DIR)/kernel/core/include \
-I$(TOP_DIR)/kernel/pm/include \
-I$(TOP_DIR)/arch/arm/arm-v7m/common/include \
-I$(TOP_DIR)/arch/arm/arm-v7m/cortex-m3/gcc \
-I$(TOP_DIR)/board/TOS_CONFIG
C_INCLUDES += $(KERNEL_INC)

CMSIS_INC = \
-I $(TOP_DIR)/osal/cmsis_os
C_INCLUDES += $(CMSIS_INC)

LVGL_INC = \
-IDrivers/lvgl \
-IDrivers
C_INCLUDES += $(LVGL_INC)

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"
#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F103VETx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES_S:.S=.o)))
vpath %.S $(sort $(dir $(ASM_SOURCES_S)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.S Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
