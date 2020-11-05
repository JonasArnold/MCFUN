################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include source/subdir.mk
-include drivers/subdir.mk
-include device/subdir.mk
-include board/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Ueb05.3_Timer_Output_Compare.axf

# Tool invocations
Ueb05.3_Timer_Output_Compare.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -Xlinker -Map="Ueb05.3_Timer_Output_Compare.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -T MK22FN512xxx12_Template_Debug.ld -o "Ueb05.3_Timer_Output_Compare.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) Ueb05.3_Timer_Output_Compare.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "Ueb05.3_Timer_Output_Compare.axf" ; arm-none-eabi-objdump -S Ueb05.3_Timer_Output_Compare.axf > Ueb05.3_Timer_Output_Compare.lst ; #arm-none-eabi-objcopy -O ihex Ueb05.3_Timer_Output_Compare.axf Ueb05.3_Timer_Output_Compare.hex ; # arm-none-eabi-objcopy -v -O binary "Ueb05.3_Timer_Output_Compare.axf" "Ueb05.3_Timer_Output_Compare.bin" ; # checksum -p MK22FN512xxx12 -d "Ueb05.3_Timer_Output_Compare.bin"
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
