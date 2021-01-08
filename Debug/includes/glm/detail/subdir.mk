################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../includes/glm/detail/glm.cpp 

OBJS += \
./includes/glm/detail/glm.o 

CPP_DEPS += \
./includes/glm/detail/glm.d 


# Each subdirectory must supply rules for building sources it contributes
includes/glm/detail/%.o: ../includes/glm/detail/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/arun/eclipse-workspace/openGL/includes" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


