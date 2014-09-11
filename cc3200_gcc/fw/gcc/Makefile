#*****************************************************************************
#
# Makefile - Rules for building the WyLight firmware.
#
#
#*****************************************************************************

# configure target output
PRJ_NAME=WyLight_fw

# The base directory.
ROOT=../..
RTOS_SOURCE_DIR=${ROOT}/third_party/FreeRTOS

# Include the common make definitions.
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
VPATH+=${ROOT}/../firmware

vpath timer.c ${ROOT}/driverlib
vpath crc.c ${ROOT}/driverlib
# Additional Compiler Flags
CFLAGS+=-DSL_PLATFORM_MULTI_THREADED
CFLAGS+=-DUSE_FREERTOS
CFLAGS+=-pedantic
CFLAGS+=-Wall
CFLAGS+=-Dcc3200

# Where to find header files that do not live in the source directory.
IPATH=${ROOT}
IPATH+=${ROOT}/fw
IPATH+=${ROOT}/common
IPATH+=${ROOT}/inc
IPATH+=${ROOT}/driverlib
IPATH+=${ROOT}/oslib
IPATH+=${ROOT}/simplelink
IPATH+=${ROOT}/simplelink/source
IPATH+=${ROOT}/simplelink/include
IPATH+=${RTOS_SOURCE_DIR}
IPATH+=${RTOS_SOURCE_DIR}/source
IPATH+=${RTOS_SOURCE_DIR}/source/include
IPATH+=${RTOS_SOURCE_DIR}/source/portable/MemMang
IPATH+=${RTOS_SOURCE_DIR}/source/portable/GCC/ARM_CM4
IPATH+=${ROOT}/../firmware

# The default rule, which causes the driver library to be built.
all: ${OBJDIR} ${BINDIR}
all: ${BINDIR}/${PRJ_NAME}.axf 

# The rule to clean out all the build products.
clean:
	@rm -rf ${OBJDIR} ${wildcard *~}
	@rm -rf ${BINDIR} ${wildcard *~}

test:
	@echo ${LIBC}
	@echo ${LIBGCC}

checksum: all
	openssl SHA256 -binary ${BINDIR}/${PRJ_NAME}.bin >> ${BINDIR}/${PRJ_NAME}.bin

install: checksum
	netcat 192.168.1.1 2000 < ${BINDIR}/${PRJ_NAME}.bin

# The rule to create the target directories.
${OBJDIR}:
	@mkdir -p ${OBJDIR}

${BINDIR}:
	@mkdir -p ${BINDIR}

# Rules for building this example.
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/main.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/pinmux.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/broadcast.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/server.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/wy_network_if.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/wifi.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/pwm.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/sniffer.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/RingBuf.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/crc.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/CommandIO.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/wy_firmware.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/Version.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/eeprom.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/ScriptCtrl.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/ledstrip.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/wy_crc.o
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
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/timer.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/udma.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/spi.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/gpio.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/crc.o

# simplelink
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/socket.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/fs.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/device.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/wlan.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/driver.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/netcfg.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/cc_pal.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/flowcont.o
${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/netapp.o

#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/tmp006drv.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/bma222drv.o

${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/timer_if.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/gpio_if.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/uart_if.o
#${BINDIR}/${PRJ_NAME}.axf: ${OBJDIR}/udma_if.o

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

# Include the automatically generated dependency files.
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
