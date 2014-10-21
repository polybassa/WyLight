#******************************************************************************
#
# Makefile - Rules for building the WyLight bootloader.
#
#
#*****************************************************************************

PRJ_NAME=WyLight_$(MAKECMDGOALS)
FLAVOUR=$(MAKECMDGOALS)

ifneq ($(MAKECMDGOALS),sim)
PREFIX=arm-none-eabi-
endif

# Include the common make definitions.
include makedefs

# Additional Compiler Flags
sim: CFLAGS+=-DSIMULATOR

common: CFLAGS+=-mthumb
common: CFLAGS+=-mcpu=cortex-m4
common: CFLAGS+=-DTARGET_IS_BLIZZARD_RB1

# additional linker flags
sim: LDFLAGS+=-lcrypto
common: LDFLAGS+=--gc-sections

# The default rule, which causes the bootloader be built as bootloader
all: bl

# build all the bootloader files. Used by bl and fw builds
common: ${OBJDIR} ${BINDIR}
common: ${BINDIR}/${PRJ_NAME}.axf

# build the bootloader as normal bootloader
BL_FILE=exe/${PRJ_NAME}.bin
bl: common

#openssl sha256 -binary ${BL_FILE} >> ${BL_FILE}

# build the bootloader as a small firmware which is used to update the bootloader ;-)
FW_FILE=WyLight_fw.bin
fw: CFLAGS += -DBUILD_AS_FW
fw: common
	mv exe/${PRJ_NAME}.bin ${FW_FILE}

sim: ${OBJDIR} ${BINDIR} ${OBJDIR}/main.o ${OBJDIR}/pinmux.o ${OBJDIR}/firmware_loader.o ${OBJDIR}/tcp_server.o ${OBJDIR}/simulator_bl.o ${OBJDIR}/fs.o ${OBJDIR}/uart_if.o ${OBJDIR}/gpio_if.o
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

# Include the automatically generated dependency files.
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
