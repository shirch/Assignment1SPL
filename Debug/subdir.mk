################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Commands.cpp \
../FileSystem.cpp \
../Files.cpp \
../Main.cpp 

OBJS += \
./Commands.o \
./FileSystem.o \
./Files.o \
./Main.o 

CPP_DEPS += \
./Commands.d \
./FileSystem.d \
./Files.d \
./Main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


