################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
broadcast.obj: F:/Wifly_Light/Software/cc3200/wyLightFirmware/firmwareRTOS/broadcast.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="F:/Wifly_Light/Software/cc3200/wyLightFirmware/firmwareRTOS/" --include_path="F:/Wifly_Light/Software/cc3200/wyLightFirmware/common/" --include_path="F:/Wifly_Light/Software/cc3200/inc" --include_path="F:/Wifly_Light/Software/cc3200/driverlib" --include_path="F:/Wifly_Light/Software/cc3200/oslib" --include_path="F:/Wifly_Light/Software/cc3200/third_party/FreeRTOS/source" --include_path="F:/Wifly_Light/Software/cc3200/third_party/FreeRTOS/source/include" --include_path="F:/Wifly_Light/Software/cc3200/third_party/FreeRTOS/source/portable/CCS/ARM_CM3" --include_path="F:/Wifly_Light/Software/cc3200/simplelink" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/include" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/source" -g --gcc --define=__CCS__ --define=ccs --define=USE_FREERTOS --define=SL_PLATFORM_MULTI_THREADED --define=PLATFORM_LP --define=cc3200 --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="broadcast.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: F:/Wifly_Light/Software/cc3200/wyLightFirmware/firmwareRTOS/main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="F:/Wifly_Light/Software/cc3200/wyLightFirmware/firmwareRTOS/" --include_path="F:/Wifly_Light/Software/cc3200/wyLightFirmware/common/" --include_path="F:/Wifly_Light/Software/cc3200/inc" --include_path="F:/Wifly_Light/Software/cc3200/driverlib" --include_path="F:/Wifly_Light/Software/cc3200/oslib" --include_path="F:/Wifly_Light/Software/cc3200/third_party/FreeRTOS/source" --include_path="F:/Wifly_Light/Software/cc3200/third_party/FreeRTOS/source/include" --include_path="F:/Wifly_Light/Software/cc3200/third_party/FreeRTOS/source/portable/CCS/ARM_CM3" --include_path="F:/Wifly_Light/Software/cc3200/simplelink" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/include" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/source" -g --gcc --define=__CCS__ --define=ccs --define=USE_FREERTOS --define=SL_PLATFORM_MULTI_THREADED --define=PLATFORM_LP --define=cc3200 --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

network_if.obj: F:/Wifly_Light/Software/cc3200/wyLightFirmware/common/network_if.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="F:/Wifly_Light/Software/cc3200/wyLightFirmware/firmwareRTOS/" --include_path="F:/Wifly_Light/Software/cc3200/wyLightFirmware/common/" --include_path="F:/Wifly_Light/Software/cc3200/inc" --include_path="F:/Wifly_Light/Software/cc3200/driverlib" --include_path="F:/Wifly_Light/Software/cc3200/oslib" --include_path="F:/Wifly_Light/Software/cc3200/third_party/FreeRTOS/source" --include_path="F:/Wifly_Light/Software/cc3200/third_party/FreeRTOS/source/include" --include_path="F:/Wifly_Light/Software/cc3200/third_party/FreeRTOS/source/portable/CCS/ARM_CM3" --include_path="F:/Wifly_Light/Software/cc3200/simplelink" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/include" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/source" -g --gcc --define=__CCS__ --define=ccs --define=USE_FREERTOS --define=SL_PLATFORM_MULTI_THREADED --define=PLATFORM_LP --define=cc3200 --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="network_if.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

pinmux.obj: F:/Wifly_Light/Software/cc3200/wyLightFirmware/firmwareRTOS/pinmux.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="F:/Wifly_Light/Software/cc3200/wyLightFirmware/firmwareRTOS/" --include_path="F:/Wifly_Light/Software/cc3200/wyLightFirmware/common/" --include_path="F:/Wifly_Light/Software/cc3200/inc" --include_path="F:/Wifly_Light/Software/cc3200/driverlib" --include_path="F:/Wifly_Light/Software/cc3200/oslib" --include_path="F:/Wifly_Light/Software/cc3200/third_party/FreeRTOS/source" --include_path="F:/Wifly_Light/Software/cc3200/third_party/FreeRTOS/source/include" --include_path="F:/Wifly_Light/Software/cc3200/third_party/FreeRTOS/source/portable/CCS/ARM_CM3" --include_path="F:/Wifly_Light/Software/cc3200/simplelink" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/include" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/source" -g --gcc --define=__CCS__ --define=ccs --define=USE_FREERTOS --define=SL_PLATFORM_MULTI_THREADED --define=PLATFORM_LP --define=cc3200 --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="pinmux.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

