################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/BallTracker.cc 

CPP_SRCS += \
../src/BallDetection.cpp \
../src/PWM.cpp \
../src/Servo.cpp \
../src/Test.cpp \
../src/ThresholdEvaluator.cpp 

CC_DEPS += \
./src/BallTracker.d 

OBJS += \
./src/BallDetection.o \
./src/BallTracker.o \
./src/PWM.o \
./src/Servo.o \
./src/Test.o \
./src/ThresholdEvaluator.o 

CPP_DEPS += \
./src/BallDetection.d \
./src/PWM.d \
./src/Servo.d \
./src/Test.d \
./src/ThresholdEvaluator.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/ -I"/home/pi/CAS_Projektarbeit/CAS-Embedded-System-2018---Object-Tracking/02_Project/src" -I/usr/local/include/opencv2 -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/ -I"/home/pi/CAS_Projektarbeit/CAS-Embedded-System-2018---Object-Tracking/02_Project/src" -I/usr/local/include/opencv2 -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


