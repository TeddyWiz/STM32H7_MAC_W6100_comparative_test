################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../io6Library/Application/loopback/loopback.c 

OBJS += \
./io6Library/Application/loopback/loopback.o 

C_DEPS += \
./io6Library/Application/loopback/loopback.d 


# Each subdirectory must supply rules for building sources it contributes
io6Library/Application/loopback/loopback.o: D:/wiznet/Workspace_Code/10_W6100/STM32H7_MAC_W6100_comparative_test/Nucleo-H723_CoreMark_test/io6Library/Application/loopback/loopback.c io6Library/Application/loopback/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../CoreMarkApp/App -I../CoreMarkApp -I../Utilities/3rdParty/MISC -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"D:/wiznet/Workspace_Code/10_W6100/STM32H7_MAC_W6100_comparative_test/Nucleo-H723_CoreMark_test/io6Library/Ethernet/W6100" -I"D:/wiznet/Workspace_Code/10_W6100/STM32H7_MAC_W6100_comparative_test/Nucleo-H723_CoreMark_test/io6Library/Ethernet/" -I"D:/wiznet/Workspace_Code/10_W6100/STM32H7_MAC_W6100_comparative_test/Nucleo-H723_CoreMark_test/io6Library/Application/loopback" -I"D:/wiznet/Workspace_Code/10_W6100/STM32H7_MAC_W6100_comparative_test/Nucleo-H723_CoreMark_test/io6Library/Application/" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-io6Library-2f-Application-2f-loopback

clean-io6Library-2f-Application-2f-loopback:
	-$(RM) ./io6Library/Application/loopback/loopback.cyclo ./io6Library/Application/loopback/loopback.d ./io6Library/Application/loopback/loopback.o ./io6Library/Application/loopback/loopback.su

.PHONY: clean-io6Library-2f-Application-2f-loopback

