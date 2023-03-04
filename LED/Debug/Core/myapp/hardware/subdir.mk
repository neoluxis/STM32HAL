################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/myapp/hardware/key.c \
../Core/myapp/hardware/led.c 

OBJS += \
./Core/myapp/hardware/key.o \
./Core/myapp/hardware/led.o 

C_DEPS += \
./Core/myapp/hardware/key.d \
./Core/myapp/hardware/led.d 


# Each subdirectory must supply rules for building sources it contributes
Core/myapp/hardware/%.o Core/myapp/hardware/%.su: ../Core/myapp/hardware/%.c Core/myapp/hardware/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/myapp/hardware -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-myapp-2f-hardware

clean-Core-2f-myapp-2f-hardware:
	-$(RM) ./Core/myapp/hardware/key.d ./Core/myapp/hardware/key.o ./Core/myapp/hardware/key.su ./Core/myapp/hardware/led.d ./Core/myapp/hardware/led.o ./Core/myapp/hardware/led.su

.PHONY: clean-Core-2f-myapp-2f-hardware

