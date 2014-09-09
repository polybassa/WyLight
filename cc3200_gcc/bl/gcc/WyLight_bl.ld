/******************************************************************************
*
*   copyright (c) 2013 texas instruments incorporated
*
*   All rights reserved. Property of Texas Instruments Incorporated.
*   Restricted rights to use, duplicate or disclose this code are
*   granted through contract.
*
*   The program may not be used without the written permission of
*   Texas Instruments Incorporated or against the terms and conditions
*   stipulated in the agreement under which this program has been supplied,
*   and under no circumstances can it be used with non-TI connectivity device.
*
******************************************************************************/

HEAP_SIZE = 0x00000100;

MEMORY
{
    SRAM (rwx) : ORIGIN = 0x20004000, LENGTH = 0x0000DFFF
}

stack_size = 0x2000;
_stack_start = ORIGIN(SRAM)+LENGTH(SRAM);
_stack_end = _stack_start - stack_size;

SECTIONS
{
    .text :
    {
        _text = .;
        KEEP(*(.intvecs))
        *(.text*)
        *(.rodata*)
	*(.ARM.extab* .gnu.linkonce.armextab.*)
	. = ALIGN(8);
        _etext = .;
    } > SRAM

    .ARM : {
    __exidx_start = .;
      *(.ARM.exidx*)
      __exidx_end = .;
    } > SRAM

    __init_data = .;

    .data : AT(__init_data)
    {
        _data = .;
        *(.data*)
	. = ALIGN (8);
        _edata = .;
    } > SRAM

    .bss :
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } > SRAM

    .heap :
    {
	_heap = .;
	. = . + HEAP_SIZE;
	. = ALIGN(8);
	_eheap = .;

    }
}

