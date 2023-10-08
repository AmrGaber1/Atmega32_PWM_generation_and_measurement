################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../hal/seven_segment/seven_segment.c 

OBJS += \
./hal/seven_segment/seven_segment.o 

C_DEPS += \
./hal/seven_segment/seven_segment.d 


# Each subdirectory must supply rules for building sources it contributes
hal/seven_segment/%.o: ../hal/seven_segment/%.c hal/seven_segment/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g3 -gdwarf-2 -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


