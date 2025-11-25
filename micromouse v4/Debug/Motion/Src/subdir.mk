################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Motion/Src/drive.c \
../Motion/Src/foc.c \
../Motion/Src/sensor.c 

OBJS += \
./Motion/Src/drive.o \
./Motion/Src/foc.o \
./Motion/Src/sensor.o 

C_DEPS += \
./Motion/Src/drive.d \
./Motion/Src/foc.d \
./Motion/Src/sensor.d 


# Each subdirectory must supply rules for building sources it contributes
Motion/Src/%.o Motion/Src/%.su Motion/Src/%.cyclo: ../Motion/Src/%.c Motion/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H533xx -c -I../X-CUBE-TOF1/Target -I../Core/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H5xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/Components/Common -I../Drivers/BSP/Components/vl53l4cx/modules -I../Drivers/BSP/Components/vl53l4cx/porting -I../Drivers/BSP/Components/vl53l4cx -I../Drivers/BSP/53L4A2 -I"D:/ZETIN/micromouse v4/Devices/Inc" -I"D:/ZETIN/micromouse v4/Drivers/BSP/ST7735" -I"D:/ZETIN/micromouse v4/Motion/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Motion-2f-Src

clean-Motion-2f-Src:
	-$(RM) ./Motion/Src/drive.cyclo ./Motion/Src/drive.d ./Motion/Src/drive.o ./Motion/Src/drive.su ./Motion/Src/foc.cyclo ./Motion/Src/foc.d ./Motion/Src/foc.o ./Motion/Src/foc.su ./Motion/Src/sensor.cyclo ./Motion/Src/sensor.d ./Motion/Src/sensor.o ./Motion/Src/sensor.su

.PHONY: clean-Motion-2f-Src

