#******************************************************************************
#
# Makefile - Rules for building the simulator of the WyLight bootloader.
#
#
#*****************************************************************************

# configure target output
PRJ_NAME=WyLight_ut

# The base directory.
ROOT=..

# Include the common make definitions.
include makedefs

# Where to find source files that do not live in this directory.
VPATH=${ROOT}/sim
VPATH+=${ROOT}/common

# Additional Compiler Flags
CFLAGS+=-pedantic
CFLAGS+=-Wall
CFLAGS+=-std=c11
CFLAGS+=-DSIMULATOR

# Where to find header files that do not live in the source directory.
IPATH=${ROOT}
IPATH+=${ROOT}/common
IPATH+=${ROOT}/sim
IPATH+=${ROOT}/simplelink
IPATH+=${ROOT}/simplelink/source
IPATH+=${ROOT}/simplelink/include
IPATH+=${ROOT}/../firmware
IPATH+=${ROOT}/inc
IPATH+=${ROOT}/driverlib
# The default rule, which causes the driver library to be built.
all: ${OBJDIR} ${BINDIR}
all: ${BINDIR}/${PRJ_NAME}.axf link

link:
	${CC} ${LDFLAGS} -o ${PRJ_NAME}.bin ${OBJDIR}/wy_fs_ut.o ${OBJDIR}/fs.o ${OBJDIR}/wy_fs.o ${OBJDIR}/uart_if.o

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
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/wy_fs_ut.o

# common
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/fs.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/wy_fs.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/uart_if.o

# Include the automatically generated dependency files.
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
