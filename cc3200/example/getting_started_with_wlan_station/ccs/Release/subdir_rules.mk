################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
gpio_if.obj: C:/ti/CC3200SDK/cc3200-sdk/example/common/gpio_if.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --float_support=fpalib --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="C:/ti/CC3200SDK/cc3200-sdk/simplelink" --include_path="C:/ti/CC3200SDK/cc3200-sdk/simplelink/include" --include_path="C:/ti/CC3200SDK/cc3200-sdk/simplelink/source" --include_path="C:/ti/CC3200SDK/cc3200-sdk/example/getting_started_with_wlan_station/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK/cc3200-sdk/oslib" --include_path="C:/ti/CC3200SDK/cc3200-sdk/driverlib" --include_path="C:/ti/CC3200SDK/cc3200-sdk/inc" -g --define=ccs --define=USE_TIRTOS --define=SL_PLATFORM_MULTI_THREADED --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="gpio_if.pp" --cmd_file="C:/ti/CC3200SDK/cc3200-sdk/ti_rtos/ti_rtos_config/Default/configPkg/compiler.opt" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: C:/ti/CC3200SDK/cc3200-sdk/example/getting_started_with_wlan_station/main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --float_support=fpalib --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="C:/ti/CC3200SDK/cc3200-sdk/simplelink" --include_path="C:/ti/CC3200SDK/cc3200-sdk/simplelink/include" --include_path="C:/ti/CC3200SDK/cc3200-sdk/simplelink/source" --include_path="C:/ti/CC3200SDK/cc3200-sdk/example/getting_started_with_wlan_station/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK/cc3200-sdk/oslib" --include_path="C:/ti/CC3200SDK/cc3200-sdk/driverlib" --include_path="C:/ti/CC3200SDK/cc3200-sdk/inc" -g --define=ccs --define=USE_TIRTOS --define=SL_PLATFORM_MULTI_THREADED --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="main.pp" --cmd_file="C:/ti/CC3200SDK/cc3200-sdk/ti_rtos/ti_rtos_config/Default/configPkg/compiler.opt" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

pinmux.obj: C:/ti/CC3200SDK/cc3200-sdk/example/getting_started_with_wlan_station/pinmux.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --float_support=fpalib --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="C:/ti/CC3200SDK/cc3200-sdk/simplelink" --include_path="C:/ti/CC3200SDK/cc3200-sdk/simplelink/include" --include_path="C:/ti/CC3200SDK/cc3200-sdk/simplelink/source" --include_path="C:/ti/CC3200SDK/cc3200-sdk/example/getting_started_with_wlan_station/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK/cc3200-sdk/oslib" --include_path="C:/ti/CC3200SDK/cc3200-sdk/driverlib" --include_path="C:/ti/CC3200SDK/cc3200-sdk/inc" -g --define=ccs --define=USE_TIRTOS --define=SL_PLATFORM_MULTI_THREADED --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="pinmux.pp" --cmd_file="C:/ti/CC3200SDK/cc3200-sdk/ti_rtos/ti_rtos_config/Default/configPkg/compiler.opt" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

uart_if.obj: C:/ti/CC3200SDK/cc3200-sdk/example/common/uart_if.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --float_support=fpalib --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="C:/ti/CC3200SDK/cc3200-sdk/simplelink" --include_path="C:/ti/CC3200SDK/cc3200-sdk/simplelink/include" --include_path="C:/ti/CC3200SDK/cc3200-sdk/simplelink/source" --include_path="C:/ti/CC3200SDK/cc3200-sdk/example/getting_started_with_wlan_station/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK/cc3200-sdk/oslib" --include_path="C:/ti/CC3200SDK/cc3200-sdk/driverlib" --include_path="C:/ti/CC3200SDK/cc3200-sdk/inc" -g --define=ccs --define=USE_TIRTOS --define=SL_PLATFORM_MULTI_THREADED --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="uart_if.pp" --cmd_file="C:/ti/CC3200SDK/cc3200-sdk/ti_rtos/ti_rtos_config/Default/configPkg/compiler.opt" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


