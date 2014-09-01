#******************************************************************************
#
# Makefile - Rules for building the websocket application.
#
#
#*****************************************************************************

# configure target output
PRJ_NAME=WyLight_fw

#
# The base directory.
#
ROOT=../..
RTOS_SOURCE_DIR=${ROOT}/third_party/FreeRTOS

#
# Include the common make definitions.
#
include makedefs

#
# Where to find source files that do not live in this directory.
#
VPATH=${ROOT}/fw
VPATH+=${ROOT}/common
VPATH+=${ROOT}/driverlib
VPATH+=${ROOT}/simplelink/source
VPATH+=${ROOT}/simplelink
VPATH+=${RTOS_SOURCE_DIR}
VPATH+=${RTOS_SOURCE_DIR}/source
VPATH+=${RTOS_SOURCE_DIR}/source/portable/MemMang
VPATH+=${RTOS_SOURCE_DIR}/source/portable/GCC/ARM_CM4
VPATH+=${ROOT}/oslib
#
# Additional Compiler Flags
#
CFLAGS+=-DSL_PLATFORM_MULTI_THREADED
CFLAGS+=-DUSE_FREERTOS
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
IPATH+=${ROOT}/oslib
IPATH+=${ROOT}/simplelink
IPATH+=${ROOT}/simplelink/source
IPATH+=${ROOT}/simplelink/include
IPATH+=${RTOS_SOURCE_DIR}
IPATH+=${RTOS_SOURCE_DIR}/source
IPATH+=${RTOS_SOURCE_DIR}/source/include
IPATH+=${RTOS_SOURCE_DIR}/source/portable/MemMang
IPATH+=${RTOS_SOURCE_DIR}/source/portable/GCC/ARM_CM4

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

test:
	@echo ${LIBC}
	@echo ${LIBGCC}
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
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/broadcast.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/server.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/wy_network_if.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/wifi.o
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
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/utils.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/shamd5.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/udma.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/spi.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/gpio.o

# simplelink
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/socket.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/nonos.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/fs.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/device.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/wlan.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/driver.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/netcfg.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/cc_pal.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/flowcont.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/netapp.o

#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/tmp006drv.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/bma222drv.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/network_if.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/timer_if.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/gpio_if.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/uart_if.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/udma_if.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/wlan.o
#${BINDIR}/${PRJ_NAME}.axf: ${ROOT}/simplelink/${COMPILER}/${BINDIR}/simplelink_nonos.a
#${BINDIR}/${PRJ_NAME}.axf: ${ROOT}/driverlib/${COMPILER}/${BINDIR}/libdriver.a

# freeRTOS
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/croutine.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/list.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/queue.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/tasks.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/port.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/timers.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/heap_3.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/osi_freertos.o

# select linker script
SCATTERgcc_${PRJ_NAME}=${PRJ_NAME}.ld
ENTRY_${PRJ_NAME}=ResetISR
CFLAGSgcc=-DTARGET_IS_BLIZZARD_RB1

#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
