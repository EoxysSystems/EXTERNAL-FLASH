################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include XNO_Elib/subdir.mk
-include NDP120/ilib/src/subdir.mk
-include NDP120/Src/subdir.mk
-include NDP120/subdir.mk
-include Middleware/FreeRTOS/portable/MemMang/subdir.mk
-include Middleware/FreeRTOS/portable/GCC/ARM_CM0/subdir.mk
-include Middleware/FreeRTOS/subdir.mk
-include Middleware/Drivers/src/subdir.mk
-include Middleware/CMSIS/GCC/subdir.mk
-include Middleware/CMSIS/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := XNO_EX_FLASH_APP
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
XNO_EX_FLASH_APP.hex \

SECONDARY_SIZE += \
XNO_EX_FLASH_APP.siz \


# All Target
all: XNO_EX_FLASH_APP.elf secondary-outputs

# Tool invocations
XNO_EX_FLASH_APP.elf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM GNU C Linker'
	arm-none-eabi-gcc -mcpu=cortex-m23 -march=armv8-m.base -mthumb -mlittle-endian -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -T "D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/FreeRTOS.ld" -Xlinker --gc-sections -L"D:\Ravikala\IOT_KIT_WS\XNO_EX_FLASH_APP\XNO_Elib" -Wl,-Map,"XNO_EX_FLASH_APP.map" --specs=nano.specs -o "XNO_EX_FLASH_APP.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

XNO_EX_FLASH_APP.hex: XNO_EX_FLASH_APP.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "XNO_EX_FLASH_APP.elf"  "XNO_EX_FLASH_APP.hex"
	@echo 'Finished building: $@'
	@echo ' '

XNO_EX_FLASH_APP.siz: XNO_EX_FLASH_APP.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "XNO_EX_FLASH_APP.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) XNO_EX_FLASH_APP.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
