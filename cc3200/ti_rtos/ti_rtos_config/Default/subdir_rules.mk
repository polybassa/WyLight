################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
configPkg/: C:/ti/CC3200SDK/cc3200-sdk/ti_rtos/ti_rtos_config/app.cfg
	@echo 'Building file: $<'
	@echo 'Invoking: XDCtools'
	"c:/ti/xdctools_3_30_03_47_core/xs" --xdcpath="c:/ti/ccsv6/ccs_base;c:/ti/tirtos_simplelink_2_00_02_36/packages;c:/ti/tirtos_simplelink_2_00_02_36/products/bios_6_40_02_27/packages;c:/ti/tirtos_simplelink_2_00_02_36/products/uia_2_00_01_34/packages;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M4 -p ti.platforms.simplelink:CC3200 -r release -c "c:/ti/ccsv6/tools/compiler/arm_5.1.7" "$<"
	@echo 'Finished building: $<'
	@echo ' '


