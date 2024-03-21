################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../NDP120/Src/ndp120_bringup_tests.c \
../NDP120/Src/syntiant_platform.c \
../NDP120/Src/syntiant_platform_afe.c \
../NDP120/Src/syntiant_platform_audio.c \
../NDP120/Src/syntiant_platform_iif.c 

OBJS += \
./NDP120/Src/ndp120_bringup_tests.o \
./NDP120/Src/syntiant_platform.o \
./NDP120/Src/syntiant_platform_afe.o \
./NDP120/Src/syntiant_platform_audio.o \
./NDP120/Src/syntiant_platform_iif.o 

C_DEPS += \
./NDP120/Src/ndp120_bringup_tests.d \
./NDP120/Src/syntiant_platform.d \
./NDP120/Src/syntiant_platform_afe.d \
./NDP120/Src/syntiant_platform_audio.d \
./NDP120/Src/syntiant_platform_iif.d 


# Each subdirectory must supply rules for building sources it contributes
NDP120/Src/%.o: ../NDP120/Src/%.c NDP120/Src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m23 -march=armv8-m.base -mthumb -mlittle-endian -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Core/Inc" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/XNO_Elib/Library" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/XNO_Elib/Driver" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/NDP120" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/NDP120/Inc" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/NDP120/ilib/inc/syntiant_ilib" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/NDP120/ilib/inc/syntiant_packager" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/NDP120/ilib/inc/syntiant-dsp-firmware" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/NDP120/ilib/inc/syntiant-firmware" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Middleware/CMSIS" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Middleware/CMSIS/GCC" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Middleware/FreeRTOS/portable/GCC/ARM_CM0" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Middleware/FreeRTOS/include" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Middleware/CMSIS/Include" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Middleware/Drivers/inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


