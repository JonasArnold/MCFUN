################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_smc.c 

OBJS += \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_smc.o 

C_DEPS += \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_smc.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK22FN512VLL12 -DCPU_MK22FN512VLL12_cm4 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__NEWLIB__ -I"C:\Users\jonas\OneDrive\HSLU\Semester3\MCFUN\Workspace\Ueb05.3_Timer_Output_Compare\board" -I"C:\Users\jonas\OneDrive\HSLU\Semester3\MCFUN\Workspace\Ueb05.3_Timer_Output_Compare\source" -I"C:\Users\jonas\OneDrive\HSLU\Semester3\MCFUN\Workspace\Ueb05.3_Timer_Output_Compare" -I"C:\Users\jonas\OneDrive\HSLU\Semester3\MCFUN\Workspace\Ueb05.3_Timer_Output_Compare\drivers" -I"C:\Users\jonas\OneDrive\HSLU\Semester3\MCFUN\Workspace\Ueb05.3_Timer_Output_Compare\device" -I"C:\Users\jonas\OneDrive\HSLU\Semester3\MCFUN\Workspace\Ueb05.3_Timer_Output_Compare\CMSIS" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__NEWLIB__ -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


