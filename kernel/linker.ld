ENTRY(_start)

KERNEL_START = 0xffffffff80000000;
KERNEL_OFFSET = 0x100000;
TABLES_OFFSET = 0x109000;
SECTIONS {

    . = KERNEL_OFFSET;

    .multiboot : {
        *(.multiboot)
    } 

    __boot_start = .;
    .bootloader : {
        *(.bootloader)
    }

    . += KERNEL_START;
    . = ALIGN(4K);
    __kernel_start = .;
    
    .tables ALIGN(4K) : AT(ADDR(.tables) - KERNEL_START) {
        *(.tables)
    }

    .text : AT(ADDR(.text) - KERNEL_START) {
        *(.text)
    }

    .rodata ALIGN(4K) : AT(ADDR(.rodata) - KERNEL_START){
        *(.data)
    }

    .bss ALIGN(4K) : AT(ADDR(.bss) - KERNEL_START){
        *(COMMON)
        *(.bss)
    }

    . = ALIGN(4K);
    __kernel_end = .;
}