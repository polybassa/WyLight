
MEMORY
{
    SRAM  (rwx) : ORIGIN = 0x20004000, LENGTH = 0x2FFFF
}

SECTIONS
{
    .text :
    {
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

    .data : AT (ADDR(.text) + SIZEOF(.text) + 0x500)
    {
        _data = .;
        *(vtable)
        *(.data*)
        _edata = .;
    } > SRAM

    .bss :
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
        . = ALIGN (8);
       _end = .;
    } > SRAM
}

/* end of allocated ram _end */
PROVIDE( _HEAP_START = _end );
PROVIDE( _heap = _end );

/* end of the heap -> align 8 byte */ 
PROVIDE ( _HEAP_END = ALIGN(ORIGIN(SRAM) + LENGTH(SRAM) - 8 ,8) );
PROVIDE ( _eheap = ALIGN(ORIGIN(SRAM) + LENGTH(SRAM) - 8 ,8) );


PROVIDE( __init_data = _data );
