################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../io6Library/Internet/DHCP6/dhcpv6.c 

OBJS += \
./io6Library/Internet/DHCP6/dhcpv6.o 

C_DEPS += \
./io6Library/Internet/DHCP6/dhcpv6.d 


# Each subdirectory must supply rules for building sources it contributes
io6Library/Internet/DHCP6/dhcpv6.o: D:/wiznet/Workspace_Code/10_W6100/STM32H7_MAC_W6100_comparative_test/STM32H723_W6100_Test/io6Library/Internet/DHCP6/dhcpv6.c io6Library/Internet/DHCP6/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"D:/wiznet/Workspace_Code/10_W6100/STM32H7_MAC_W6100_comparative_test/STM32H723_W6100_Test/io6Library/Ethernet" -I"D:/wiznet/Workspace_Code/10_W6100/STM32H7_MAC_W6100_comparative_test/STM32H723_W6100_Test/io6Library/Ethernet/W6100" -I"D:/wiznet/Workspace_Code/10_W6100/STM32H7_MAC_W6100_comparative_test/STM32H723_W6100_Test/io6Library/Application/loopback" -I"D:/wiznet/Workspace_Code/10_W6100/STM32H7_MAC_W6100_comparative_test/STM32H723_W6100_Test/io6Library/Application" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-io6Library-2f-Internet-2f-DHCP6

clean-io6Library-2f-Internet-2f-DHCP6:
	-$(RM) ./io6Library/Internet/DHCP6/dhcpv6.cyclo ./io6Library/Internet/DHCP6/dhcpv6.d ./io6Library/Internet/DHCP6/dhcpv6.o ./io6Library/Internet/DHCP6/dhcpv6.su

.PHONY: clean-io6Library-2f-Internet-2f-DHCP6

