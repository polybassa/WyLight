//*****************************************************************************
//
//  interrupt.c
//
//  Driver for the NVIC Interrupt Controller.
//
//  Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/
//
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//
//    Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
//    Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the
//    distribution.
//
//    Neither the name of Texas Instruments Incorporated nor the names of
//    its contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup interrupt_api
//! @{
//
//*****************************************************************************

#include "interrupt.h"
#include <stdio.h>

bool
IntMasterEnable(void)
{
    printf("Enable processor interrupts.\r\n");
    return true;
}

//*****************************************************************************
//
//! Disables the processor interrupt.
//!
//! Prevents the processor from receiving interrupts.  This does not affect the
//! set of interrupts enabled in the interrupt controller; it just gates the
//! single interrupt from the controller to the processor.
//!
//! \note Previously, this function had no return value.  As such, it was
//! possible to include <tt>interrupt.h</tt> and call this function without
//! having included <tt>hw_types.h</tt>.  Now that the return is a
//! <tt>tBoolean</tt>, a compiler error will occur in this case.  The solution
//! is to include <tt>hw_types.h</tt> before including <tt>interrupt.h</tt>.
//!
//! \return Returns \b true if interrupts were already disabled when the
//! function was called or \b false if they were initially enabled.
//
//*****************************************************************************
bool
IntMasterDisable(void)
{
    printf("Disable processor interrupts.\r\n");
    return true;
}
//*****************************************************************************
//
//! Sets the NVIC VTable base.
//!
//! \param ulVtableBase specifies the new base address of VTable
//!
//! This function is used to specify a new base address for the VTable.
//! This function must be called before using IntRegister() for registering
//! any interrupt handler.
//!
//!
//! \return None.
//
//*****************************************************************************
void
IntVTableBaseSet(unsigned long ulVtableBase)
{
    printf("Interrupt VTable base set to 0x%lx.\r\n", ulVtableBase);
}

//*****************************************************************************
//
//! Registers a function to be called when an interrupt occurs.
//!
//! \param ulInterrupt specifies the interrupt in question.
//! \param pfnHandler is a pointer to the function to be called.
//!
//! This function is used to specify the handler function to be called when the
//! given interrupt is asserted to the processor.  When the interrupt occurs,
//! if it is enabled (via IntEnable()), the handler function will be called in
//! interrupt context.  Since the handler function can preempt other code, care
//! must be taken to protect memory or peripherals that are accessed by the
//! handler and other non-handler code.
//!
//!
//! \return None.
//
//*****************************************************************************
void
IntRegister(unsigned long ulInterrupt, void (* pfnHandler)(void))
{
    printf("Interrupt %lu set to handler 0x%lx", ulInterrupt, (unsigned long)&pfnHandler);
}

//*****************************************************************************
//
//! Unregisters the function to be called when an interrupt occurs.
//!
//! \param ulInterrupt specifies the interrupt in question.
//!
//! This function is used to indicate that no handler should be called when the
//! given interrupt is asserted to the processor.  The interrupt source will be
//! automatically disabled (via IntDisable()) if necessary.
//!
//! \sa IntRegister() for important information about registering interrupt
//! handlers.
//!
//! \return None.
//
//*****************************************************************************
void
IntUnregister(unsigned long ulInterrupt)
{
    printf("Interrupt %lu disabled", ulInterrupt);
}

//*****************************************************************************
//
//! Sets the priority grouping of the interrupt controller.
//!
//! \param ulBits specifies the number of bits of preemptable priority.
//!
//! This function specifies the split between preemptable priority levels and
//! subpriority levels in the interrupt priority specification.  The range of
//! the grouping values are dependent upon the hardware implementation; on
//! the CC3200 , three bits are available for hardware interrupt
//! prioritization and therefore priority grouping values of three through
//! seven have the same effect.
//!
//! \return None.
//
//*****************************************************************************
void
IntPrioritySet(unsigned long ulInterrupt, unsigned char ucPriority)
{
    printf("Interrupt %lu set to priority %d \r\n", ulInterrupt, ucPriority);
}

//*****************************************************************************
//
//! Gets the priority of an interrupt.
//!
//! \param ulInterrupt specifies the interrupt in question.
//!
//! This function gets the priority of an interrupt.  See IntPrioritySet() for
//! a definition of the priority value.
//!
//! \return Returns the interrupt priority, or -1 if an invalid interrupt was
//! specified.
//
//*****************************************************************************
long
IntPriorityGet(unsigned long ulInterrupt)
{
    return 0;
}

//*****************************************************************************
//
//! Enables an interrupt.
//!
//! \param ulInterrupt specifies the interrupt to be enabled.
//!
//! The specified interrupt is enabled in the interrupt controller.  Other
//! enables for the interrupt (such as at the peripheral level) are unaffected
//! by this function.
//!
//! \return None.
//
//*****************************************************************************
void
IntEnable(unsigned long ulInterrupt)
{
    printf("Interrupt %lu enabled\r\n", ulInterrupt);
}

//*****************************************************************************
//
//! Disables an interrupt.
//!
//! \param ulInterrupt specifies the interrupt to be disabled.
//!
//! The specified interrupt is disabled in the interrupt controller.  Other
//! enables for the interrupt (such as at the peripheral level) are unaffected
//! by this function.
//!
//! \return None.
//
//*****************************************************************************
void
IntDisable(unsigned long ulInterrupt)
{
    printf("Interrupt %lu disabled\r\n", ulInterrupt);
}

//*****************************************************************************
//
//! Pends an interrupt.
//!
//! \param ulInterrupt specifies the interrupt to be pended.
//!
//! The specified interrupt is pended in the interrupt controller.  This will
//! cause the interrupt controller to execute the corresponding interrupt
//! handler at the next available time, based on the current interrupt state
//! priorities.  For example, if called by a higher priority interrupt handler,
//! the specified interrupt handler will not be called until after the current
//! interrupt handler has completed execution.  The interrupt must have been
//! enabled for it to be called.
//!
//! \return None.
//
//*****************************************************************************

void
IntPriorityMaskSet(unsigned long ulPriorityMask)
{
    printf("Interrupt priority mask set to %lu\r\n", ulPriorityMask);
}

//*****************************************************************************
//
//! Gets the priority masking level
//!
//! This function gets the current setting of the interrupt priority masking
//! level.  The value returned is the priority level such that all interrupts
//! of that and lesser priority are masked.  A value of 0 means that priority
//! masking is disabled.
//!
//! Smaller numbers correspond to higher interrupt priorities.  So for example
//! a priority level mask of 4 will allow interrupts of priority level 0-3,
//! and interrupts with a numerical priority of 4 and greater will be blocked.
//!
//! The hardware priority mechanism will only look at the upper N bits of the
//! priority level (where N is 3), so any
//! prioritization must be performed in those bits.
//!
//! \return Returns the value of the interrupt priority level mask.
//
//*****************************************************************************
unsigned long
IntPriorityMaskGet(void)
{
    return 0;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
