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
VPATH+=${ROOT}/sim
#VPATH+=${ROOT}/driverlib
#VPATH+=${ROOT}/simplelink/source
#VPATH+=${ROOT}/simplelink
VPATH+=/usr/local/Cellar/openssl/1.0.1i/lib/

# Additional Compiler Flags
CFLAGS+=-pedantic
CFLAGS+=-Wall
CFLAGS+=-std=c11
CFLAGS+=-DSIMULATOR

LDFLAGS+=-lcrypto

# Where to find header files that do not live in the source directory.
IPATH=${ROOT}
IPATH+=${ROOT}/bl
IPATH+=${ROOT}/common
IPATH+=${ROOT}/sim
IPATH+=${ROOT}/inc
IPATH+=${ROOT}/driverlib
IPATH+=${ROOT}/simplelink
IPATH+=${ROOT}/simplelink/source
IPATH+=${ROOT}/simplelink/include
IPATH+=/usr/local/Cellar/openssl/1.0.1i/include/openssl/
# The default rule, which causes the driver library to be built.
all: ${OBJDIR} ${BINDIR}
all: ${BINDIR}/${PRJ_NAME}.axf link

link:
	${CC} ${LDFLAGS} -o simulator.bin ${OBJDIR}/main.o ${OBJDIR}/pinmux.o ${OBJDIR}/firmware_loader.o ${OBJDIR}/tcp_server.o ${OBJDIR}/simulator_bl.o ${OBJDIR}/fs.o ${OBJDIR}/uart_if.o ${OBJDIR}/gpio_if.o

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
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/simulator_bl.o

# common
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/fs.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/uart_if.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/gpio_if.o	

# Include the automatically generated dependency files.
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
