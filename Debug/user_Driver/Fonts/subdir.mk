################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../user_Driver/Fonts/font12.c \
../user_Driver/Fonts/font16.c \
../user_Driver/Fonts/font20.c \
../user_Driver/Fonts/font24.c \
../user_Driver/Fonts/font8.c 

OBJS += \
./user_Driver/Fonts/font12.o \
./user_Driver/Fonts/font16.o \
./user_Driver/Fonts/font20.o \
./user_Driver/Fonts/font24.o \
./user_Driver/Fonts/font8.o 

C_DEPS += \
./user_Driver/Fonts/font12.d \
./user_Driver/Fonts/font16.d \
./user_Driver/Fonts/font20.d \
./user_Driver/Fonts/font24.d \
./user_Driver/Fonts/font8.d 


# Each subdirectory must supply rules for building sources it contributes
user_Driver/Fonts/%.o user_Driver/Fonts/%.su: ../user_Driver/Fonts/%.c user_Driver/Fonts/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../user_Driver -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-user_Driver-2f-Fonts

clean-user_Driver-2f-Fonts:
	-$(RM) ./user_Driver/Fonts/font12.d ./user_Driver/Fonts/font12.o ./user_Driver/Fonts/font12.su ./user_Driver/Fonts/font16.d ./user_Driver/Fonts/font16.o ./user_Driver/Fonts/font16.su ./user_Driver/Fonts/font20.d ./user_Driver/Fonts/font20.o ./user_Driver/Fonts/font20.su ./user_Driver/Fonts/font24.d ./user_Driver/Fonts/font24.o ./user_Driver/Fonts/font24.su ./user_Driver/Fonts/font8.d ./user_Driver/Fonts/font8.o ./user_Driver/Fonts/font8.su

.PHONY: clean-user_Driver-2f-Fonts

