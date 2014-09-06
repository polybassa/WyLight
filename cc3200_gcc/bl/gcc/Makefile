#******************************************************************************
#
# Makefile - Rules for building the WyLight bootloader.
#
#
#*****************************************************************************

# configure target output
PRJ_NAME=WyLight_bl

# The base directory.
ROOT=../..

# Include the common make definitions.
include makedefs

# Where to find source files that do not live in this directory.
VPATH=${ROOT}/bl
VPATH+=${ROOT}/common
VPATH+=${ROOT}/driverlib
VPATH+=${ROOT}/simplelink/source
VPATH+=${ROOT}/simplelink

# Additional Compiler Flags
CFLAGS+=-pedantic
CFLAGS+=-Wall
CFLAGS+=-O1

# Where to find header files that do not live in the source directory.
IPATH=${ROOT}
IPATH+=${ROOT}/bl
IPATH+=${ROOT}/common
IPATH+=${ROOT}/inc
IPATH+=${ROOT}/driverlib
IPATH+=${ROOT}/simplelink
IPATH+=${ROOT}/simplelink/source
IPATH+=${ROOT}/simplelink/include

# The default rule, which causes the bootloader be built as bootloader
all: bl

# build all the bootloader files. Used by as_bl and as_fw builds
common: clean ${OBJDIR} ${BINDIR}
common: ${BINDIR}/${PRJ_NAME}.axf

# build the bootloader as normal bootloader
# NOTE: the bootloader is not expected to exceed 56k
BL_FILE=exe/${PRJ_NAME}.bin
bl: SCATTERgcc_${PRJ_NAME}=../../bl/gcc/WyLight_bl.ld
bl: common
	openssl sha256 -binary ${BL_FILE} >> ${BL_FILE}

# build the bootloader as a small firmware which is used to update the bootloader ;-)
FW_FILE=WyLight_fw.bin
as_fw: SCATTERgcc_${PRJ_NAME}=../../fw/gcc/WyLight_fw.ld
as_fw: CFLAGS += -DFW_FILENAME='"/sys/mcuimg.bin"'
as_fw: CFLAGS += -DAPP_NAME='"WyLight Firmware"'
as_fw: CFLAGS += -DFIRMWARE_ORIGIN=0x20004000
as_fw: common
	mv exe/${PRJ_NAME}.bin ${FW_FILE}
	openssl sha256 -binary ${FW_FILE} >> ${FW_FILE}

install_bl:
	netcat 192.168.1.1 2000 < ${BL_FILE}

install_fw:
	netcat 192.168.1.1 2000 < ${FW_FILE}

# The rule to clean out all the build products.
clean:
	@rm -rf ${OBJDIR} ${wildcard *~}
	@rm -rf ${BINDIR} ${wildcard *~}

test:
	@echo ${LIBC}
	@echo ${LIBGCC}

# The rule to create the target directories.
${OBJDIR}:
	@mkdir -p ${OBJDIR}

${BINDIR}:
	@mkdir -p ${BINDIR}

# WyLight bootloader
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/main.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/pinmux.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/firmware_loader.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/tcp_server.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/wy_bl_network_if.o

# common
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/startup_${COMPILER}.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/uart_if.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/gpio_if.o	

#driverLib
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/cpu.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/flash.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/interrupt.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/pin.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/prcm.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/uart.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/utils.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/shamd5.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/udma.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/spi.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/gpio.o

# simplelink
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/socket.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/nonos.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/fs.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/device.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/wlan.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/driver.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/netcfg.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/cc_pal.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/flowcont.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/netapp.o

# select linker script
ENTRY_${PRJ_NAME}=ResetISR
CFLAGSgcc=-DTARGET_IS_BLIZZARD_RB1

# Include the automatically generated dependency files.
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
