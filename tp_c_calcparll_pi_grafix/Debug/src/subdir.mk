################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/gnuplot_i.c \
../src/main.c \
../src/tp_c_calcparll_pi_grafix.c 

OBJS += \
./src/gnuplot_i.o \
./src/main.o \
./src/tp_c_calcparll_pi_grafix.o 

C_DEPS += \
./src/gnuplot_i.d \
./src/main.d \
./src/tp_c_calcparll_pi_grafix.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


