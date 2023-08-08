ENTRY(_start)

KERNEL_START = 0xffffffff80000000;
KERNEL_OFFSET = 0x100000;
TABLES_OFFSET = 0x009000;
SECTIONS {

    . = KERNEL_OFFSET;

    .multiboot : {
        *(.multiboot)
    } 

    .bootloader : {
        *(.bootloader)
    }

    . += KERNEL_START;
    __kernel_start = .;

    . = KERNEL_START + TABLES_OFFSET;
    .tables : AT(ADDR(.tables) - KERNEL_START) {
        *(.tables)
    }
    . += 0x4000;

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
    __kernel_end = .;
}