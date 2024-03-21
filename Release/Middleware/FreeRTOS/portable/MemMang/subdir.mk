################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middleware/FreeRTOS/portable/MemMang/heap_4.c 

OBJS += \
./Middleware/FreeRTOS/portable/MemMang/heap_4.o 

C_DEPS += \
./Middleware/FreeRTOS/portable/MemMang/heap_4.d 


# Each subdirectory must supply rules for building sources it contributes
Middleware/FreeRTOS/portable/MemMang/%.o: ../Middleware/FreeRTOS/portable/MemMang/%.c Middleware/FreeRTOS/portable/MemMang/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m23 -march=armv8-m.base -mthumb -mlittle-endian -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Core/Inc" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/XNO_Elib/Library" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/XNO_Elib/Driver" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/NDP120" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/NDP120/Inc" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/NDP120/ilib/inc/syntiant_ilib" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/NDP120/ilib/inc/syntiant_packager" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/NDP120/ilib/inc/syntiant-dsp-firmware" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/NDP120/ilib/inc/syntiant-firmware" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Middleware/CMSIS" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Middleware/CMSIS/GCC" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Middleware/FreeRTOS/portable/GCC/ARM_CM0" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Middleware/FreeRTOS/include" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Middleware/CMSIS/Include" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Middleware/Drivers/inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


