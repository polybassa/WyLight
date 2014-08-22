#******************************************************************************
#
# Makefile - Rules for building the websocket application.
#
#
#*****************************************************************************

# configure target output
PRJ_NAME=WyLight_bl


#
# The base directory.
#
ROOT=../..

#
# Include the common make definitions.
#
include makedefs

#
# Where to find source files that do not live in this directory.
#
VPATH=${ROOT}/bl
VPATH+=${ROOT}/common
VPATH+=${ROOT}/driverlib
#VPATH+=../../drivers


#
# Additional Compiler Flags
#
#CFLAGS+=-DSL_PLATFORM_MULTI_THREADED
#CFLAGS+=-DUSE_FREERTOS

#CFLAGS+=-Dcc3200
CFLAGS+=-pedantic
CFLAGS+=-Wall

#
# Where to find header files that do not live in the source directory.
#
IPATH=${ROOT}
IPATH+=${ROOT}/bl
IPATH+=${ROOT}/common
IPATH+=${ROOT}/inc
IPATH+=${ROOT}/driverlib
#IPATH+=../..
#IPATH+=${ROOT}/oslib
#IPATH+=${ROOT}/simplelink
#IPATH+=${ROOT}/simplelink/source
#IPATH+=${ROOT}/simplelink/include
#IPATH+=${ROOT}/third_party/FreeRTOS
#IPATH+=${ROOT}/third_party/FreeRTOS/source
#IPATH+=${ROOT}/third_party/FreeRTOS/source/portable/GCC/ARM_CM4
#IPATH+=${ROOT}/third_party/FreeRTOS/source/include

#
# The default rule, which causes the driver library to be built.
#
all: ${OBJDIR} ${BINDIR}
all: ${BINDIR}/${PRJ_NAME}.axf

#
# The rule to clean out all the build products.
#
clean:
	@rm -rf ${OBJDIR} ${wildcard *~}
	@rm -rf ${BINDIR} ${wildcard *~}


#
# The rule to create the target directories.
#
${OBJDIR}:
	@mkdir -p ${OBJDIR}

${BINDIR}:
	@mkdir -p ${BINDIR}

#
# Rules for building this example.
#
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/main.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/pinmux.o

# common
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/startup_${COMPILER}.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/uart_if.o

# driverLib
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/cpu.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/flash.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/interrupt.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/pin.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/prcm.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/uart.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/utils.o

#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/firmware_download.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/broadcast.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/server.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/wifi.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/RingBuf.o

#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/tmp006drv.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/bma222drv.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/network_if.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/timer_if.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/gpio_if.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/uart_if.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/udma_if.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/wlan.o
#${BINDIR}/${PRJ_NAME}.axf: ${ROOT}/simplelink/${COMPILER}/${BINDIR}/simplelink.a
#${BINDIR}/${PRJ_NAME}.axf: ${ROOT}/driverlib/${COMPILER}/${BINDIR}/libdriver.a
#${BINDIR}/${PRJ_NAME}.axf: ${ROOT}/oslib/${COMPILER}/${BINDIR}/FreeRTOS.a

# select linker script
SCATTERgcc_${PRJ_NAME}=${PRJ_NAME}.ld

#ENTRY_websocket=ResetISR
#CFLAGSgcc=-DTARGET_IS_BLIZZARD_RB1

#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
