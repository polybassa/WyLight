################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
gpio_if.obj: C:/ti/CC3200SDK/cc3200-sdk/example/common/gpio_if.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --float_support=fpalib --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="C:/ti/CC3200SDK/cc3200-sdk/example/httpserver/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK/cc3200-sdk/simplelink/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/simplelink/Include/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/simplelink/Source/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/driverlib" --include_path="C:/ti/CC3200SDK/cc3200-sdk/oslib/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/third_party/FreeRTOS/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/third_party/FreeRTOS/source/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/third_party/FreeRTOS/source/include" --include_path="C:/ti/CC3200SDK/cc3200-sdk/third_party/FreeRTOS/source/portable/CCS/ARM_CM3" -g --define=ccs --define=cc3200 --define=USE_FREERTOS --define=SL_PLATFORM_MULTI_THREADED --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="gpio_if.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: C:/ti/CC3200SDK/cc3200-sdk/example/httpserver/main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --float_support=fpalib --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="C:/ti/CC3200SDK/cc3200-sdk/example/httpserver/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK/cc3200-sdk/simplelink/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/simplelink/Include/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/simplelink/Source/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/driverlib" --include_path="C:/ti/CC3200SDK/cc3200-sdk/oslib/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/third_party/FreeRTOS/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/third_party/FreeRTOS/source/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/third_party/FreeRTOS/source/include" --include_path="C:/ti/CC3200SDK/cc3200-sdk/third_party/FreeRTOS/source/portable/CCS/ARM_CM3" -g --define=ccs --define=cc3200 --define=USE_FREERTOS --define=SL_PLATFORM_MULTI_THREADED --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

pinmux.obj: C:/ti/CC3200SDK/cc3200-sdk/example/httpserver/pinmux.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --float_support=fpalib --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="C:/ti/CC3200SDK/cc3200-sdk/example/httpserver/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK/cc3200-sdk/simplelink/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/simplelink/Include/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/simplelink/Source/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/driverlib" --include_path="C:/ti/CC3200SDK/cc3200-sdk/oslib/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/third_party/FreeRTOS/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/third_party/FreeRTOS/source/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/third_party/FreeRTOS/source/include" --include_path="C:/ti/CC3200SDK/cc3200-sdk/third_party/FreeRTOS/source/portable/CCS/ARM_CM3" -g --define=ccs --define=cc3200 --define=USE_FREERTOS --define=SL_PLATFORM_MULTI_THREADED --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="pinmux.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

smartconfig.obj: C:/ti/CC3200SDK/cc3200-sdk/example/httpserver/smartconfig.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --float_support=fpalib --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="C:/ti/CC3200SDK/cc3200-sdk/example/httpserver/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK/cc3200-sdk/simplelink/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/simplelink/Include/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/simplelink/Source/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/driverlib" --include_path="C:/ti/CC3200SDK/cc3200-sdk/oslib/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/third_party/FreeRTOS/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/third_party/FreeRTOS/source/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/third_party/FreeRTOS/source/include" --include_path="C:/ti/CC3200SDK/cc3200-sdk/third_party/FreeRTOS/source/portable/CCS/ARM_CM3" -g --define=ccs --define=cc3200 --define=USE_FREERTOS --define=SL_PLATFORM_MULTI_THREADED --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="smartconfig.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

startup_ccs.obj: C:/ti/CC3200SDK/cc3200-sdk/example/common/startup_ccs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --float_support=fpalib --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="C:/ti/CC3200SDK/cc3200-sdk/example/httpserver/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK/cc3200-sdk/simplelink/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/simplelink/Include/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/simplelink/Source/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/driverlib" --include_path="C:/ti/CC3200SDK/cc3200-sdk/oslib/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/third_party/FreeRTOS/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/third_party/FreeRTOS/source/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/third_party/FreeRTOS/source/include" --include_path="C:/ti/CC3200SDK/cc3200-sdk/third_party/FreeRTOS/source/portable/CCS/ARM_CM3" -g --define=ccs --define=cc3200 --define=USE_FREERTOS --define=SL_PLATFORM_MULTI_THREADED --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="startup_ccs.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

uart_if.obj: C:/ti/CC3200SDK/cc3200-sdk/example/common/uart_if.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --float_support=fpalib --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="C:/ti/CC3200SDK/cc3200-sdk/example/httpserver/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK/cc3200-sdk/simplelink/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/simplelink/Include/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/simplelink/Source/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/driverlib" --include_path="C:/ti/CC3200SDK/cc3200-sdk/oslib/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/third_party/FreeRTOS/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/third_party/FreeRTOS/source/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/third_party/FreeRTOS/source/include" --include_path="C:/ti/CC3200SDK/cc3200-sdk/third_party/FreeRTOS/source/portable/CCS/ARM_CM3" -g --define=ccs --define=cc3200 --define=USE_FREERTOS --define=SL_PLATFORM_MULTI_THREADED --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="uart_if.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


