################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middleware/Drivers/src/acmp.c \
../Middleware/Drivers/src/bpwm.c \
../Middleware/Drivers/src/can.c \
../Middleware/Drivers/src/clk.c \
../Middleware/Drivers/src/crc.c \
../Middleware/Drivers/src/crypto.c \
../Middleware/Drivers/src/dac.c \
../Middleware/Drivers/src/dpm.c \
../Middleware/Drivers/src/eadc.c \
../Middleware/Drivers/src/ebi.c \
../Middleware/Drivers/src/ecap.c \
../Middleware/Drivers/src/epwm.c \
../Middleware/Drivers/src/ewdt.c \
../Middleware/Drivers/src/ewwdt.c \
../Middleware/Drivers/src/fmc.c \
../Middleware/Drivers/src/fvc.c \
../Middleware/Drivers/src/gpio.c \
../Middleware/Drivers/src/i2c.c \
../Middleware/Drivers/src/i2s.c \
../Middleware/Drivers/src/keystore.c \
../Middleware/Drivers/src/lcd.c \
../Middleware/Drivers/src/pdma.c \
../Middleware/Drivers/src/qei.c \
../Middleware/Drivers/src/qspi.c \
../Middleware/Drivers/src/retarget.c \
../Middleware/Drivers/src/rng.c \
../Middleware/Drivers/src/rtc.c \
../Middleware/Drivers/src/sc.c \
../Middleware/Drivers/src/scuart.c \
../Middleware/Drivers/src/sdh.c \
../Middleware/Drivers/src/spi.c \
../Middleware/Drivers/src/sys.c \
../Middleware/Drivers/src/tamper.c \
../Middleware/Drivers/src/timer.c \
../Middleware/Drivers/src/timer_pwm.c \
../Middleware/Drivers/src/uart.c \
../Middleware/Drivers/src/usbd.c \
../Middleware/Drivers/src/usci_i2c.c \
../Middleware/Drivers/src/usci_spi.c \
../Middleware/Drivers/src/usci_uart.c \
../Middleware/Drivers/src/wdt.c \
../Middleware/Drivers/src/wwdt.c 

OBJS += \
./Middleware/Drivers/src/acmp.o \
./Middleware/Drivers/src/bpwm.o \
./Middleware/Drivers/src/can.o \
./Middleware/Drivers/src/clk.o \
./Middleware/Drivers/src/crc.o \
./Middleware/Drivers/src/crypto.o \
./Middleware/Drivers/src/dac.o \
./Middleware/Drivers/src/dpm.o \
./Middleware/Drivers/src/eadc.o \
./Middleware/Drivers/src/ebi.o \
./Middleware/Drivers/src/ecap.o \
./Middleware/Drivers/src/epwm.o \
./Middleware/Drivers/src/ewdt.o \
./Middleware/Drivers/src/ewwdt.o \
./Middleware/Drivers/src/fmc.o \
./Middleware/Drivers/src/fvc.o \
./Middleware/Drivers/src/gpio.o \
./Middleware/Drivers/src/i2c.o \
./Middleware/Drivers/src/i2s.o \
./Middleware/Drivers/src/keystore.o \
./Middleware/Drivers/src/lcd.o \
./Middleware/Drivers/src/pdma.o \
./Middleware/Drivers/src/qei.o \
./Middleware/Drivers/src/qspi.o \
./Middleware/Drivers/src/retarget.o \
./Middleware/Drivers/src/rng.o \
./Middleware/Drivers/src/rtc.o \
./Middleware/Drivers/src/sc.o \
./Middleware/Drivers/src/scuart.o \
./Middleware/Drivers/src/sdh.o \
./Middleware/Drivers/src/spi.o \
./Middleware/Drivers/src/sys.o \
./Middleware/Drivers/src/tamper.o \
./Middleware/Drivers/src/timer.o \
./Middleware/Drivers/src/timer_pwm.o \
./Middleware/Drivers/src/uart.o \
./Middleware/Drivers/src/usbd.o \
./Middleware/Drivers/src/usci_i2c.o \
./Middleware/Drivers/src/usci_spi.o \
./Middleware/Drivers/src/usci_uart.o \
./Middleware/Drivers/src/wdt.o \
./Middleware/Drivers/src/wwdt.o 

C_DEPS += \
./Middleware/Drivers/src/acmp.d \
./Middleware/Drivers/src/bpwm.d \
./Middleware/Drivers/src/can.d \
./Middleware/Drivers/src/clk.d \
./Middleware/Drivers/src/crc.d \
./Middleware/Drivers/src/crypto.d \
./Middleware/Drivers/src/dac.d \
./Middleware/Drivers/src/dpm.d \
./Middleware/Drivers/src/eadc.d \
./Middleware/Drivers/src/ebi.d \
./Middleware/Drivers/src/ecap.d \
./Middleware/Drivers/src/epwm.d \
./Middleware/Drivers/src/ewdt.d \
./Middleware/Drivers/src/ewwdt.d \
./Middleware/Drivers/src/fmc.d \
./Middleware/Drivers/src/fvc.d \
./Middleware/Drivers/src/gpio.d \
./Middleware/Drivers/src/i2c.d \
./Middleware/Drivers/src/i2s.d \
./Middleware/Drivers/src/keystore.d \
./Middleware/Drivers/src/lcd.d \
./Middleware/Drivers/src/pdma.d \
./Middleware/Drivers/src/qei.d \
./Middleware/Drivers/src/qspi.d \
./Middleware/Drivers/src/retarget.d \
./Middleware/Drivers/src/rng.d \
./Middleware/Drivers/src/rtc.d \
./Middleware/Drivers/src/sc.d \
./Middleware/Drivers/src/scuart.d \
./Middleware/Drivers/src/sdh.d \
./Middleware/Drivers/src/spi.d \
./Middleware/Drivers/src/sys.d \
./Middleware/Drivers/src/tamper.d \
./Middleware/Drivers/src/timer.d \
./Middleware/Drivers/src/timer_pwm.d \
./Middleware/Drivers/src/uart.d \
./Middleware/Drivers/src/usbd.d \
./Middleware/Drivers/src/usci_i2c.d \
./Middleware/Drivers/src/usci_spi.d \
./Middleware/Drivers/src/usci_uart.d \
./Middleware/Drivers/src/wdt.d \
./Middleware/Drivers/src/wwdt.d 


# Each subdirectory must supply rules for building sources it contributes
Middleware/Drivers/src/%.o: ../Middleware/Drivers/src/%.c Middleware/Drivers/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m23 -march=armv8-m.base -mthumb -mlittle-endian -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Core/Inc" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/XNO_Elib/Library" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/XNO_Elib/Driver" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/NDP120" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/NDP120/Inc" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/NDP120/ilib/inc/syntiant_ilib" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/NDP120/ilib/inc/syntiant_packager" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/NDP120/ilib/inc/syntiant-dsp-firmware" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/NDP120/ilib/inc/syntiant-firmware" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Middleware/CMSIS" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Middleware/CMSIS/GCC" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Middleware/FreeRTOS/portable/GCC/ARM_CM0" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Middleware/FreeRTOS/include" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Middleware/CMSIS/Include" -I"D:/Ravikala/IOT_KIT_WS/XNO_EX_FLASH_APP/Middleware/Drivers/inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