server.obj: F:/Wifly_Light/Software/cc3200/wyLightFirmware/firmwareRTOS/server.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="F:/Wifly_Light/Software/cc3200/wyLightFirmware/firmwareRTOS/" --include_path="F:/Wifly_Light/Software/cc3200/wyLightFirmware/common/" --include_path="F:/Wifly_Light/Software/cc3200/inc" --include_path="F:/Wifly_Light/Software/cc3200/driverlib" --include_path="F:/Wifly_Light/Software/cc3200/oslib" --include_path="F:/Wifly_Light/Software/cc3200/third_party/FreeRTOS/source" --include_path="F:/Wifly_Light/Software/cc3200/third_party/FreeRTOS/source/include" --include_path="F:/Wifly_Light/Software/cc3200/third_party/FreeRTOS/source/portable/CCS/ARM_CM3" --include_path="F:/Wifly_Light/Software/cc3200/simplelink" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/include" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/source" -g --gcc --define=__CCS__ --define=ccs --define=USE_FREERTOS --define=SL_PLATFORM_MULTI_THREADED --define=PLATFORM_LP --define=cc3200 --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="server.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

startup_ccs.obj: F:/Wifly_Light/Software/cc3200/wyLightFirmware/common/startup_ccs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="F:/Wifly_Light/Software/cc3200/wyLightFirmware/firmwareRTOS/" --include_path="F:/Wifly_Light/Software/cc3200/wyLightFirmware/common/" --include_path="F:/Wifly_Light/Software/cc3200/inc" --include_path="F:/Wifly_Light/Software/cc3200/driverlib" --include_path="F:/Wifly_Light/Software/cc3200/oslib" --include_path="F:/Wifly_Light/Software/cc3200/third_party/FreeRTOS/source" --include_path="F:/Wifly_Light/Software/cc3200/third_party/FreeRTOS/source/include" --include_path="F:/Wifly_Light/Software/cc3200/third_party/FreeRTOS/source/portable/CCS/ARM_CM3" --include_path="F:/Wifly_Light/Software/cc3200/simplelink" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/include" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/source" -g --gcc --define=__CCS__ --define=ccs --define=USE_FREERTOS --define=SL_PLATFORM_MULTI_THREADED --define=PLATFORM_LP --define=cc3200 --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="startup_ccs.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

uart_if.obj: F:/Wifly_Light/Software/cc3200/wyLightFirmware/common/uart_if.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="F:/Wifly_Light/Software/cc3200/wyLightFirmware/firmwareRTOS/" --include_path="F:/Wifly_Light/Software/cc3200/wyLightFirmware/common/" --include_path="F:/Wifly_Light/Software/cc3200/inc" --include_path="F:/Wifly_Light/Software/cc3200/driverlib" --include_path="F:/Wifly_Light/Software/cc3200/oslib" --include_path="F:/Wifly_Light/Software/cc3200/third_party/FreeRTOS/source" --include_path="F:/Wifly_Light/Software/cc3200/third_party/FreeRTOS/source/include" --include_path="F:/Wifly_Light/Software/cc3200/third_party/FreeRTOS/source/portable/CCS/ARM_CM3" --include_path="F:/Wifly_Light/Software/cc3200/simplelink" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/include" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/source" -g --gcc --define=__CCS__ --define=ccs --define=USE_FREERTOS --define=SL_PLATFORM_MULTI_THREADED --define=PLATFORM_LP --define=cc3200 --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="uart_if.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

wifi.obj: F:/Wifly_Light/Software/cc3200/wyLightFirmware/firmwareRTOS/wifi.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.7/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.7/include" --include_path="F:/Wifly_Light/Software/cc3200/wyLightFirmware/firmwareRTOS/" --include_path="F:/Wifly_Light/Software/cc3200/wyLightFirmware/common/" --include_path="F:/Wifly_Light/Software/cc3200/inc" --include_path="F:/Wifly_Light/Software/cc3200/driverlib" --include_path="F:/Wifly_Light/Software/cc3200/oslib" --include_path="F:/Wifly_Light/Software/cc3200/third_party/FreeRTOS/source" --include_path="F:/Wifly_Light/Software/cc3200/third_party/FreeRTOS/source/include" --include_path="F:/Wifly_Light/Software/cc3200/third_party/FreeRTOS/source/portable/CCS/ARM_CM3" --include_path="F:/Wifly_Light/Software/cc3200/simplelink" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/include" --include_path="F:/Wifly_Light/Software/cc3200/simplelink/source" -g --gcc --define=__CCS__ --define=ccs --define=USE_FREERTOS --define=SL_PLATFORM_MULTI_THREADED --define=PLATFORM_LP --define=cc3200 --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="wifi.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


