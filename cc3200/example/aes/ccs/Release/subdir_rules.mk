################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
aes_userinput.obj: C:/ti/CC3200SDK/cc3200-sdk/example/aes/aes_userinput.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me -Ooff --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="C:/ti/CC3200SDK/cc3200-sdk/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK/cc3200-sdk/driverlib" --include_path="C:/ti/CC3200SDK/cc3200-sdk/example/aes/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/inc" -g --gcc --define=ccs --define=NON_NETWORK --define=cc3200 --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --preproc_with_compile --preproc_dependency="aes_userinput.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: C:/ti/CC3200SDK/cc3200-sdk/example/aes/main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me -Ooff --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="C:/ti/CC3200SDK/cc3200-sdk/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK/cc3200-sdk/driverlib" --include_path="C:/ti/CC3200SDK/cc3200-sdk/example/aes/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/inc" -g --gcc --define=ccs --define=NON_NETWORK --define=cc3200 --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

pinmux.obj: C:/ti/CC3200SDK/cc3200-sdk/example/aes/pinmux.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me -Ooff --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="C:/ti/CC3200SDK/cc3200-sdk/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK/cc3200-sdk/driverlib" --include_path="C:/ti/CC3200SDK/cc3200-sdk/example/aes/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/inc" -g --gcc --define=ccs --define=NON_NETWORK --define=cc3200 --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --preproc_with_compile --preproc_dependency="pinmux.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

startup_ccs.obj: C:/ti/CC3200SDK/cc3200-sdk/example/common/startup_ccs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me -Ooff --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="C:/ti/CC3200SDK/cc3200-sdk/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK/cc3200-sdk/driverlib" --include_path="C:/ti/CC3200SDK/cc3200-sdk/example/aes/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/inc" -g --gcc --define=ccs --define=NON_NETWORK --define=cc3200 --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --preproc_with_compile --preproc_dependency="startup_ccs.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

uart_if.obj: C:/ti/CC3200SDK/cc3200-sdk/example/common/uart_if.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me -Ooff --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="C:/ti/CC3200SDK/cc3200-sdk/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK/cc3200-sdk/driverlib" --include_path="C:/ti/CC3200SDK/cc3200-sdk/example/aes/" --include_path="C:/ti/CC3200SDK/cc3200-sdk/inc" -g --gcc --define=ccs --define=NON_NETWORK --define=cc3200 --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --preproc_with_compile --preproc_dependency="uart_if.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


