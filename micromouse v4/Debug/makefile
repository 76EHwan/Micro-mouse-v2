################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Motion/Src/subdir.mk
-include Drivers/STM32H5xx_HAL_Driver/Src/subdir.mk
-include Drivers/BSP/ST7735/subdir.mk
-include Drivers/BSP/Components/vl53l4cx/porting/subdir.mk
-include Drivers/BSP/Components/vl53l4cx/modules/subdir.mk
-include Drivers/BSP/Components/vl53l4cx/subdir.mk
-include Drivers/BSP/53L4A2/subdir.mk
-include Devices/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := micromouse v4
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
micromouse\ v4.elf \

MAP_FILES += \
micromouse\ v4.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
micromouse\ v4.list \


# All Target
all: main-build

# Main-build Target
main-build: micromouse\ v4.elf secondary-outputs

# Tool invocations
micromouse\ v4.elf micromouse\ v4.map: $(OBJS) $(USER_OBJS) C:\SPB_Data\git\Micro-mouse-v2\micromouse\ v4\STM32H533RETX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "micromouse v4.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m33 -T"C:\SPB_Data\git\Micro-mouse-v2\micromouse v4\STM32H533RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="micromouse v4.map" -Wl,--gc-sections -static -u _printf_float --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

micromouse\ v4.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "micromouse v4.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) default.size.stdout micromouse\ v4.elf micromouse\ v4.list micromouse\ v4.map micromouse\\ v4.elf
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
