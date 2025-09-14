ENTRY(reset_handler)

MEMORY
{
    FLASH (rx) : ORIGIN = 0X08000000 LENGTH = 512K
    SRAM (rwx) : ORIGIN = 0X20000000 LENGTH = 64K
}



SECTIONS {
    .text : {
        *(.vectors)
        *(.text*)
        *(.rodata*)
    } > FLASH

    _sidata = LOADADDR(.data);

    .data : AT (ADDR(.text) + SIZEOF(.text)) {
        _sdata = .;
        *(.data*)
        _edata = .;
    } > SRAM

    .bss : {
        _sbss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } > SRAM

    .stack (NOLOAD) : {   /* The NOLOAD attribute tells the loader not to initialize this region — the memory is reserved at runtime, not in the binary. */
        _stack_bottom = .;  /* Current position is the stack bottom*/
        . = . + 0x1000;     /* Move to 0x1000 over */
        _stack_top = .;     /* Mark that position as the stack top*/
    } > SRAM
}