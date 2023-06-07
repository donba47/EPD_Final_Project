################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../user_Driver/ili9341/ili9341.c 

OBJS += \
./user_Driver/ili9341/ili9341.o 

C_DEPS += \
./user_Driver/ili9341/ili9341.d 


# Each subdirectory must supply rules for building sources it contributes
user_Driver/ili9341/%.o user_Driver/ili9341/%.su user_Driver/ili9341/%.cyclo: ../user_Driver/ili9341/%.c user_Driver/ili9341/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../user_Driver -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-user_Driver-2f-ili9341

clean-user_Driver-2f-ili9341:
	-$(RM) ./user_Driver/ili9341/ili9341.cyclo ./user_Driver/ili9341/ili9341.d ./user_Driver/ili9341/ili9341.o ./user_Driver/ili9341/ili9341.su

.PHONY: clean-user_Driver-2f-ili9341

