################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../user_Driver/stm32f429i_discovery.c \
../user_Driver/stm32f429i_discovery_lcd.c \
../user_Driver/stm32f429i_discovery_sdram.c 

OBJS += \
./user_Driver/stm32f429i_discovery.o \
./user_Driver/stm32f429i_discovery_lcd.o \
./user_Driver/stm32f429i_discovery_sdram.o 

C_DEPS += \
./user_Driver/stm32f429i_discovery.d \
./user_Driver/stm32f429i_discovery_lcd.d \
./user_Driver/stm32f429i_discovery_sdram.d 


# Each subdirectory must supply rules for building sources it contributes
user_Driver/%.o user_Driver/%.su: ../user_Driver/%.c user_Driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../user_Driver -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-user_Driver

clean-user_Driver:
	-$(RM) ./user_Driver/stm32f429i_discovery.d ./user_Driver/stm32f429i_discovery.o ./user_Driver/stm32f429i_discovery.su ./user_Driver/stm32f429i_discovery_lcd.d ./user_Driver/stm32f429i_discovery_lcd.o ./user_Driver/stm32f429i_discovery_lcd.su ./user_Driver/stm32f429i_discovery_sdram.d ./user_Driver/stm32f429i_discovery_sdram.o ./user_Driver/stm32f429i_discovery_sdram.su

.PHONY: clean-user_Driver

