#******************************************************************************
#
# Makefile - Rules for building the WyLight bootloader.
#
#
#*****************************************************************************


PRJ_NAME=WyLight_bl

# The base directory.
ROOT=.

# Include the common make definitions.

ifneq ($(MAKECMDGOALS),sim)
PREFIX=arm-none-eabi-
endif
include makedefs

# Additional Compiler Flags
sim: CFLAGS+=-DSIMULATOR
common: CFLAGS+=-mthumb
common: CFLAGS+=-mcpu=cortex-m4

common: LDFLAGS+=--gc-sections
sim: LDFLAGS+=-lcrypto

# Where to find header files that do not live in the source directory.
IPATH=${ROOT}
IPATH+=${ROOT}/bl
IPATH+=${ROOT}/common
IPATH+=${ROOT}/inc
IPATH+=${ROOT}/driverlib
IPATH+=${ROOT}/simplelink
IPATH+=${ROOT}/simplelink/source
IPATH+=${ROOT}/simplelink/include
sim: IPATH+=${ROOT}/sim

# The default rule, which causes the bootloader be built as bootloader
all: bl

# build all the bootloader files. Used by as_bl and as_fw builds
common: ${OBJDIR} ${BINDIR}
common: ${BINDIR}/${PRJ_NAME}.axf

# build the bootloader as normal bootloader
BL_FILE=exe/${PRJ_NAME}.bin
bl: SCATTERgcc_${PRJ_NAME}=${ROOT}/bl/WyLight_bl.ld
bl: common

#openssl sha256 -binary ${BL_FILE} >> ${BL_FILE}

# build the bootloader as a small firmware which is used to update the bootloader ;-)
FW_FILE=WyLight_fw.bin
as_fw: SCATTERgcc_${PRJ_NAME}=${ROOT}/fw/gcc/WyLight_fw.ld
as_fw: CFLAGS += -DBUILD_AS_FW
as_fw: common
	mv exe/${PRJ_NAME}.bin ${FW_FILE}

sim: ${OBJDIR} ${BINDIR} link

link: ${OBJDIR}/main.o ${OBJDIR}/pinmux.o ${OBJDIR}/firmware_loader.o ${OBJDIR}/tcp_server.o ${OBJDIR}/simulator_bl.o ${OBJDIR}/fs.o ${OBJDIR}/uart_if.o ${OBJDIR}/gpio_if.o
	${CC} ${LDFLAGS} -o simulator.bin ${OBJDIR}/main.o ${OBJDIR}/pinmux.o ${OBJDIR}/firmware_loader.o ${OBJDIR}/tcp_server.o ${OBJDIR}/simulator_bl.o ${OBJDIR}/fs.o ${OBJDIR}/uart_if.o ${OBJDIR}/gpio_if.o

install_bl:
	sh ../install.sh ${BL_FILE} "/sys/mcuimg.bin"

install_fw:
	sh ../install.sh ${FW_FILE} "/temp/firmware.bin"

# The rule to clean out all the build products.
clean:
	@rm -rf ${OBJDIR} ${wildcard *~}
	@rm -rf ${BINDIR} ${wildcard *~}
	@rm -f ${FW_FILE}

# The rule to create the target directories.
${OBJDIR}:
	@mkdir -p ${OBJDIR}

${BINDIR}:
	@mkdir -p ${BINDIR}

ifeq ($(MAKECMDGOALS),sim)
include depend.bl_sim
else
include depend.bl
endif

# select linker script
ENTRY_${PRJ_NAME}=ResetISR
CFLAGSgcc=-DTARGET_IS_BLIZZARD_RB1

# Include the automatically generated dependency files.
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
