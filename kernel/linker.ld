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
    
    .text : AT(ADDR(.text) - KERNEL_START) {
        *(.init)
        *(.text)
        *(.text.*)
        *(.fini)
    }

    .rodata ALIGN(4K) : AT(ADDR(.rodata) - KERNEL_START){
        *(.data)
        *(.rodata.*)
    }

    .tables ALIGN(4K) : AT(ADDR(.tables) - KERNEL_START) {
        *(.tables)
    }

    .bss ALIGN(4K) : AT(ADDR(.bss) - KERNEL_START){
        *(COMMON)
        *(.bss)
        *(.bss.*)
    }

    __kernel_end = .;
}