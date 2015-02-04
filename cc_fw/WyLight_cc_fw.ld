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

HEAP_SIZE = 0x00008000;

MEMORY
{
    SRAM (rwx) : ORIGIN = 0x20012000, LENGTH = 0x0002BFFF
}

stack_size = 0xFFFF;
_stack_start = ORIGIN(SRAM)+LENGTH(SRAM);
_stack_end = _stack_start - stack_size;

SECTIONS
{
   .text :
    {
    	_text = .;
        KEEP(*(.intvecs))
    	CREATE_OBJECT_SYMBOLS
		*(.text .text.* .gnu.linkonce.t.*) 
		*(.plt)
		*(.gnu.warning)
		*(.glue_7t)
        *(.glue_7)
		
		. = ALIGN(0x4);
		/* These are for static constructors and destructors under ELF */
		KEEP (*crtbegin.o(.ctors))
		KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors)) KEEP (*(SORT(.ctors.*)))
		KEEP (*crtend.o(.ctors))
		KEEP (*crtbegin.o(.dtors))
		KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors)) KEEP (*(SORT(.dtors.*)))
		KEEP (*crtend.o(.dtors))
		
        *(.text*)
        *(.rodata .rodata.* .gnu.linkonce.r.*)
		*(.ARM.extab* .gnu.linkonce.armextab.*)
		*(.init) 
		*(.fini)
		. = ALIGN(8);
        _etext = .;
    } > SRAM

    .ARM : 
    {
    	__exidx_start = .;
      	*(.ARM.exidx* .gnu.linkonce.armexidx.*)
      	__exidx_end = .;
    } > SRAM

    __init_data = .;

    .data : AT(__init_data)
    {
        _data = .;
        KEEP(*(.jcr)) 
        *(.got.plt) *(.got) 
        *(.shdata)
		*(.data .data.* .gnu.linkonce.d.*)
		. = ALIGN (8);
        _edata = .;
    } > SRAM

	. = ALIGN(4);
	.init_array :
	{
		__init_array_start = .;
		KEEP (*(.init_array*))
		__init_array_end = .;
	} > SRAM

    .bss :
    {
        _bss = .;
        *(.shbss)
		*(.bss .bss.* .gnu.linkonce.b.*)        
		*(COMMON)
        _ebss = .;
    } > SRAM

    .heap :
    {
	_heap = .;
	. = . + HEAP_SIZE;
	. = ALIGN(8);
	_eheap = .;

    } > SRAM
}

