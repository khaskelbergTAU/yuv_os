ENTRY(_start)

SECTIONS {

    . = 0x100000;

    .multiboot : {
        *(.multiboot)
    } 

    .bootloader : {
        *(.bootloader)
    }

    . += 0xc0000000;
    __kernel_start = .;

    .text : AT(ADDR(.text) - 0xc0000000) {
        *(.text)
    }

    .rodata ALIGN(4K) : AT(ADDR(.rodata)-0xC0000000){
        *(.data)
    }

    .bss ALIGN(4K) : AT(ADDR(.bss)-0xC0000000){
        *(COMMON)
        *(.bss)
    }
    __kernel_end = .;
}