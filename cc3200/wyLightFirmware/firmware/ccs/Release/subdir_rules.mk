################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
firmware_download.obj: F:/Wifly_Light/Software/cc3200/wyLightFirmware/firmware/firmware_download.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="F:/Wifly_Light/Software/cc3200/driverlib/" --include_path="F:/Wifly_Light/Software/cc3200/inc/" --include_path="F:/Wifly_Light/Software/cc3200/wyLightFirmware/firmware/" --include_path="F:/Wifly_Light/Software/cc3200/wyLightFirmware/common" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/source" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/include" -g --define=cc3200 --define=ccs --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="firmware_download.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: F:/Wifly_Light/Software/cc3200/wyLightFirmware/firmware/main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="F:/Wifly_Light/Software/cc3200/driverlib/" --include_path="F:/Wifly_Light/Software/cc3200/inc/" --include_path="F:/Wifly_Light/Software/cc3200/wyLightFirmware/firmware/" --include_path="F:/Wifly_Light/Software/cc3200/wyLightFirmware/common" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/source" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/include" -g --define=cc3200 --define=ccs --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

pinmux.obj: F:/Wifly_Light/Software/cc3200/wyLightFirmware/firmware/pinmux.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="F:/Wifly_Light/Software/cc3200/driverlib/" --include_path="F:/Wifly_Light/Software/cc3200/inc/" --include_path="F:/Wifly_Light/Software/cc3200/wyLightFirmware/firmware/" --include_path="F:/Wifly_Light/Software/cc3200/wyLightFirmware/common" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/source" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/include" -g --define=cc3200 --define=ccs --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="pinmux.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

startup_ccs.obj: F:/Wifly_Light/Software/cc3200/wyLightFirmware/common/startup_ccs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="F:/Wifly_Light/Software/cc3200/driverlib/" --include_path="F:/Wifly_Light/Software/cc3200/inc/" --include_path="F:/Wifly_Light/Software/cc3200/wyLightFirmware/firmware/" --include_path="F:/Wifly_Light/Software/cc3200/wyLightFirmware/common" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/source" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/include" -g --define=cc3200 --define=ccs --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="startup_ccs.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

uart_if.obj: F:/Wifly_Light/Software/cc3200/wyLightFirmware/common/uart_if.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="F:/Wifly_Light/Software/cc3200/driverlib/" --include_path="F:/Wifly_Light/Software/cc3200/inc/" --include_path="F:/Wifly_Light/Software/cc3200/wyLightFirmware/firmware/" --include_path="F:/Wifly_Light/Software/cc3200/wyLightFirmware/common" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/source" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/include" -g --define=cc3200 --define=ccs --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="uart_if.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


