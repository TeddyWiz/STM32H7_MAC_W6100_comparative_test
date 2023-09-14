################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Utilities/3rdParty/MISC/core_list_join.c \
../Utilities/3rdParty/MISC/core_main.c \
../Utilities/3rdParty/MISC/core_matrix.c \
../Utilities/3rdParty/MISC/core_portme.c \
../Utilities/3rdParty/MISC/core_state.c \
../Utilities/3rdParty/MISC/core_util.c 

OBJS += \
./Utilities/3rdParty/MISC/core_list_join.o \
./Utilities/3rdParty/MISC/core_main.o \
./Utilities/3rdParty/MISC/core_matrix.o \
./Utilities/3rdParty/MISC/core_portme.o \
./Utilities/3rdParty/MISC/core_state.o \
./Utilities/3rdParty/MISC/core_util.o 

C_DEPS += \
./Utilities/3rdParty/MISC/core_list_join.d \
./Utilities/3rdParty/MISC/core_main.d \
./Utilities/3rdParty/MISC/core_matrix.d \
./Utilities/3rdParty/MISC/core_portme.d \
./Utilities/3rdParty/MISC/core_state.d \
./Utilities/3rdParty/MISC/core_util.d 


# Each subdirectory must supply rules for building sources it contributes
Utilities/3rdParty/MISC/%.o Utilities/3rdParty/MISC/%.su: ../Utilities/3rdParty/MISC/%.c Utilities/3rdParty/MISC/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../CoreMarkApp/App -I../CoreMarkApp -I../Utilities/3rdParty/MISC -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"D:/wiznet/05_WorkSpace/04_w6100/03_STMh723_W6100_Test/Nucleo-H723_CoreMark_test/io6Library/Ethernet/W6100" -I"D:/wiznet/05_WorkSpace/04_w6100/03_STMh723_W6100_Test/Nucleo-H723_CoreMark_test/io6Library/Ethernet/" -I"D:/wiznet/05_WorkSpace/04_w6100/03_STMh723_W6100_Test/Nucleo-H723_CoreMark_test/io6Library/Application/loopback" -I"D:/wiznet/05_WorkSpace/04_w6100/03_STMh723_W6100_Test/Nucleo-H723_CoreMark_test/io6Library/Application/" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Utilities-2f-3rdParty-2f-MISC

clean-Utilities-2f-3rdParty-2f-MISC:
	-$(RM) ./Utilities/3rdParty/MISC/core_list_join.d ./Utilities/3rdParty/MISC/core_list_join.o ./Utilities/3rdParty/MISC/core_list_join.su ./Utilities/3rdParty/MISC/core_main.d ./Utilities/3rdParty/MISC/core_main.o ./Utilities/3rdParty/MISC/core_main.su ./Utilities/3rdParty/MISC/core_matrix.d ./Utilities/3rdParty/MISC/core_matrix.o ./Utilities/3rdParty/MISC/core_matrix.su ./Utilities/3rdParty/MISC/core_portme.d ./Utilities/3rdParty/MISC/core_portme.o ./Utilities/3rdParty/MISC/core_portme.su ./Utilities/3rdParty/MISC/core_state.d ./Utilities/3rdParty/MISC/core_state.o ./Utilities/3rdParty/MISC/core_state.su ./Utilities/3rdParty/MISC/core_util.d ./Utilities/3rdParty/MISC/core_util.o ./Utilities/3rdParty/MISC/core_util.su

.PHONY: clean-Utilities-2f-3rdParty-2f-MISC
