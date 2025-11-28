################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Devices/Src/DRV8316C.c \
../Devices/Src/LSM6DS3TR.c 

OBJS += \
./Devices/Src/DRV8316C.o \
./Devices/Src/LSM6DS3TR.o 

C_DEPS += \
./Devices/Src/DRV8316C.d \
./Devices/Src/LSM6DS3TR.d 


# Each subdirectory must supply rules for building sources it contributes
Devices/Src/%.o Devices/Src/%.su Devices/Src/%.cyclo: ../Devices/Src/%.c Devices/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H533xx -c -I../X-CUBE-TOF1/Target -I../Core/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H5xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/Components/Common -I../Drivers/BSP/Components/vl53l4cx/modules -I../Drivers/BSP/Components/vl53l4cx/porting -I../Drivers/BSP/Components/vl53l4cx -I../Drivers/BSP/53L4A2 -I"C:/SPB_Data/git/Micro-mouse-v2/micromouse v4/Devices/Inc" -I"C:/SPB_Data/git/Micro-mouse-v2/micromouse v4/Drivers/BSP/ST7735" -I"C:/SPB_Data/git/Micro-mouse-v2/micromouse v4/Motion/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Devices-2f-Src

clean-Devices-2f-Src:
	-$(RM) ./Devices/Src/DRV8316C.cyclo ./Devices/Src/DRV8316C.d ./Devices/Src/DRV8316C.o ./Devices/Src/DRV8316C.su ./Devices/Src/LSM6DS3TR.cyclo ./Devices/Src/LSM6DS3TR.d ./Devices/Src/LSM6DS3TR.o ./Devices/Src/LSM6DS3TR.su

.PHONY: clean-Devices-2f-Src

