################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Shaders/ShaderHelper.cpp 

OBJS += \
./Shaders/ShaderHelper.o 

CPP_DEPS += \
./Shaders/ShaderHelper.d 


# Each subdirectory must supply rules for building sources it contributes
Shaders/%.o: ../Shaders/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/arun/eclipse-workspace/openGL/includes" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


