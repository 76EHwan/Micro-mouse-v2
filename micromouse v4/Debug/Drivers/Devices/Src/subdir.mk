################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Devices/Src/DRV8316C.c 

OBJS += \
./Drivers/Devices/Src/DRV8316C.o 

C_DEPS += \
./Drivers/Devices/Src/DRV8316C.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Devices/Src/%.o Drivers/Devices/Src/%.su Drivers/Devices/Src/%.cyclo: ../Drivers/Devices/Src/%.c Drivers/Devices/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H533xx -c -I../X-CUBE-TOF1/Target -I../Core/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H5xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/Components/Common -I../Drivers/BSP/Components/vl53l4cx/modules -I../Drivers/BSP/Components/vl53l4cx/porting -I../Drivers/BSP/Components/vl53l4cx -I../Drivers/BSP/53L4A2 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-Devices-2f-Src

clean-Drivers-2f-Devices-2f-Src:
	-$(RM) ./Drivers/Devices/Src/DRV8316C.cyclo ./Drivers/Devices/Src/DRV8316C.d ./Drivers/Devices/Src/DRV8316C.o ./Drivers/Devices/Src/DRV8316C.su

.PHONY: clean-Drivers-2f-Devices-2f-Src

