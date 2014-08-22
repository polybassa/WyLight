
MEMORY
{
    FLASH (rx) : ORIGIN = 0x20004000, LENGTH = 0xBFFF
    SRAM (rwx) : ORIGIN = 0x20010000, LENGTH = 0x3FFF
}

SECTIONS
{
    .text :
    {
        KEEP(*(.isr_vector))
        *(.text*)
        *(.rodata*)
        _etext = .;
    } > FLASH

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
