################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Configs/I2C_CFG.c \
../Core/Src/Configs/SPI_CFG.c \
../Core/Src/Configs/TIM-Config.c \
../Core/Src/Configs/uart_CFG.c 

OBJS += \
./Core/Src/Configs/I2C_CFG.o \
./Core/Src/Configs/SPI_CFG.o \
./Core/Src/Configs/TIM-Config.o \
./Core/Src/Configs/uart_CFG.o 

C_DEPS += \
./Core/Src/Configs/I2C_CFG.d \
./Core/Src/Configs/SPI_CFG.d \
./Core/Src/Configs/TIM-Config.d \
./Core/Src/Configs/uart_CFG.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Configs/%.o Core/Src/Configs/%.su Core/Src/Configs/%.cyclo: ../Core/Src/Configs/%.c Core/Src/Configs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Configs

clean-Core-2f-Src-2f-Configs:
	-$(RM) ./Core/Src/Configs/I2C_CFG.cyclo ./Core/Src/Configs/I2C_CFG.d ./Core/Src/Configs/I2C_CFG.o ./Core/Src/Configs/I2C_CFG.su ./Core/Src/Configs/SPI_CFG.cyclo ./Core/Src/Configs/SPI_CFG.d ./Core/Src/Configs/SPI_CFG.o ./Core/Src/Configs/SPI_CFG.su ./Core/Src/Configs/TIM-Config.cyclo ./Core/Src/Configs/TIM-Config.d ./Core/Src/Configs/TIM-Config.o ./Core/Src/Configs/TIM-Config.su ./Core/Src/Configs/uart_CFG.cyclo ./Core/Src/Configs/uart_CFG.d ./Core/Src/Configs/uart_CFG.o ./Core/Src/Configs/uart_CFG.su

.PHONY: clean-Core-2f-Src-2f-Configs

