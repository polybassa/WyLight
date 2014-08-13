################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
gpio_if.obj: F:/Wifly_Light/Software/cc3200/example/common/gpio_if.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="F:/Wifly_Light/Software/cc3200/driverlib/" --include_path="F:/Wifly_Light/Software/cc3200/example/timer/" --include_path="F:/Wifly_Light/Software/cc3200/example/common" --include_path="F:/Wifly_Light/Software/cc3200/inc/" -g --gcc --define=ccs --define=NON_NETWORK --define=cc3200 --display_error_number --diag_warning=225 --printf_support=full --preproc_with_compile --preproc_dependency="gpio_if.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: F:/Wifly_Light/Software/cc3200/example/timer/main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="F:/Wifly_Light/Software/cc3200/driverlib/" --include_path="F:/Wifly_Light/Software/cc3200/example/timer/" --include_path="F:/Wifly_Light/Software/cc3200/example/common" --include_path="F:/Wifly_Light/Software/cc3200/inc/" -g --gcc --define=ccs --define=NON_NETWORK --define=cc3200 --display_error_number --diag_warning=225 --printf_support=full --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

pinmux.obj: F:/Wifly_Light/Software/cc3200/example/timer/pinmux.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="F:/Wifly_Light/Software/cc3200/driverlib/" --include_path="F:/Wifly_Light/Software/cc3200/example/timer/" --include_path="F:/Wifly_Light/Software/cc3200/example/common" --include_path="F:/Wifly_Light/Software/cc3200/inc/" -g --gcc --define=ccs --define=NON_NETWORK --define=cc3200 --display_error_number --diag_warning=225 --printf_support=full --preproc_with_compile --preproc_dependency="pinmux.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

startup_ccs.obj: F:/Wifly_Light/Software/cc3200/example/common/startup_ccs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="F:/Wifly_Light/Software/cc3200/driverlib/" --include_path="F:/Wifly_Light/Software/cc3200/example/timer/" --include_path="F:/Wifly_Light/Software/cc3200/example/common" --include_path="F:/Wifly_Light/Software/cc3200/inc/" -g --gcc --define=ccs --define=NON_NETWORK --define=cc3200 --display_error_number --diag_warning=225 --printf_support=full --preproc_with_compile --preproc_dependency="startup_ccs.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

timer_if.obj: F:/Wifly_Light/Software/cc3200/example/common/timer_if.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="F:/Wifly_Light/Software/cc3200/driverlib/" --include_path="F:/Wifly_Light/Software/cc3200/example/timer/" --include_path="F:/Wifly_Light/Software/cc3200/example/common" --include_path="F:/Wifly_Light/Software/cc3200/inc/" -g --gcc --define=ccs --define=NON_NETWORK --define=cc3200 --display_error_number --diag_warning=225 --printf_support=full --preproc_with_compile --preproc_dependency="timer_if.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


