################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../io6Library/Internet/DNS/dns.c 

OBJS += \
./io6Library/Internet/DNS/dns.o 

C_DEPS += \
./io6Library/Internet/DNS/dns.d 


# Each subdirectory must supply rules for building sources it contributes
io6Library/Internet/DNS/dns.o: D:/wiznet/05_WorkSpace/04_w6100/03_STMh723_W6100_Test/Nucleo-H723_CoreMark_test/io6Library/Internet/DNS/dns.c io6Library/Internet/DNS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../CoreMarkApp/App -I../CoreMarkApp -I../Utilities/3rdParty/MISC -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"D:/wiznet/05_WorkSpace/04_w6100/03_STMh723_W6100_Test/Nucleo-H723_CoreMark_test/io6Library/Ethernet/W6100" -I"D:/wiznet/05_WorkSpace/04_w6100/03_STMh723_W6100_Test/Nucleo-H723_CoreMark_test/io6Library/Ethernet/" -I"D:/wiznet/05_WorkSpace/04_w6100/03_STMh723_W6100_Test/Nucleo-H723_CoreMark_test/io6Library/Application/loopback" -I"D:/wiznet/05_WorkSpace/04_w6100/03_STMh723_W6100_Test/Nucleo-H723_CoreMark_test/io6Library/Application/" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-io6Library-2f-Internet-2f-DNS

clean-io6Library-2f-Internet-2f-DNS:
	-$(RM) ./io6Library/Internet/DNS/dns.d ./io6Library/Internet/DNS/dns.o ./io6Library/Internet/DNS/dns.su

.PHONY: clean-io6Library-2f-Internet-2f-DNS

