MEMORY
{
	FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 64K
	SRAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 8K
}

SECTIONS
{
	.text :
	{
		LONG( ORIGIN(SRAM) + LENGTH(SRAM) )
		*(.vectors)
		*(.text)
		*(.text.*)
		*(.rodata)
		*(.rodata.*)
		__etext = ALIGN(4) ;
	} > FLASH

	.data ORIGIN(SRAM) : AT (__etext)
	{
		__data = . ;
		*(.data)
		__edata = . ;
	} > SRAM

	.bss ALIGN(4) :
	{
		__bss = . ;
		*(.bss)
		*(COMMON)
		__ebss = . ;
	} > SRAM

	/DISCARD/ :
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	}
}
