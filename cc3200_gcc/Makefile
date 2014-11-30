#******************************************************************************
# Copyright (C) 2014 Nils Weiss, Patrick Bruenn.
#
# This file is part of WyLight.
#
# WyLight is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# WyLight is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with WyLight.  If not, see <http://www.gnu.org/licenses/>.

#******************************************************************************

#******************************************************************************
#
# Makefile - Rules for building the WyLight bootloader.
#
#
#*****************************************************************************
PRJ_PREFIX=WyLight_
PRJ_NAME=${PRJ_PREFIX}$(MAKECMDGOALS)
FLAVOUR=$(MAKECMDGOALS)

# linker script
SCATTERgcc_${PRJ_NAME}=${ROOT}/${FLAVOUR}/${PRJ_NAME}.ld
ENTRY_${PRJ_NAME}=ResetISR

ifeq ($(MAKECMDGOALS),bl)
PREFIX=arm-none-eabi-
endif

ifeq ($(MAKECMDGOALS),fw) 
PREFIX=arm-none-eabi-
endif

ifeq ($(MAKECMDGOALS),te)
PREFIX=arm-none-eabi-
endif

# Include the common make definitions.
include makedefs

# Additional Compiler Flags
sim: CFLAGS+=-DSIMULATOR

common: CFLAGS+=-DTARGET_IS_BLIZZARD_RB1

fw: CFLAGS+=-DSL_PLATFORM_MULTI_THREADED
fw: CFLAGS+=-DUSE_FREERTOS
fw: CFLAGS+=-Dcc3200

ut: CFLAGS+=-DSIMULATOR
ut: CFLAGS+=-std=c11
ut: CFLAGS+=-g

bl: CFLAGS+=-DNOTERM
bl: CFLAGS+=-Dcc3200

# additional linker flags
sim: LDFLAGS+=-lcrypto

# The default rule, which causes the bootloader be built as bootloader
all: bl

pre-build: clean

post-build: 
	openssl sha256 -binary ${BINDIR}/${PRJ_NAME}.bin > ${BINDIR}/${PRJ_NAME}.sha
# build all the bootloader files. Used by bl and fw builds
common: ${OBJDIR} ${BINDIR}
common: ${BINDIR}/${PRJ_NAME}.axf

# build the bootloader as normal bootloader 
fw: pre-build common post-build

bl: pre-build common post-build

te: pre-build common post-build

SIM_FILES=${OBJDIR}/main.o ${OBJDIR}/wy_fs.o ${OBJDIR}/pinmux.o ${OBJDIR}/firmware_loader.o ${OBJDIR}/tcp_server.o ${OBJDIR}/simulator_bl.o ${OBJDIR}/fs.o ${OBJDIR}/uart_if.o ${OBJDIR}/gpio_if.o
sim: pre-build ${OBJDIR} ${BINDIR} ${SIM_FILES}
	${CC} ${LDFLAGS} -o ${BINDIR}/${PRJ_NAME}.bin ${SIM_FILES} 

UT_FILES=${OBJDIR}/wy_fs_ut.o ${OBJDIR}/fs.o ${OBJDIR}/wy_fs.o ${OBJDIR}/uart_if.o
ut: pre-build ${OBJDIR} ${BINDIR} ${UT_FILES}
	${CC} ${LDFLAGS} -o ${BINDIR}/${PRJ_NAME}.bin ${UT_FILES}


BL_FILE=${OBJDIR}/${PRJ_PREFIX}bl.bin
FW_FILE=${OBJDIR}/${PRJ_PREFIX}fw.bin
install_bl:
	sh install.sh ${BL_FILE} "/sys/mcuimg.bin"

install_fw:
	sh install.sh ${FW_FILE} "/temp/firmware.bin"

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


# Include the automatically generated dependency files.
ifneq (${MAKECMDGOALS},clean)
include ${FLAVOUR}/Makefile.${FLAVOUR}
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
