################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/App/smart_blind.c 

OBJS += \
./Core/Src/App/smart_blind.o 

C_DEPS += \
./Core/Src/App/smart_blind.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/App/%.o Core/Src/App/%.su Core/Src/App/%.cyclo: ../Core/Src/App/%.c Core/Src/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-App

clean-Core-2f-Src-2f-App:
	-$(RM) ./Core/Src/App/smart_blind.cyclo ./Core/Src/App/smart_blind.d ./Core/Src/App/smart_blind.o ./Core/Src/App/smart_blind.su

.PHONY: clean-Core-2f-Src-2f-App

