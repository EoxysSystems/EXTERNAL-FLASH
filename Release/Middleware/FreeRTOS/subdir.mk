################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middleware/FreeRTOS/croutine.c \
../Middleware/FreeRTOS/event_groups.c \
../Middleware/FreeRTOS/list.c \
../Middleware/FreeRTOS/queue.c \
../Middleware/FreeRTOS/stream_buffer.c \
../Middleware/FreeRTOS/tasks.c \
../Middleware/FreeRTOS/timers.c 

OBJS += \
./Middleware/FreeRTOS/croutine.o \
./Middleware/FreeRTOS/event_groups.o \
./Middleware/FreeRTOS/list.o \
./Middleware/FreeRTOS/queue.o \
./Middleware/FreeRTOS/stream_buffer.o \
./Middleware/FreeRTOS/tasks.o \
./Middleware/FreeRTOS/timers.o 

C_DEPS += \
./Middleware/FreeRTOS/croutine.d \
./Middleware/FreeRTOS/event_groups.d \
./Middleware/FreeRTOS/list.d \
./Middleware/FreeRTOS/queue.d \
./Middleware/FreeRTOS/stream_buffer.d \
./Middleware/FreeRTOS/tasks.d \
./Middleware/FreeRTOS/timers.d 


# Each subdirectory must supply rules for building sources it contributes
Middleware/FreeRTOS/%.o: ../Middleware/FreeRTOS/%.c Middleware/FreeRTOS/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m23 -march=armv8-m.base -mthumb -mlittle-endian -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Core/Inc" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/XNO_Elib/Library" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/XNO_Elib/Driver" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/NDP120" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/NDP120/Inc" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/NDP120/ilib/inc/syntiant_ilib" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/NDP120/ilib/inc/syntiant_packager" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/NDP120/ilib/inc/syntiant-dsp-firmware" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/NDP120/ilib/inc/syntiant-firmware" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Middleware/CMSIS" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Middleware/CMSIS/GCC" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Middleware/FreeRTOS/portable/GCC/ARM_CM0" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Middleware/FreeRTOS/include" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Middleware/CMSIS/Include" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Middleware/Drivers/inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


