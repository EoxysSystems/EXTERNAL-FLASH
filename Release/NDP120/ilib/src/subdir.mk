################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../NDP120/ilib/src/checksum.c \
../NDP120/ilib/src/snprintf.c \
../NDP120/ilib/src/syntiant_ndp.c \
../NDP120/ilib/src/syntiant_ndp120.c \
../NDP120/ilib/src/syntiant_ndp120_config.c \
../NDP120/ilib/src/syntiant_ndp120_config_misc.c \
../NDP120/ilib/src/syntiant_ndp120_mailbox.c \
../NDP120/ilib/src/syntiant_ndp120_ph.c \
../NDP120/ilib/src/syntiant_ndp_error.c \
../NDP120/ilib/src/syntiant_package.c \
../NDP120/ilib/src/syntiant_portability.c 

OBJS += \
./NDP120/ilib/src/checksum.o \
./NDP120/ilib/src/snprintf.o \
./NDP120/ilib/src/syntiant_ndp.o \
./NDP120/ilib/src/syntiant_ndp120.o \
./NDP120/ilib/src/syntiant_ndp120_config.o \
./NDP120/ilib/src/syntiant_ndp120_config_misc.o \
./NDP120/ilib/src/syntiant_ndp120_mailbox.o \
./NDP120/ilib/src/syntiant_ndp120_ph.o \
./NDP120/ilib/src/syntiant_ndp_error.o \
./NDP120/ilib/src/syntiant_package.o \
./NDP120/ilib/src/syntiant_portability.o 

C_DEPS += \
./NDP120/ilib/src/checksum.d \
./NDP120/ilib/src/snprintf.d \
./NDP120/ilib/src/syntiant_ndp.d \
./NDP120/ilib/src/syntiant_ndp120.d \
./NDP120/ilib/src/syntiant_ndp120_config.d \
./NDP120/ilib/src/syntiant_ndp120_config_misc.d \
./NDP120/ilib/src/syntiant_ndp120_mailbox.d \
./NDP120/ilib/src/syntiant_ndp120_ph.d \
./NDP120/ilib/src/syntiant_ndp_error.d \
./NDP120/ilib/src/syntiant_package.d \
./NDP120/ilib/src/syntiant_portability.d 


# Each subdirectory must supply rules for building sources it contributes
NDP120/ilib/src/%.o: ../NDP120/ilib/src/%.c NDP120/ilib/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m23 -march=armv8-m.base -mthumb -mlittle-endian -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Core/Inc" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/XNO_Elib/Library" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/XNO_Elib/Driver" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/NDP120" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/NDP120/Inc" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/NDP120/ilib/inc/syntiant_ilib" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/NDP120/ilib/inc/syntiant_packager" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/NDP120/ilib/inc/syntiant-dsp-firmware" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/NDP120/ilib/inc/syntiant-firmware" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Middleware/CMSIS" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Middleware/CMSIS/GCC" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Middleware/FreeRTOS/portable/GCC/ARM_CM0" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Middleware/FreeRTOS/include" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Middleware/CMSIS/Include" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Middleware/Drivers/inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


