################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
gpio_if.obj: F:/Wifly_Light/Software/cc3200/example/common/gpio_if.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --float_support=fpalib --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="F:/Wifly_Light/Software/cc3200/simplelink" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/include" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/source" --include_path="F:/Wifly_Light/Software/cc3200/example/getting_started_with_wlan_station/" --include_path="F:/Wifly_Light/Software/cc3200/example/common" --include_path="F:/Wifly_Light/Software/cc3200/oslib" --include_path="F:/Wifly_Light/Software/cc3200/driverlib" --include_path="F:/Wifly_Light/Software/cc3200/inc" -g --define=ccs --define=USE_TIRTOS --define=SL_PLATFORM_MULTI_THREADED --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="gpio_if.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: F:/Wifly_Light/Software/cc3200/example/getting_started_with_wlan_station/main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --float_support=fpalib --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="F:/Wifly_Light/Software/cc3200/simplelink" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/include" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/source" --include_path="F:/Wifly_Light/Software/cc3200/example/getting_started_with_wlan_station/" --include_path="F:/Wifly_Light/Software/cc3200/example/common" --include_path="F:/Wifly_Light/Software/cc3200/oslib" --include_path="F:/Wifly_Light/Software/cc3200/driverlib" --include_path="F:/Wifly_Light/Software/cc3200/inc" -g --define=ccs --define=USE_TIRTOS --define=SL_PLATFORM_MULTI_THREADED --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

pinmux.obj: F:/Wifly_Light/Software/cc3200/example/getting_started_with_wlan_station/pinmux.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --float_support=fpalib --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="F:/Wifly_Light/Software/cc3200/simplelink" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/include" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/source" --include_path="F:/Wifly_Light/Software/cc3200/example/getting_started_with_wlan_station/" --include_path="F:/Wifly_Light/Software/cc3200/example/common" --include_path="F:/Wifly_Light/Software/cc3200/oslib" --include_path="F:/Wifly_Light/Software/cc3200/driverlib" --include_path="F:/Wifly_Light/Software/cc3200/inc" -g --define=ccs --define=USE_TIRTOS --define=SL_PLATFORM_MULTI_THREADED --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="pinmux.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

uart_if.obj: F:/Wifly_Light/Software/cc3200/example/common/uart_if.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --float_support=fpalib --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="F:/Wifly_Light/Software/cc3200/simplelink" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/include" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/source" --include_path="F:/Wifly_Light/Software/cc3200/example/getting_started_with_wlan_station/" --include_path="F:/Wifly_Light/Software/cc3200/example/common" --include_path="F:/Wifly_Light/Software/cc3200/oslib" --include_path="F:/Wifly_Light/Software/cc3200/driverlib" --include_path="F:/Wifly_Light/Software/cc3200/inc" -g --define=ccs --define=USE_TIRTOS --define=SL_PLATFORM_MULTI_THREADED --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="uart_if.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


