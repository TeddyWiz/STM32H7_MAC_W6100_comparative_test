################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CoreMarkApp/App/app_coremarkapp.c 

OBJS += \
./CoreMarkApp/App/app_coremarkapp.o 

C_DEPS += \
./CoreMarkApp/App/app_coremarkapp.d 


# Each subdirectory must supply rules for building sources it contributes
CoreMarkApp/App/%.o CoreMarkApp/App/%.su CoreMarkApp/App/%.cyclo: ../CoreMarkApp/App/%.c CoreMarkApp/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../CoreMarkApp/App -I../CoreMarkApp -I../Utilities/3rdParty/MISC -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"D:/wiznet/Workspace_Code/10_W6100/STM32H7_MAC_W6100_comparative_test/Nucleo-H723_CoreMark_test/io6Library/Ethernet/W6100" -I"D:/wiznet/Workspace_Code/10_W6100/STM32H7_MAC_W6100_comparative_test/Nucleo-H723_CoreMark_test/io6Library/Ethernet/" -I"D:/wiznet/Workspace_Code/10_W6100/STM32H7_MAC_W6100_comparative_test/Nucleo-H723_CoreMark_test/io6Library/Application/loopback" -I"D:/wiznet/Workspace_Code/10_W6100/STM32H7_MAC_W6100_comparative_test/Nucleo-H723_CoreMark_test/io6Library/Application/" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-CoreMarkApp-2f-App

clean-CoreMarkApp-2f-App:
	-$(RM) ./CoreMarkApp/App/app_coremarkapp.cyclo ./CoreMarkApp/App/app_coremarkapp.d ./CoreMarkApp/App/app_coremarkapp.o ./CoreMarkApp/App/app_coremarkapp.su

.PHONY: clean-CoreMarkApp-2f-App

