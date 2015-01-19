/*#******************************************************************************
# Copyright (C) 2015 Nils Weiss, Patrick Bruenn.
#
# This file is part of WyLight.
#
# WyLight is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# WyLight is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with WyLight.  If not, see <http://www.gnu.org/licenses/>.

#******************************************************************************/
/*#******************************************************************************
File   : gcc_arm_ram.ld

Author : Hoang Nguyen Hoan          Oct. 1, 2014

Desc   : Generic common linker script sections.  General sections
         definitions for of GCC compiler.  This file is to be inlcuded
	 	 in the device specific linker script.

		This is script is for RAM based only processor such as the 
		TI CC3200
		
	 	Require memory region name definition as follow
		 
	        SRAM : SRAM memory region for both data & code  

Copyright (c) 2014, I-SYST, all rights reserved

Permission to use, copy, modify, and distribute this software for any purpose
with or without fee is hereby granted, provided that the above copyright
notice and this permission notice appear in all copies, and none of the
names : I-SYST or its contributors may be used to endorse or
promote products derived from this software without specific prior written
permission.

For info or contributing contact : hnhoan at i-syst dot com

THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */
/******************************************************************************/

HEAP_SIZE = 0x00000100;

MEMORY
{
    SRAM (rwx) : ORIGIN = 0x20004000, LENGTH = 0x0000DFFF
}

stack_size = 0x2000;
_stack_start = ORIGIN(SRAM)+LENGTH(SRAM);
_stack_end = _stack_start - stack_size;


__CheckSum = 0;

SECTIONS {

     /* Require ARM vectors located at begining of RAM region at address 0*/
	.ivector : ALIGN(4)
	{
		__text_start__ = .;

		/* interrupt vector */
		KEEP(*(.intvect))
		FILL(0xff) /* fill unsused area to have consistant checksum */
		. = __text_start__ + 0x400;
	} > SRAM
	
	/* This is where the firmware informaton is stored. Usefull for bootloader */
	.Version : ALIGN(4)
	{
		KEEP(*(.Version))
		LONG(__CheckSum);
	} > SRAM
	
	/* Code region starts here */
	.text : ALIGN(4)
	{	
		/* Application Entry point starts here.  Normaly is the ResetEntry function */ 
		KEEP(*(.AppStart))
		
		KEEP(*(.init))
		KEEP(*(.fini))
		
		/* .ctors */
		*crtbegin.o(.ctors)
		*crtbegin?.o(.ctors)
		*(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
		*(SORT(.ctors.*))
		*(.ctors)

		/* .dtors */
 		*crtbegin.o(.dtors)
 		*crtbegin?.o(.dtors)
 		*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
 		*(SORT(.dtors.*))
 		*(.dtors)

		*(.text*)
		*(.rodata .rodata.* .gnu.linkonce.r.* .rodata1)
	} > SRAM
    

	.ARM.extab : 
	{
		*(.ARM.extab* .gnu.linkonce.armextab.*)
	} > SRAM

	PROVIDE (__exidx_start = .);
	.ARM.exidx :
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} > SRAM
	PROVIDE (__exidx_end = .);

	.eh_frame_hdr : 
	{ 
		*(.eh_frame_hdr) 
	} > SRAM
	
	__text_end__ = .;
	
	.eh_frame : ONLY_IF_RW 
	{ 
		. = ALIGN(4);
		*(.eh_frame) 
	} > SRAM
	
	.gcc_except_table : ONLY_IF_RW 
	{ 
		. = ALIGN(4);
		*(.gcc_except_table .gcc_except_table.*) 
	} > SRAM
	
	__init_data = .;
		
	.data : AT(__init_data)
	{
		. = ALIGN(4);
		_data = .;
		/* preinit data */
		PROVIDE (__preinit_array_start = .);
		KEEP(*(.preinit_array))
		PROVIDE (__preinit_array_end = .);

		. = ALIGN(4);
		/* init data */
		PROVIDE (__init_array_start = .);
		*(SORT(.init_array.*))
		KEEP(*(.init_array))
		PROVIDE (__init_array_end = .);

		. = ALIGN(4);
		/* finit data */
		PROVIDE (__fini_array_start = .);
		*(SORT(.fini_array.*))
		KEEP(*(.fini_array))
		PROVIDE (__fini_array_end = .);

		. = ALIGN(4);        

		*(vtable)
		*(.data*)
		*(.fastrun*)

		*(.jcr)

		/* All data end */
		_edata = .;

	} > SRAM
	
	__data_start__ = ADDR(.data);
   	__data_loc__ = LOADADDR(.data);
   	__data_size__ = SIZEOF(.data);
	
	.bss : ALIGN(4)
	{
		_bss = .;
		*(.bss*)
		*(COMMON)
      	*(.gnu.linkonce.b.*)
		_ebss = .;
		PROVIDE(end = .);		              
	} > SRAM
        __bss_size__ = SIZEOF(.bss);
	
    .heap :
    {
        _heap = .;
        . = . + HEAP_SIZE;
        . = ALIGN(8);
        _eheap = .;
    } > SRAM

}
