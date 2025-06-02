ENTRY(KernelMain)

SECTIONS {
    . = 0x200000;
    .text : { *(.text*) }
    .rodata : { *(.rodata*) }
    .data : { *(.data*) }
    .bss : { *(.bss*) }
    /DISCARD/ : { *(.eh_frame*) } 
}