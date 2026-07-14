################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Drivers/I2C.c \
../Core/Src/Drivers/SPI.c \
../Core/Src/Drivers/uart.c 

OBJS += \
./Core/Src/Drivers/I2C.o \
./Core/Src/Drivers/SPI.o \
./Core/Src/Drivers/uart.o 

C_DEPS += \
./Core/Src/Drivers/I2C.d \
./Core/Src/Drivers/SPI.d \
./Core/Src/Drivers/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Drivers/%.o Core/Src/Drivers/%.su Core/Src/Drivers/%.cyclo: ../Core/Src/Drivers/%.c Core/Src/Drivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Drivers

clean-Core-2f-Src-2f-Drivers:
	-$(RM) ./Core/Src/Drivers/I2C.cyclo ./Core/Src/Drivers/I2C.d ./Core/Src/Drivers/I2C.o ./Core/Src/Drivers/I2C.su ./Core/Src/Drivers/SPI.cyclo ./Core/Src/Drivers/SPI.d ./Core/Src/Drivers/SPI.o ./Core/Src/Drivers/SPI.su ./Core/Src/Drivers/uart.cyclo ./Core/Src/Drivers/uart.d ./Core/Src/Drivers/uart.o ./Core/Src/Drivers/uart.su

.PHONY: clean-Core-2f-Src-2f-Drivers

